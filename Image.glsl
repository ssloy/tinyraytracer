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

vec3 cast_ray(in Ray ray) {
    if (abs(ray.dir.z)>1e-5) {
        float dist = (0. - ray.origin.z) / ray.dir.z;
        if (dist > 0.) {
            vec3 point = ray.origin + ray.dir*dist;
            if (point.x>-.25 && point.x<.25 &&
                    point.y>-.25 && point.y<.25) {
                return vec3(0.2, 0.7, 0.8);
            }
        }
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

    vec2 coord = fragCoord/iResolution.xy*vec2(JFIGW, JFIGH);
    if (jfig(uint(coord.x), uint(coord.y))) {
        col += vec3(.5);
    }

    fragColor = vec4(col, 1.);
}
