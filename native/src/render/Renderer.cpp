#include "football/render/Renderer.hpp"

#include <android/log.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace football {
namespace {
constexpr char kLogTag[] = "FootballLegendsRender";
constexpr float kPi = 3.14159265358979323846F;
constexpr char kVertexShader[] = R"(#version 300 es
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec4 aColor;
layout(location=3) in vec2 aUv;
layout(location=4) in vec4 aBoneIndices;
layout(location=5) in vec4 aBoneWeights;
layout(location=6) in float aMaterial;
uniform mat4 uMvp;
uniform mat4 uModel;
uniform mat4 uBones[5];
out vec3 vNormal;
out vec4 vColor;
out vec3 vWorldPosition;
out vec2 vUv;
out float vMaterial;
void main() {
    mat4 skin = uBones[int(aBoneIndices.x)] * aBoneWeights.x
              + uBones[int(aBoneIndices.y)] * aBoneWeights.y
              + uBones[int(aBoneIndices.z)] * aBoneWeights.z
              + uBones[int(aBoneIndices.w)] * aBoneWeights.w;
    vec4 localPosition = skin * vec4(aPosition, 1.0);
    vec4 world = uModel * localPosition;
    vWorldPosition = world.xyz;
    vNormal = normalize(mat3(uModel) * mat3(skin) * aNormal);
    vColor = aColor;
    vUv = aUv;
    vMaterial = aMaterial;
    gl_Position = uMvp * localPosition;
})";
constexpr char kFragmentShader[] = R"(#version 300 es
precision highp float;
in vec3 vNormal;
in vec4 vColor;
in vec3 vWorldPosition;
in vec2 vUv;
in float vMaterial;
uniform vec3 uCameraPosition;
uniform vec3 uLightDirection;
uniform sampler2D uMaterialTexture;
uniform vec3 uTeamTint;
out vec4 outColor;
void main() {
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(-uLightDirection);
    vec3 viewDir = normalize(uCameraPosition - vWorldPosition);
    float diffuse = max(dot(normal, lightDir), 0.0);
    float texel = texture(uMaterialTexture, fract(vUv * 2.7)).r;
    float grain = mix(0.94, 1.06, texel);
    float grassBand = (vMaterial < 0.5) ? (0.965 + 0.035 * sin(vWorldPosition.x * 0.62)) : 1.0;
    float fabricWeave = (vMaterial > 0.5 && vMaterial < 1.5)
        ? (0.975 + 0.025 * sin(vUv.x * 420.0) * sin(vUv.y * 360.0)) : 1.0;
    vec3 base = vColor.rgb * uTeamTint * grain * grassBand * fabricWeave;
    float roughness = (vMaterial > 0.5 && vMaterial < 1.5) ? 0.48 : 0.82;
    vec3 halfDir = normalize(lightDir + viewDir);
    float specular = pow(max(dot(normal, halfDir), 0.0), mix(48.0, 10.0, roughness));
    float lighting = 0.30 + diffuse * 0.82;
    vec3 lit = base * lighting + vec3(0.16,0.20,0.24) * specular * (1.0 - roughness) * 0.32;
    float distanceToCamera = length(vWorldPosition - uCameraPosition);
    float fog = smoothstep(100.0, 180.0, distanceToCamera);
    vec3 sky = vec3(0.035, 0.065, 0.105);
    outColor = vec4(mix(lit, sky, fog * 0.72), vColor.a);
})";

