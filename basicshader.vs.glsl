in vec3 vp;

void main () {
  gl_PointSize = 20.0f;
  gl_Position = vec4 (vp.x,vp.y,vp.z, 1.0);
}
