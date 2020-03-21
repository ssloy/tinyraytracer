#define JFIGW 32u
#define JFIGH 18u
uint[] jfig_bitfield = uint[]( 0x0u,0x0u,0x0u,0xf97800u,0x90900u,0xc91800u,0x890900u,0xf90900u,0x180u,0x0u,0x30e30e0u,0x4904900u,0x49e49e0u,0x4824820u,0x31e31e0u,0x0u,0x0u,0x0u );
bool jfig(in uint x, in uint y) {
    uint id = x + (JFIGH-1u-y)*JFIGW;
    if (id>=JFIGW*JFIGH) return false;
    return 0u != (jfig_bitfield[id/32u] & (1u << (id&31u)));
}

struct Ray {
    vec3 origin;
    vec3 dir;
};

struct Box {
    vec3 center;
    vec3 halfsize;
};

struct Light {
    vec3 position;
    vec3 color;
};

Light[] lights = Light[]( Light(vec3(-15,10,10), vec3(1,1,1)) );

bool box_ray_intersect(in Box box, in Ray ray, out vec3 point, out vec3 normal) {
    for (int d=0; d<3; d++) {
        if (abs(ray.dir[d])<1e-5) continue;
        float side = (ray.dir[d] > 0. ? -1.0 : 1.0);
        float dist = (box.center[d] + side*box.halfsize[d] - ray.origin[d]) / ray.dir[d];
        if (dist < 0.) continue;
        point = ray.origin + ray.dir*dist;
        int i1 = (d+1)%3;
        int i2 = (d+2)%3;
        if (point[i1]>box.center[i1]-box.halfsize[i1] && point[i1]<box.center[i1]+box.halfsize[i1] &&
            point[i2]>box.center[i2]-box.halfsize[i2] && point[i2]<box.center[i2]+box.halfsize[i2]) {
            normal = vec3(0);
            normal[d] = side;
            return true;
        }
    }
    return false;
}

vec3 cast_ray(in Ray ray) {
    vec3 p, n;
    if (box_ray_intersect(Box(vec3(0.), vec3(.25)), ray, p, n)) {
        vec3 diffuse_light = vec3(0.);
        for (int i=0; i<lights.length(); i++) {
            vec3 light_dir = normalize(lights[i].position - p);
            diffuse_light  += lights[i].color * max(0., dot(light_dir, n));
        }
        return vec3(0.2, 0.7, 0.8)*(vec3(.7,.7,.7) + diffuse_light);
    }
    return texture(iChannel0, ray.dir).xyz;
}

vec3 rotateCamera(in vec3 orig, in vec3 dir, in vec3 target) {
    vec3 zAxis = normalize(orig - target);
    vec3 xAxis = normalize(cross(vec3(0., 1., 0.), zAxis));
    vec3 yAxis = normalize(cross(zAxis, xAxis));
    mat4 transform = mat4(vec4(xAxis, 0.), vec4(yAxis, 0.), vec4(zAxis, 0.), vec4(orig, 1.));
    return (transform * vec4(dir, 0.)).xyz;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    const float fov = 3.1416 / 4.;
    vec2 uv = (fragCoord/iResolution.xy*2. - 1.)*tan(fov/2.);
    uv.x *= iResolution.x/iResolution.y;

    vec3 orig = vec3(-sin(iTime/4.), 0., cos(iTime/4.));
    vec3 dir = normalize(vec3(uv, -1));
    dir = rotateCamera(orig, dir, vec3(0.));

    vec3 col = cast_ray(Ray(orig, dir));
    float m = max(col.x, max(col.y, col.z));
    if(m>1.) col = col / m;

    vec2 coord = fragCoord/iResolution.xy*vec2(JFIGW, JFIGH);
    if (jfig(uint(coord.x), uint(coord.y))) {
        col += vec3(.5);
    }

    fragColor = vec4(col, 1.);
}