struct Vec3 { float x,y,z; };
struct Mat4 { std::array<float,16> v{}; };
Mat4 identity() {
    Mat4 r{}; r.v[0]=r.v[5]=r.v[10]=r.v[15]=1.0F; return r;
}
Mat4 multiply(const Mat4& a,const Mat4& b) {
    Mat4 r{};
    for(int c=0;c<4;++c) for(int row=0;row<4;++row)
        for(int k=0;k<4;++k) r.v[c*4+row]+=a.v[k*4+row]*b.v[c*4+k];
    return r;
}
Mat4 perspective(float fov,float aspect,float nearPlane,float farPlane) {
    Mat4 r{}; const float f=1.0F/std::tan(fov*.5F);
    r.v[0]=f/aspect; r.v[5]=f; r.v[10]=(farPlane+nearPlane)/(nearPlane-farPlane);
    r.v[11]=-1.0F; r.v[14]=(2.0F*farPlane*nearPlane)/(nearPlane-farPlane); return r;
}
Vec3 normalize(Vec3 a) {
    const float n=std::sqrt(std::max(a.x*a.x+a.y*a.y+a.z*a.z,.000001F));
    return {a.x/n,a.y/n,a.z/n};
}
Vec3 subtract(Vec3 a,Vec3 b) { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
Vec3 cross(Vec3 a,Vec3 b) { return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x}; }
float dot(Vec3 a,Vec3 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
Mat4 lookAt(Vec3 eye,Vec3 center,Vec3 up) {
    const Vec3 f=normalize(subtract(center,eye)), s=normalize(cross(f,up)), u=cross(s,f);
    Mat4 r=identity();
    r.v[0]=s.x; r.v[1]=u.x; r.v[2]=-f.x;
    r.v[4]=s.y; r.v[5]=u.y; r.v[6]=-f.y;
    r.v[8]=s.z; r.v[9]=u.z; r.v[10]=-f.z;
    r.v[12]=-dot(s,eye); r.v[13]=-dot(u,eye); r.v[14]=dot(f,eye);
    return r;
}
Mat4 rotationAroundX(Vec3 pivot,float angle) {
    Mat4 t=identity(),rot=identity(),back=identity();
    const float c=std::cos(angle),s=std::sin(angle);
    rot.v[5]=c; rot.v[6]=s; rot.v[9]=-s; rot.v[10]=c;
    t.v[12]=pivot.x; t.v[13]=pivot.y; t.v[14]=pivot.z;
    back.v[12]=-pivot.x; back.v[13]=-pivot.y; back.v[14]=-pivot.z;
    return multiply(multiply(t,rot),back);
}
Mat4 modelAt(float x,float y,float z,float yaw) {
    Mat4 r=identity(); const float c=std::cos(yaw),s=std::sin(yaw);
    r.v[0]=c; r.v[2]=-s; r.v[8]=s; r.v[10]=c;
    r.v[12]=x; r.v[13]=y; r.v[14]=z; return r;
}
void configureVertexLayout() {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(LobbyVertex),reinterpret_cast<void*>(offsetof(LobbyVertex,position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(LobbyVertex),reinterpret_cast<void*>(offsetof(LobbyVertex,normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,sizeof(LobbyVertex),reinterpret_cast<void*>(offsetof(LobbyVertex,color)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(LobbyVertex),reinterpret_cast<void*>(offsetof(LobbyVertex,uv)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,sizeof(LobbyVertex),reinterpret_cast<void*>(offsetof(LobbyVertex,boneIndices)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,sizeof(LobbyVertex),reinterpret_cast<void*>(offsetof(LobbyVertex,boneWeights)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6,1,GL_FLOAT,GL_FALSE,sizeof(LobbyVertex),reinterpret_cast<void*>(offsetof(LobbyVertex,material)));
}
} // namespace

Renderer::~Renderer() { destroy(); }

GLuint Renderer::compileShader(GLenum type,const char* source) {
    const GLuint shader=glCreateShader(type);
    if(!shader) return 0;
    glShaderSource(shader,1,&source,nullptr); glCompileShader(shader);
    GLint ok=GL_FALSE; glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
    if(ok!=GL_TRUE) {
        char log[2048]{}; glGetShaderInfoLog(shader,sizeof(log),nullptr,log);
        __android_log_print(ANDROID_LOG_ERROR,kLogTag,"Shader compilation failed: %s",log);
        glDeleteShader(shader); return 0;
    }
    return shader;
}

void Renderer::initialize() {
    destroy();
    scene_.build();
    const GLuint vs=compileShader(GL_VERTEX_SHADER,kVertexShader);
    const GLuint fs=compileShader(GL_FRAGMENT_SHADER,kFragmentShader);
    if(!vs||!fs) { if(vs)glDeleteShader(vs); if(fs)glDeleteShader(fs); return; }
    program_=glCreateProgram();
    glAttachShader(program_,vs); glAttachShader(program_,fs); glLinkProgram(program_);
    glDeleteShader(vs); glDeleteShader(fs);
    GLint linked=GL_FALSE; glGetProgramiv(program_,GL_LINK_STATUS,&linked);
    if(linked!=GL_TRUE) {
        char log[2048]{}; glGetProgramInfoLog(program_,sizeof(log),nullptr,log);
        __android_log_print(ANDROID_LOG_ERROR,kLogTag,"Program link failed: %s",log);
        destroy(); return;
    }
    mvpLocation_=glGetUniformLocation(program_,"uMvp");
    modelLocation_=glGetUniformLocation(program_,"uModel");
    cameraLocation_=glGetUniformLocation(program_,"uCameraPosition");
    lightLocation_=glGetUniformLocation(program_,"uLightDirection");
    bonesLocation_=glGetUniformLocation(program_,"uBones[0]");
    textureLocation_=glGetUniformLocation(program_,"uMaterialTexture");
    teamTintLocation_=glGetUniformLocation(program_,"uTeamTint");
    createMaterialTexture();
    uploadScene();
    initialized_=environmentVao_!=0&&playerVao_!=0;
    if(initialized_) __android_log_print(ANDROID_LOG_INFO,kLogTag,"3D lobby stadium and player models initialized.");
}

void Renderer::createMaterialTexture() {
    constexpr int size=128;
    std::vector<std::uint8_t> pixels(size*size*4);
    for(int y=0;y<size;++y) for(int x=0;x<size;++x) {
        const std::uint32_t hash=static_cast<std::uint32_t>(x*374761393U+y*668265263U+0x9e3779b9U);
        const std::uint32_t noise=(hash^(hash>>13U))*1274126177U;
        const std::uint8_t value=static_cast<std::uint8_t>(112U+(noise%48U));
        const std::size_t offset=static_cast<std::size_t>((y*size+x)*4);
        pixels[offset]=value; pixels[offset+1]=value; pixels[offset+2]=value; pixels[offset+3]=255U;
    }
    glGenTextures(1,&materialTexture_);
    glBindTexture(GL_TEXTURE_2D,materialTexture_);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,size,size,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D,0);
}

void Renderer::uploadScene() {
    const auto& environment=scene_.environmentVertices();
    glGenVertexArrays(1,&environmentVao_); glGenBuffers(1,&environmentBuffer_);
    glBindVertexArray(environmentVao_); glBindBuffer(GL_ARRAY_BUFFER,environmentBuffer_);
    glBufferData(GL_ARRAY_BUFFER,static_cast<GLsizeiptr>(environment.size()*sizeof(LobbyVertex)),
                 environment.data(),GL_STATIC_DRAW);
    configureVertexLayout(); glBindVertexArray(0);

    const auto& player=scene_.playerVertices();
    glGenVertexArrays(1,&playerVao_); glGenBuffers(1,&playerBuffer_);
    glBindVertexArray(playerVao_); glBindBuffer(GL_ARRAY_BUFFER,playerBuffer_);
    glBufferData(GL_ARRAY_BUFFER,static_cast<GLsizeiptr>(player.size()*sizeof(LobbyVertex)),
                 player.data(),GL_STATIC_DRAW);
    configureVertexLayout(); glBindVertexArray(0);

    const auto& ball=scene_.ballVertices();
    glGenVertexArrays(1,&ballVao_); glGenBuffers(1,&ballBuffer_);
    glBindVertexArray(ballVao_); glBindBuffer(GL_ARRAY_BUFFER,ballBuffer_);
    glBufferData(GL_ARRAY_BUFFER,static_cast<GLsizeiptr>(ball.size()*sizeof(LobbyVertex)),
                 ball.data(),GL_STATIC_DRAW);
    configureVertexLayout(); glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Renderer::resize(int width,int height) {
    width_=std::max(width,1); height_=std::max(height,1);
    glViewport(0,0,width_,height_);
}

void Renderer::render() {
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); glDisable(GL_CULL_FACE);
    glClearColor(.035F,.065F,.105F,1.0F);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(!initialized_) return;

    using Clock=std::chrono::steady_clock;
    static const auto start=Clock::now();
    const float seconds=std::chrono::duration<float>(Clock::now()-start).count();
    const float orbit=cinematicCamera_?std::sin(seconds*.10F)*.055F:0.0F;
    const Vec3 matchTarget{matchCameraX_,1.0F,matchCameraZ_};
    const Vec3 eye=matchActive_
        ? Vec3{matchCameraX_+std::sin(orbit)*2.0F,27.0F,matchCameraZ_-43.0F}
        : Vec3{std::sin(orbit)*96.0F,48.0F,std::cos(orbit)*96.0F};
    const Vec3 target=matchActive_?matchTarget:Vec3{0,5.2F,0};
    const Mat4 view=lookAt(eye,target,{0,1,0});
    const Mat4 projection=perspective((matchActive_?54.0F:48.0F)*kPi/180.0F,static_cast<float>(width_)/height_,.1F,240.0F);
    const Mat4 vp=multiply(projection,view);
    const Mat4 world=identity();

    glUseProgram(program_);
    glUniform3f(cameraLocation_,eye.x,eye.y,eye.z);
    glUniform3f(lightLocation_,-.35F,-1.0F,-.25F);
    glUniform1i(textureLocation_,0);
    glUniform3f(teamTintLocation_,1.0F,1.0F,1.0F);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,materialTexture_);
    const Mat4 rootBones[5]={identity(),identity(),identity(),identity(),identity()};
    glUniformMatrix4fv(bonesLocation_,5,GL_FALSE,rootBones[0].v.data());
    glUniformMatrix4fv(modelLocation_,1,GL_FALSE,world.v.data());
    glUniformMatrix4fv(mvpLocation_,1,GL_FALSE,vp.v.data());
    glBindVertexArray(environmentVao_);
    glDrawArrays(GL_TRIANGLES,0,static_cast<GLsizei>(scene_.environmentVertices().size()));

    glBindVertexArray(playerVao_);
    const auto& lobbyPositions=scene_.playerPositions();
    const std::size_t count=matchActive_?MatchSimulation::kPlayerCount:lobbyPositions.size();
    const GLsizei playerCount=static_cast<GLsizei>(scene_.playerVertices().size());
    for(std::size_t i=0;i<count;++i) {
        const float px=matchActive_?matchPositions_[i].x:lobbyPositions[i].x;
        const float pz=matchActive_?matchPositions_[i].z:lobbyPositions[i].z;
        const float yaw=matchActive_?matchPositions_[i].yaw:lobbyPositions[i].yaw;
        const float phase=seconds*1.65F+static_cast<float>(i)*.61F;
        const float pose=matchActive_?matchPositions_[i].pose:0.0F;
        const float poseTime=matchActive_?matchPositions_[i].poseTime:0.0F;
        const float poseBlend=std::min(1.0F,poseTime*3.5F);
        const bool falling=pose==5.0F;
        const bool recovering=pose==6.0F;
        const bool highCatching=pose==7.0F;
        const float bob=(pose==3.0F||falling)?-.24F*poseBlend:(recovering?-.12F*(1.0F-poseBlend):std::sin(phase*2.0F)*.012F);
        const Mat4 model=modelAt(px,bob,pz,yaw);
        const Mat4 mvp=multiply(vp,model);
        const float protestBlend=(pose==1.0F||pose==2.0F)?std::min(1.0F,poseTime*2.8F):0.0F;
        const bool diving=pose==3.0F;
        const bool catching=pose==4.0F;
        const float armSwing=(catching||highCatching)?(highCatching?1.42F:1.15F)*poseBlend:(diving?1.25F*poseBlend:std::sin(phase)*.075F+1.05F*protestBlend);
        const float oppositeArm=(catching||highCatching)?armSwing:(diving?-1.25F*poseBlend:-armSwing);
        const float legSwing=(diving||falling)?.45F:std::sin(phase)*.045F;
        const float bodyLean=diving?1.20F*poseBlend:(falling?1.48F*poseBlend:(recovering?0.65F*(1.0F-poseBlend):0.0F));
        const Mat4 rootPose=bodyLean>0.0F?rotationAroundX({0,.82F,0},bodyLean):identity();
        const Mat4 bones[5]={
            rootPose,
            rotationAroundX({-.20F,1.31F,0},armSwing),
            rotationAroundX({.20F,1.31F,0},oppositeArm),
            rotationAroundX({-.115F,.80F,0},-legSwing),
            rotationAroundX({.115F,.80F,0},legSwing)
        };
        if(matchActive_) {
            if(i < 11) glUniform3f(teamTintLocation_,0.28F,0.68F,1.0F);
            else glUniform3f(teamTintLocation_,1.0F,0.32F,0.25F);
        } else glUniform3f(teamTintLocation_,1.0F,1.0F,1.0F);
        glUniformMatrix4fv(bonesLocation_,5,GL_FALSE,bones[0].v.data());
        glUniformMatrix4fv(modelLocation_,1,GL_FALSE,model.v.data());
        glUniformMatrix4fv(mvpLocation_,1,GL_FALSE,mvp.v.data());
        glDrawArrays(GL_TRIANGLES,0,playerCount);
    }
    if(matchActive_) {
        const Mat4 refereeModel=modelAt(refereePosition_.x,0.0F,refereePosition_.y,0.0F);
        const Mat4 refereeMvp=multiply(vp,refereeModel);
        const Mat4 refereeBones[5]={identity(),rotationAroundX({-.20F,1.31F,0},.45F),rotationAroundX({.20F,1.31F,0},-.45F),identity(),identity()};
        glUniform3f(teamTintLocation_,.12F,.15F,.20F);
        glUniformMatrix4fv(bonesLocation_,5,GL_FALSE,refereeBones[0].v.data());
        glUniformMatrix4fv(modelLocation_,1,GL_FALSE,refereeModel.v.data());
        glUniformMatrix4fv(mvpLocation_,1,GL_FALSE,refereeMvp.v.data());
        glBindVertexArray(playerVao_);
        glDrawArrays(GL_TRIANGLES,0,playerCount);
    }
    if(matchActive_ && offsideLineVisible_ && offsideLineVao_!=0) {
        const Mat4 lineModel=modelAt(0.0F,0.0F,offsideLine_,0.0F);
        const Mat4 lineMvp=multiply(vp,lineModel);
        glUniform3f(teamTintLocation_,1.0F,1.0F,1.0F);
        glUniformMatrix4fv(bonesLocation_,5,GL_FALSE,rootBones[0].v.data());
        glUniformMatrix4fv(modelLocation_,1,GL_FALSE,lineModel.v.data());
        glUniformMatrix4fv(mvpLocation_,1,GL_FALSE,lineMvp.v.data());
        glBindVertexArray(offsideLineVao_);
        glDrawArrays(GL_LINES,0,2);
    }
    if(matchActive_ && ballVao_!=0) {
        const Mat4 ballModel=modelAt(matchBallPosition_.x,.24F+matchBallHeight_,matchBallPosition_.y,0.0F);
        const Mat4 ballMvp=multiply(vp,ballModel);
        glUniform3f(teamTintLocation_,1.0F,1.0F,1.0F);
        glUniformMatrix4fv(bonesLocation_,5,GL_FALSE,rootBones[0].v.data());
        glUniformMatrix4fv(modelLocation_,1,GL_FALSE,ballModel.v.data());
        glUniformMatrix4fv(mvpLocation_,1,GL_FALSE,ballMvp.v.data());
        glBindVertexArray(ballVao_);
        glDrawArrays(GL_TRIANGLES,0,static_cast<GLsizei>(scene_.ballVertices().size()));
    }
    glBindVertexArray(0); glBindTexture(GL_TEXTURE_2D,0); glUseProgram(0);
}

void Renderer::setFormation(int formation) { scene_.setFormation(formation); }
void Renderer::setMatchSnapshot(const MatchSimulation& match) {
    matchActive_=match.state()==MatchState::Running || match.state()==MatchState::Paused;
    const auto& players=match.players();
    for(std::size_t i=0;i<players.size();++i) {
        matchPositions_[i]={players[i].position.y,players[i].position.x,players[i].team==0?3.14159265F:0.0F,players[i].protestSeconds,static_cast<float>(players[i].animation),players[i].animationSeconds};
    }
    matchBallPosition_={match.ball().position.y,match.ball().position.x};
    matchBallHeight_=match.ball().height;
    const int focusId=match.controlRole()==ControlRole::Player?match.controlledPlayer():-1;
    if(focusId>=0 && focusId<MatchSimulation::kPlayerCount) {
        const auto& focus=players[static_cast<std::size_t>(focusId)];
        matchCameraX_=focus.position.y*0.58F+match.ball().position.y*0.42F;
        matchCameraZ_=focus.position.x*0.58F+match.ball().position.x*0.42F;
    } else {
        matchCameraX_=match.ball().position.y;
        matchCameraZ_=match.ball().position.x;
    }
    refereePosition_={match.refereePosition().y,match.refereePosition().x};
    offsideLine_=match.offsideLine();
    offsideLineVisible_=match.offsideLineVisible();
}
void Renderer::setCinematicCamera(bool enabled) { cinematicCamera_=enabled; }

void Renderer::destroy() {
    if(environmentBuffer_) { glDeleteBuffers(1,&environmentBuffer_); environmentBuffer_=0; }
    if(environmentVao_) { glDeleteVertexArrays(1,&environmentVao_); environmentVao_=0; }
    if(playerBuffer_) { glDeleteBuffers(1,&playerBuffer_); playerBuffer_=0; }
    if(playerVao_) { glDeleteVertexArrays(1,&playerVao_); playerVao_=0; }
    if(ballBuffer_) { glDeleteBuffers(1,&ballBuffer_); ballBuffer_=0; }
    if(offsideLineBuffer_) { glDeleteBuffers(1,&offsideLineBuffer_); offsideLineBuffer_=0; }
    if(offsideLineVao_) { glDeleteVertexArrays(1,&offsideLineVao_); offsideLineVao_=0; }
    if(ballVao_) { glDeleteVertexArrays(1,&ballVao_); ballVao_=0; }
    matchActive_=false;
    if(program_) { glDeleteProgram(program_); program_=0; }
    if(materialTexture_) { glDeleteTextures(1,&materialTexture_); materialTexture_=0; }
    initialized_=false;
}

} // namespace football