#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D texture;
uniform float hue;
uniform float saturation;

vec3 hsl2rgb(vec3 hsl) {
    float hue = hsl.x;
    float sat = hsl.y;
    float lum = hsl.z;

    float C = (1.0 - abs(2.0 * lum - 1.0)) * sat;
    float X = C * (1.0 - abs(mod(hue / 60.0, 2.0) - 1.0));
    float m = lum - 0.5 * C;

    vec3 rgb;

    if (hue < 60.0) {
        rgb = vec3(C, X, 0.0);
    } else if (hue < 120.0) {
        rgb = vec3(X, C, 0.0);
    } else if (hue < 180.0) {
        rgb = vec3(0.0, C, X);
    } else if (hue < 240.0) {
        rgb = vec3(0.0, X, C);
    } else if (hue < 300.0) {
        rgb = vec3(X, 0.0, C);
    } else {
        rgb = vec3(C, 0.0, X);
    }

    return rgb + vec3(m);
}

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    if (color.a != 1.0) {
        gl_FragColor = color * gl_Color;
        return;
    }
    float Lightness = (color.r + color.g + color.b) / 3.0;
    vec3 hsl = vec3(hue, saturation, Lightness);
    gl_FragColor = vec4(hsl2rgb(hsl), gl_Color.a);
}