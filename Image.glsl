#define JFIGW 32u
#define JFIGH 18u
uint[] jfig_bitfield = uint[]( 0x0u,0x0u,0x0u,0xf97800u,0x90900u,0xc91800u,0x890900u,0xf90900u,0x180u,0x0u,0x30e30e0u,0x4904900u,0x49e49e0u,0x4824820u,0x31e31e0u,0x0u,0x0u,0x0u );
bool jfig(in uint x, in uint y) {
    uint id = x + (JFIGH-1u-y)*JFIGW;
    if (id>=JFIGW*JFIGH) return false;
    return 0u != (jfig_bitfield[id/32u] & (1u << (id&31u)));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 uv = fragCoord/iResolution.xy;
    vec3 col = vec3(uv.x, uv.y, 0.);
    

    vec2 coord = fragCoord/iResolution.xy*vec2(JFIGW, JFIGH);
    if (jfig(uint(coord.x), uint(coord.y))) {
        col += vec3(.5);
    }
    
    fragColor = vec4(col, 1.);
}
