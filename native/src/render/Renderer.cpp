#include "football/render/Renderer.hpp"

#include <android/log.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>

namespace football {
namespace {
constexpr char kLogTag[] = "FootballLegendsRender";
constexpr float kPi = 3.14159265358979323846F;
constexpr char kVertexShader[] = R"(#version 300 es
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec4 aColor;
uniform mat4 uMvp;
uniform mat4 uModel;
out vec3 vNormal;
out vec4 vColor;
out vec3 vWorldPosition;
void main() {
    vec4 world = uModel * vec4(aPosition, 1.0);
    vWorldPosition = world.xyz;
    vNormal = normalize(mat3(uModel) * aNormal);
    vColor = aColor;
    gl_Position = uMvp * vec4(aPosition, 1.0);
})";
constexpr char kFragmentShader[] = R"(#version 300 es
precision highp float;
in vec3 vNormal;
in vec4 vColor;
in vec3 vWorldPosition;
uniform vec3 uCameraPosition;
uniform vec3 uLightDirection;
out vec4 outColor;
void main() {
    float diffuse = max(dot(normalize(vNormal), normalize(-uLightDirection)), 0.0);
    float lighting = 0.42 + diffuse * 0.72;
    vec3 lit = vColor.rgb * lighting;
    float distanceToCamera = length(vWorldPosition - uCameraPosition);
    float fog = smoothstep(92.0, 165.0, distanceToCamera);
    vec3 sky = vec3(0.035, 0.065, 0.105);
    outColor = vec4(mix(lit, sky, fog * 0.78), vColor.a);
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
    uploadScene();
    initialized_=environmentVao_!=0&&playerVao_!=0;
    if(initialized_) __android_log_print(ANDROID_LOG_INFO,kLogTag,"3D lobby stadium and player models initialized.");
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
    const Vec3 eye{std::sin(orbit)*96.0F,48.0F,std::cos(orbit)*96.0F};
    const Vec3 target{0,5.2F,0};
    const Mat4 view=lookAt(eye,target,{0,1,0});
    const Mat4 projection=perspective(48.0F*kPi/180.0F,static_cast<float>(width_)/height_,.1F,240.0F);
    const Mat4 vp=multiply(projection,view);
    const Mat4 world=identity();

    glUseProgram(program_);
    glUniform3f(cameraLocation_,eye.x,eye.y,eye.z);
    glUniform3f(lightLocation_,-.35F,-1.0F,-.25F);
    glUniformMatrix4fv(modelLocation_,1,GL_FALSE,world.v.data());
    glUniformMatrix4fv(mvpLocation_,1,GL_FALSE,vp.v.data());
    glBindVertexArray(environmentVao_);
    glDrawArrays(GL_TRIANGLES,0,static_cast<GLsizei>(scene_.environmentVertices().size()));

    glBindVertexArray(playerVao_);
    const auto& positions=scene_.playerPositions();
    const GLsizei playerCount=static_cast<GLsizei>(scene_.playerVertices().size());
    for(std::size_t i=0;i<positions.size();++i) {
        const float bob=std::sin(seconds*1.7F+static_cast<float>(i)*.61F)*.018F;
        const float yaw=positions[i].yaw+std::sin(seconds*.45F+static_cast<float>(i))*.025F;
        const Mat4 model=modelAt(positions[i].x,bob,positions[i].z,yaw);
        const Mat4 mvp=multiply(vp,model);
        glUniformMatrix4fv(modelLocation_,1,GL_FALSE,model.v.data());
        glUniformMatrix4fv(mvpLocation_,1,GL_FALSE,mvp.v.data());
        glDrawArrays(GL_TRIANGLES,0,playerCount);
    }
    glBindVertexArray(0); glUseProgram(0);
}

void Renderer::setFormation(int formation) { scene_.setFormation(formation); }
void Renderer::setCinematicCamera(bool enabled) { cinematicCamera_=enabled; }

void Renderer::destroy() {
    if(environmentBuffer_) { glDeleteBuffers(1,&environmentBuffer_); environmentBuffer_=0; }
    if(environmentVao_) { glDeleteVertexArrays(1,&environmentVao_); environmentVao_=0; }
    if(playerBuffer_) { glDeleteBuffers(1,&playerBuffer_); playerBuffer_=0; }
    if(playerVao_) { glDeleteVertexArrays(1,&playerVao_); playerVao_=0; }
    if(program_) { glDeleteProgram(program_); program_=0; }
    initialized_=false;
}

} // namespace football