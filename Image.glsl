void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 uv = fragCoord/iResolution.xy;
    vec3 col = vec3(uv.x, uv.y, 0.);
    fragColor = vec4(col, 1.);
}
