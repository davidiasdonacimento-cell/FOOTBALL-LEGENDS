#include "football/lobby/LobbyScene.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace football {
namespace {
constexpr float kPi = 3.14159265358979323846F;
struct Vec3 { float x, y, z; };

Vec3 add(Vec3 a, Vec3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
Vec3 sub(Vec3 a, Vec3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
Vec3 mul(Vec3 a, float s) { return {a.x*s, a.y*s, a.z*s}; }
float dot(Vec3 a, Vec3 b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
Vec3 cross(Vec3 a, Vec3 b) { return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x}; }
Vec3 normalized(Vec3 v) {
    const float length = std::sqrt(std::max(dot(v,v), 0.000001F));
    return mul(v, 1.0F/length);
}
using Color = std::array<float,4>;

void vertex(std::vector<LobbyVertex>& out, Vec3 p, Vec3 n, Color c) {
    LobbyVertex v{};
    v.position[0]=p.x; v.position[1]=p.y; v.position[2]=p.z;
    v.normal[0]=n.x; v.normal[1]=n.y; v.normal[2]=n.z;
    v.color[0]=c[0]; v.color[1]=c[1]; v.color[2]=c[2]; v.color[3]=c[3];
    v.uv[0]=p.x*.075F; v.uv[1]=p.z*.075F;
    v.boneIndices[0]=0.0F; v.boneWeights[0]=1.0F;
    if(c[1]>c[0]*1.35F && c[1]>c[2]*1.2F) v.material=0.0F;
    else if(c[2]>c[0]*1.25F && c[2]>c[1]*1.05F) v.material=1.0F;
    else if(c[0]>c[1]*1.12F && c[1]>c[2]*1.05F) v.material=2.0F;
    else if(c[0]>.72F && c[1]>.72F && c[2]>.72F) v.material=3.0F;
    else v.material=4.0F;
    out.push_back(v);
}
void assignBone(std::vector<LobbyVertex>& out,std::size_t begin,std::size_t end,float bone) {
    for(std::size_t i=begin;i<end;++i) {
        out[i].boneIndices[0]=bone;
        out[i].boneWeights[0]=1.0F;
    }
}
void triangle(std::vector<LobbyVertex>& out, Vec3 a, Vec3 b, Vec3 c, Vec3 normal, Color color) {
    vertex(out,a,normal,color); vertex(out,b,normal,color); vertex(out,c,normal,color);
}
void quad(std::vector<LobbyVertex>& out, Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 normal, Color color) {
    triangle(out,a,b,c,normal,color); triangle(out,a,c,d,normal,color);
}
void box(std::vector<LobbyVertex>& out, Vec3 center, Vec3 size, Color color) {
    const float x0=center.x-size.x*.5F, x1=center.x+size.x*.5F;
    const float y0=center.y-size.y*.5F, y1=center.y+size.y*.5F;
    const float z0=center.z-size.z*.5F, z1=center.z+size.z*.5F;
    quad(out,{x0,y0,z1},{x1,y0,z1},{x1,y1,z1},{x0,y1,z1},{0,0,1},color);
    quad(out,{x1,y0,z0},{x0,y0,z0},{x0,y1,z0},{x1,y1,z0},{0,0,-1},color);
    quad(out,{x1,y0,z1},{x1,y0,z0},{x1,y1,z0},{x1,y1,z1},{1,0,0},color);
    quad(out,{x0,y0,z0},{x0,y0,z1},{x0,y1,z1},{x0,y1,z0},{-1,0,0},color);
    quad(out,{x0,y1,z1},{x1,y1,z1},{x1,y1,z0},{x0,y1,z0},{0,1,0},color);
    quad(out,{x0,y0,z0},{x1,y0,z0},{x1,y0,z1},{x0,y0,z1},{0,-1,0},color);
}
void ellipsoid(std::vector<LobbyVertex>& out, Vec3 center, Vec3 radius, Color color, int slices=16, int stacks=10) {
    for (int y=0;y<stacks;++y) {
        const float a0=-kPi*.5F+kPi*static_cast<float>(y)/stacks;
        const float a1=-kPi*.5F+kPi*static_cast<float>(y+1)/stacks;
        for (int x=0;x<slices;++x) {
            const float b0=2*kPi*static_cast<float>(x)/slices;
            const float b1=2*kPi*static_cast<float>(x+1)/slices;
            auto point=[&](float a,float b) {
                return Vec3{center.x+radius.x*std::cos(a)*std::cos(b),
                            center.y+radius.y*std::sin(a),
                            center.z+radius.z*std::cos(a)*std::sin(b)};
            };
            const Vec3 p00=point(a0,b0), p01=point(a0,b1), p10=point(a1,b0), p11=point(a1,b1);
            auto normal=[&](Vec3 p) {
                return normalized({(p.x-center.x)/(radius.x*radius.x),
                                   (p.y-center.y)/(radius.y*radius.y),
                                   (p.z-center.z)/(radius.z*radius.z)});
            };
            triangle(out,p00,p10,p11,normal(p00),color);
            triangle(out,p00,p11,p01,normal(p00),color);
        }
    }
}
void cylinder(std::vector<LobbyVertex>& out, Vec3 start, Vec3 end, float r0, float r1, Color color, int slices=12) {
    const Vec3 axis=normalized(sub(end,start));
    Vec3 helper=std::fabs(axis.y)<.9F?Vec3{0,1,0}:Vec3{1,0,0};
    const Vec3 u=normalized(cross(axis,helper));
    const Vec3 v=normalized(cross(axis,u));
    for(int i=0;i<slices;++i) {
        const float a0=2*kPi*static_cast<float>(i)/slices;
        const float a1=2*kPi*static_cast<float>(i+1)/slices;
        const Vec3 n0=add(mul(u,std::cos(a0)),mul(v,std::sin(a0)));
        const Vec3 n1=add(mul(u,std::cos(a1)),mul(v,std::sin(a1)));
        const Vec3 p0=add(start,mul(n0,r0)), p1=add(start,mul(n1,r0));
        const Vec3 q0=add(end,mul(n0,r1)), q1=add(end,mul(n1,r1));
        quad(out,p0,p1,q1,q0,normalized(add(n0,n1)),color);
    }
}
void groundRect(std::vector<LobbyVertex>& out,float x0,float z0,float x1,float z1,float y,Color color) {
    quad(out,{x0,y,z0},{x1,y,z0},{x1,y,z1},{x0,y,z1},{0,1,0},color);
}
void lineX(std::vector<LobbyVertex>& out,float x0,float x1,float z,float width,float y,Color color) {
    groundRect(out,x0,z-width*.5F,x1,z+width*.5F,y,color);
}
void lineZ(std::vector<LobbyVertex>& out,float x,float z0,float z1,float width,float y,Color color) {
    groundRect(out,x-width*.5F,z0,x+width*.5F,z1,y,color);
}
void ring(std::vector<LobbyVertex>& out,float cx,float cz,float radius,float width,float y,Color color,int segments=72) {
    const float inner=radius-width*.5F, outer=radius+width*.5F;
    for(int i=0;i<segments;++i) {
        const float a0=2*kPi*static_cast<float>(i)/segments;
        const float a1=2*kPi*static_cast<float>(i+1)/segments;
        quad(out,{cx+std::cos(a0)*inner,y,cz+std::sin(a0)*inner},
                 {cx+std::cos(a0)*outer,y,cz+std::sin(a0)*outer},
                 {cx+std::cos(a1)*outer,y,cz+std::sin(a1)*outer},
                 {cx+std::cos(a1)*inner,y,cz+std::sin(a1)*inner},
                 {0,1,0},color);
    }
}
} // namespace

void LobbyScene::build() {
    environment_.clear(); player_.clear();
    buildEnvironment();
    buildPlayerModel();
    updateFormation();
}
void LobbyScene::setFormation(int formation) {
    formation_=std::clamp(formation,0,3);
    updateFormation();
}
const std::vector<LobbyVertex>& LobbyScene::environmentVertices() const noexcept { return environment_; }
const std::vector<LobbyVertex>& LobbyScene::playerVertices() const noexcept { return player_; }
const std::array<LobbyPlayerPosition,11>& LobbyScene::playerPositions() const noexcept { return positions_; }

void LobbyScene::updateFormation() {
    const std::array<LobbyPlayerPosition,11> f433{{
        {0,30,0},{-25,20,0},{-9,21,0},{9,21,0},{25,20,0},
        {-18,10,0},{0,9,0},{18,10,0},{-22,-2,0},{0,-5,0},{22,-2,0}}};
    const std::array<LobbyPlayerPosition,11> f442{{
        {0,30,0},{-25,20,0},{-9,21,0},{9,21,0},{25,20,0},
        {-25,10,0},{-8,10,0},{8,10,0},{25,10,0},{-10,-4,0},{10,-4,0}}};
    const std::array<LobbyPlayerPosition,11> f352{{
        {0,30,0},{-18,20,0},{0,21,0},{18,20,0},
        {-28,10,0},{-14,8,0},{0,10,0},{14,8,0},{28,10,0},{-10,-4,0},{10,-4,0}}};
    const std::array<LobbyPlayerPosition,11> f4231{{
        {0,30,0},{-25,20,0},{-9,21,0},{9,21,0},{25,20,0},
        {-11,12,0},{11,12,0},{-20,4,0},{0,3,0},{20,4,0},{0,-6,0}}};
    positions_=formation_==1?f442:formation_==2?f352:formation_==3?f4231:f433;
}

void LobbyScene::buildEnvironment() {
    const Color grassA{.075F,.34F,.13F,1}, grassB{.095F,.40F,.16F,1};
    const Color white{.92F,.95F,.91F,1};
    const Color concrete{.18F,.22F,.29F,1};
    const Color seatBlue{.035F,.13F,.34F,1}, seatCyan{.05F,.32F,.43F,1};
    const Color roof{.055F,.085F,.14F,1};
    // Full-size presentation pitch: 110 x 72 metres, slightly broader than a compact mobile pitch.
    for(int i=0;i<11;++i) {
        const float x0=-55.0F+static_cast<float>(i)*10.0F;
        groundRect(environment_,x0,-36,x0+10,36,.0F,(i%2)?grassA:grassB);
    }
    const float y=.035F;
    lineX(environment_,-55,55,-36,.20F,y,white); lineX(environment_,-55,55,36,.20F,y,white);
    lineZ(environment_,-55,-36,36,.20F,y,white); lineZ(environment_,55,-36,36,.20F,y,white);
    lineZ(environment_,0,-36,36,.16F,y,white);
    ring(environment_,0,0,9.15F,.16F,y,white);
    ring(environment_,0,0,.24F,.48F,y,white,24);
    for(float sign:{-1.0F,1.0F}) {
        const float goalZ=sign*36.0F;
        lineX(environment_,-20.16F,20.16F,goalZ-sign*16.5F,.16F,y,white);
        lineZ(environment_,-20.16F,goalZ-sign*16.5F,goalZ,.16F,y,white);
        lineZ(environment_,20.16F,goalZ-sign*16.5F,goalZ,.16F,y,white);
        lineX(environment_,-9.16F,9.16F,goalZ-sign*5.5F,.16F,y,white);
        lineZ(environment_,-9.16F,goalZ-sign*5.5F,goalZ,.16F,y,white);
        lineZ(environment_,9.16F,goalZ-sign*5.5F,goalZ,.16F,y,white);
        ring(environment_,0,goalZ-sign*11.0F, .22F,.42F,y,white,20);
        // Goal frames and restrained net supports.
        const float gz=goalZ+sign*.8F;
        cylinder(environment_,{-3.66F,.0F,gz},{-3.66F,2.44F,gz},.12F,.12F,white,12);
        cylinder(environment_,{3.66F,.0F,gz},{3.66F,2.44F,gz},.12F,.12F,white,12);
        cylinder(environment_,{-3.66F,2.44F,gz},{3.66F,2.44F,gz},.12F,.12F,white,12);
        for(int n=0;n<=8;++n) {
            const float nx=-3.66F+7.32F*static_cast<float>(n)/8.0F;
            cylinder(environment_,{nx,.05F,gz},{nx,2.4F,gz+sign*2.0F},.018F,.018F,{.72F,.82F,.83F,1},6);
        }
    }
    // Stadium bowl, continuous seating tiers and structural fascia.
    box(environment_,{0,1.0F,-42},{132,2.0F,8},concrete);
    box(environment_,{0,1.0F,42},{132,2.0F,8},concrete);
    box(environment_,{-64,1.0F,0},{8,2.0F,84},concrete);
    box(environment_,{64,1.0F,0},{8,2.0F,84},concrete);
    for(int row=0;row<15;++row) {
        const float h=2.3F+static_cast<float>(row)*.48F;
        const float zFront=40.0F+static_cast<float>(row)*1.05F;
        const float zBack=-40.0F-static_cast<float>(row)*1.05F;
        const Color seat=(row%3==0)?seatCyan:seatBlue;
        box(environment_,{0,h,zFront},{128,.42F,1.02F},seat);
        box(environment_,{0,h,zBack},{128,.42F,1.02F},seat);
        const float xSide=62.0F+static_cast<float>(row)*1.05F;
        box(environment_,{xSide,h,0},{1.02F,.42F,80},seat);
        box(environment_,{-xSide,h,0},{1.02F,.42F,80},seat);
    }
    // Upper decks and roof canopies.
    box(environment_,{0,10.4F,48},{140,.8F,13},roof);
    box(environment_,{0,10.4F,-48},{140,.8F,13},roof);
    box(environment_,{70,10.4F,0},{13,.8F,96},roof);
    box(environment_,{-70,10.4F,0},{13,.8F,96},roof);
    for(float x:{-62.0F,62.0F}) for(float z:{-42.0F,42.0F})
        box(environment_,{x,6.0F,z},{1.3F,12.0F,1.3F},concrete);
    // Roof ribs and floodlight pylons.
    for(int i=0;i<9;++i) {
        const float x=-60.0F+static_cast<float>(i)*15.0F;
        box(environment_,{x,11.1F,48},{.35F,1.2F,14},concrete);
        box(environment_,{x,11.1F,-48},{.35F,1.2F,14},concrete);
    }
    const Color metal{.48F,.58F,.68F,1}, lamp{.82F,.91F,1,1};
    for(float x:{-67.0F,67.0F}) for(float z:{-47.0F,47.0F}) {
        cylinder(environment_,{x,0,z},{x,19,z},.22F,.12F,metal,10);
        box(environment_,{x,19.0F,z},{3.6F,.35F,1.2F},lamp);
    }
    // Main stadium display over the far stand.
    box(environment_,{0,14.2F,-52.0F},{18.0F,5.0F,.9F},{.025F,.07F,.12F,1});
    box(environment_,{0,14.2F,-51.48F},{16.8F,3.8F,.08F},{.02F,.38F,.48F,1});
    // Technical area and touchline walkways.
    box(environment_,{0,.12F,38.8F},{112,.18F,2.0F},{.30F,.34F,.39F,1});
    box(environment_,{0,.12F,-38.8F},{112,.18F,2.0F},{.30F,.34F,.39F,1});
}

void LobbyScene::buildPlayerModel() {
    const Color skin{.68F,.43F,.29F,1}, skinLight{.79F,.55F,.39F,1};
    const Color jersey{.025F,.13F,.48F,1}, jerseyLight{.06F,.30F,.78F,1};
    const Color shorts{.86F,.89F,.94F,1}, socks{.035F,.12F,.38F,1};
    const Color boots{.035F,.045F,.065F,1}, hair{.055F,.035F,.025F,1};
    const Color trim{.88F,.91F,.98F,1};
    // Anatomical silhouette built from tapered limbs and smooth ellipsoids, not cubes.
    ellipsoid(player_,{0,1.10F,0},{.235F,.34F,.135F},jersey,20,14);
    ellipsoid(player_,{0,.83F,0},{.225F,.16F,.13F},shorts,18,10);
    ellipsoid(player_,{0,1.405F,0},{.075F,.09F,.075F},skin,12,8);
    ellipsoid(player_,{0,1.57F,-.005F},{.135F,.17F,.125F},skinLight,20,14);
    ellipsoid(player_,{0,1.69F,.0F},{.137F,.065F,.13F},hair,18,9);
    ellipsoid(player_,{-.137F,1.56F,0},{.027F,.055F,.035F},skin,10,7);
    ellipsoid(player_,{.137F,1.56F,0},{.027F,.055F,.035F},skin,10,7);
    // Face details are oriented toward the front (-Z).
    ellipsoid(player_,{-.052F,1.585F,-.116F},{.014F,.018F,.009F},hair,8,6);
    ellipsoid(player_,{.052F,1.585F,-.116F},{.014F,.018F,.009F},hair,8,6);
    ellipsoid(player_,{0,1.535F,-.126F},{.025F,.035F,.018F},skin,10,7);
    // Shirt collar, chest stripe and sleeve cuffs.
    ellipsoid(player_,{0,1.405F,-.002F},{.082F,.025F,.078F},trim,12,7);
    box(player_,{0,1.12F,-.136F},{.025F,.38F,.012F},jerseyLight);
    const std::size_t leftArmBegin=player_.size();
    cylinder(player_,{-.20F,1.31F,0},{-.30F,1.02F,-.005F},.075F,.055F,jersey,14);
    ellipsoid(player_,{-.30F,1.02F,-.005F},{.056F,.065F,.06F},skin,12,8);
    cylinder(player_,{-.30F,1.02F,-.005F},{-.32F,.78F,-.025F},.052F,.043F,skin,12);
    ellipsoid(player_,{-.32F,.77F,-.025F},{.047F,.055F,.05F},skin,10,7);
    assignBone(player_,leftArmBegin,player_.size(),1.0F);

    const std::size_t rightArmBegin=player_.size();
    cylinder(player_,{.20F,1.31F,0},{.30F,1.02F,-.005F},.075F,.055F,jersey,14);
    ellipsoid(player_,{.30F,1.02F,-.005F},{.056F,.065F,.06F},skin,12,8);
    cylinder(player_,{.30F,1.02F,-.005F},{.32F,.78F,-.025F},.052F,.043F,skin,12);
    ellipsoid(player_,{.32F,.77F,-.025F},{.047F,.055F,.05F},skin,10,7);
    assignBone(player_,rightArmBegin,player_.size(),2.0F);

    // Articulated upper and lower legs, socks and boots.
    const std::size_t leftLegBegin=player_.size();
    cylinder(player_,{-.115F,.80F,0},{-.13F,.49F,-.005F},.105F,.075F,shorts,14);
    ellipsoid(player_,{-.13F,.47F,-.005F},{.078F,.075F,.078F},skin,12,8);
    cylinder(player_,{-.13F,.45F,-.005F},{-.135F,.12F,-.015F},.066F,.052F,socks,14);
    ellipsoid(player_,{-.135F,.075F,-.075F},{.085F,.065F,.16F},boots,14,8);
    assignBone(player_,leftLegBegin,player_.size(),3.0F);

    const std::size_t rightLegBegin=player_.size();
    cylinder(player_,{.115F,.80F,0},{.13F,.49F,-.005F},.105F,.075F,shorts,14);
    ellipsoid(player_,{.13F,.47F,-.005F},{.078F,.075F,.078F},skin,12,8);
    cylinder(player_,{.13F,.45F,-.005F},{.135F,.12F,-.015F},.066F,.052F,socks,14);
    ellipsoid(player_,{.135F,.075F,-.075F},{.085F,.065F,.16F},boots,14,8);
    assignBone(player_,rightLegBegin,player_.size(),4.0F);
}

} // namespace football