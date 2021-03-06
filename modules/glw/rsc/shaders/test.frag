
#version 330 core


layout(location = 0) out vec4 color;

#define resolution vec2(500.0, 500.0)
#define Thickness 0.1

float drawLine(vec2 p1, vec2 p2) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;

  float a = abs(distance(p1, uv));
  float b = abs(distance(p2, uv));
  float c = abs(distance(p1, p2));

  if (a < Thickness || b < Thickness || h < Thickness) {
    return 0.5f;
  } 

  return 0.f;

  if ( a >= c || b >=  c ) return 0.0;

  float p = (a + b + c) * 0.5;

  // median to (p1, p2) vector
  float h = 2 / c * sqrt( p * ( p - a) * ( p - b) * ( p - c));



  return mix(1.0, 0.0, smoothstep(Thickness, Thickness, h));
}

void main() {
  
  color = vec4(
      max(
        max(
          drawLine(vec2(0.1, 0.1), vec2(0.1, 0.9)),
          drawLine(vec2(0.1, 0.9), vec2(0.7, 0.5))),
        drawLine(vec2(0.1, 0.1), vec2(0.7, 0.5))));
  
  //color.xyz = vec3(1);
  color.w = 1;
}