
#include "glutils.h"

#include <lunasvg.h>

#include "SOIL/soil.h"
#include "map.h"
#include "shader.h"
#include "strings.h"

struct texture_drawer_data {
  HashMap<GLuint, string> textures;

  GLuint quad_VertexArrayID;
  GLuint quad_vertexbuffer;
  GLuint texID;
  ogl::shader shader;

  const GLfloat g_quad_vertex_buffer_data[18] = {
      -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
  };

  texture_drawer_data() : shader("Texture", NULL, "Texture") {
    // The fullscreen quad's FBO
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data),
                 g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    texID = shader.getu("renderedTexture");
  }

  ~texture_drawer_data() {
    glDeleteBuffers(1, &quad_vertexbuffer);
    glDeleteVertexArrays(1, &quad_VertexArrayID);

    for (auto tex : textures) {
      glDeleteTextures(1, &tex->val);
    }
  }
};

texture_drawer_data* texdd = NULL;

void init_utils() {
  if (!texdd) texdd = new texture_drawer_data();
}

void finalize_utils() {
  if (texdd) delete texdd;
}

void draw_texture(GLuint out, GLuint in) {
  assert(in);

  // Render to the screen
  glBindFramebuffer(GL_FRAMEBUFFER, out ? out : 0);

  // Use our shader
  texdd->shader.bind();

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, in);
  // Set our "renderedTexture" sampler to use Texture Unit 0
  glUniform1i(texdd->texID, 0);

  // glUniformMatrix4fv(texdd->rect_mat, 1, GL_FALSE, &tmat[0][0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, texdd->quad_vertexbuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Draw the triangles. 2*3 indices starting at 0 -> 2 triangles
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(0);

  texdd->shader.unbind();
}

GLuint load_texture(string name) {
  GLuint tex_2d = 0;

  if (0) {
    auto document = lunasvg::Document::loadFromFile("tiger.svg");
    auto bitmap = document->renderToBitmap();
  } else {
    tex_2d = SOIL_load_OGL_texture(
        name.cstr(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT);

    if (0 == tex_2d) {
      printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }
  }

  return tex_2d;
}

GLuint get_tex(const char* TexId) {
  GLuint out = 0;
  alni idx = texdd->textures.Presents(TexId);
  if (idx != -1) {
    out = texdd->textures.Get(TexId);
  } else {
    out = load_texture(TexId);
    texdd->textures.Put(TexId, out);
  }
  return out;
}

void drawCurcle(vec2f pos, double radius, vec4f col) {
  static alni precision = 40;

  glColor4f(col.x, col.y, col.z, col.w);

  double twicePi = 2.0 * 3.142;

  glBegin(GL_TRIANGLE_FAN);  // BEGIN CIRCLE
  glVertex2f(pos.x, pos.y);  // center of circle

  for (alni i = 0; i <= precision; i++) {
    glVertex2f((GLfloat)(pos.x + (radius * cos(i * twicePi / precision))),
               (GLfloat)(pos.y + (radius * sin(i * twicePi / precision))));
  }
  glEnd();  // END
}

void fpscount::update(bool log) {
  frames++;
  if (time.timeout()) {
    fps = frames;
    if (log) {
      printf("fps %i \n", fps);
    }
    frames = 0;
    time.reset();
  }
}

void CamProjectMat(float out[4][4], float fovy, float aspect, float zNear, float zFar) {
  float const tanHalfFovy = (halnf) trigs::tan(fovy / 2.f);
  out[0][0] = 1 / (aspect * tanHalfFovy);
  out[1][1] = 1 / (tanHalfFovy);
  out[2][2] = -(zFar + zNear) / (zFar - zNear);
  out[2][3] = -1;
  out[3][2] = -(2 * zFar * zNear) / (zFar - zNear);
}

void CamViewMat(float out[4][4], vec3f const& eye, vec3f const& center, vec3f const& up) {
  vec3f const f((center - eye).Dir());
  vec3f const s((f.Cross(up)).Dir());
  vec3f const u(s.Cross(f));

  out[0][0] = s.x;
  out[1][0] = s.y;
  out[2][0] = s.z;
  out[0][1] = u.x;
  out[1][1] = u.y;
  out[2][1] = u.z;
  out[0][2] = -f.x;
  out[1][2] = -f.y;
  out[2][2] = -f.z;
  out[3][0] = -s.Dot(eye);
  out[3][1] = -u.Dot(eye);
  out[3][2] = f.Dot(eye);
  out[3][3] = 1;
}
