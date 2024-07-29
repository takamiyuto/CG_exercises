varying vec4 oPosition;
varying vec4 oNormal;
varying vec2 bPosition;
varying vec4 FragColor;


void main()
{
    vec3 brickColor = vec3(1.0, 0.3, 0.2);
    vec3 mortarColor = vec3(0.85, 0.86, 0.84);
    vec2 brickSize = vec2(0.3, 0.15);
    vec2 brickPct = vec2(0.90, 0.85);
    vec3 color;
    vec2 position;
    vec2 useBrick;

    position = bPosition / brickSize;
    if (fract(position.y * 0.5) > 0.5) {
        position.x += 0.5;
    }
    position = fract(position);
    useBrick = step(position, brickPct);
    color = mix(mortarColor, brickColor, useBrick.x * useBrick.y);
    gl_FragColor = vec4(color, 1.0);

    // light position in eye space
  vec4 light2 = vec4(10.0, 5.0, 5.0, 1.0);

  // vertex in eye space
  vec4 V = oPosition;

  // vertex to light direction
  vec4 L = normalize(light2 - V);

  // normal in eye space
  vec4 N = oNormal;
// material
  vec4 amb = vec4(0.1, 0.1, 0.1, 1.0);
  vec4 diff = vec4(color, 1.0);
  vec4 spec = vec4(1.0, 1.0, 1.0, 1.0);
  float shiny = 32.0;
  
  // light color
  vec4 lcol = vec4(1.0, 1.0, 1.0, 1.0);

  // Phong shading model
  //
  // reflected light vector:
  vec4 R = reflect(-L, N);
  vec4 View = normalize(-V);

  // lighting
  float ndl = clamp(dot(N, L), 0.0, 1.0);
  float vdr = pow(clamp(dot(View, R), 0.0, 1.0), shiny);

  vec4 lin = vec4(0.0);
  lin += amb * lcol;
  lin += ndl * diff * lcol;
  lin += vdr * spec * lcol;

  gl_FragColor = lin;
    clamp(gl_FragColor, 0.0, 1.0);
}