#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "Scene.h"
#include "RayTrace.h"
#include "Geometry.h"


// Clamp c's entries between low and high. 
static void clamp(Color* c, float low, float high) {
    c->_red = fminf(fmaxf(c->_red, low), high);
    c->_green = fminf(fmaxf(c->_green, low), high);
    c->_blue = fminf(fmaxf(c->_blue, low), high);
}


// Complete
// Given a ray (origin, direction), check if it intersects a given
// sphere.
// Return 1 if there is an intersection, 0 otherwise.
// *t contains the distance to the closest intersection point, if any.
static int
hitSphere(Vector3 origin, Vector3 direction, Sphere sphere, float* t)
{
    return 0;
}


// Check if the ray defined by (scene._camera, direction) is intersecting
// any of the spheres defined in the scene.
// Return 0 if there is no intersection, and 1 otherwise.
//
// If there is an intersection:
// - the position of the intersection with the closest sphere is computed 
// in hit_pos
// - the normal to the surface at the intersection point in hit_normal
// - the diffuse color and specular color of the intersected sphere
// in hit_color and hit_spec
static int
hitScene(Vector3 origin, Vector3 direction, Scene scene,
    Vector3* hit_pos, Vector3* hit_normal,
    Color* hit_color, Color* hit_spec)
{
    Vector3 o = origin;
    Vector3 d = direction;

    float t_min = FLT_MAX;
    int hit_idx = -1;
    Sphere hit_sph;

    // For each sphere in the scene
    int i;
    for (i = 0; i < scene._number_spheres; ++i) {
        Sphere curr = scene._spheres[i];
        float t = 0.0f;
        if (hitSphere(o, d, curr, &t)) {
            if (t < t_min) {
                hit_idx = i;
                t_min = t;
                hit_sph = curr;
            }
        }
    }

    if (hit_idx == -1) return 0;

    Vector3 td;
    mulAV(t_min, d, &td);
    add(o, td, hit_pos);

    Vector3 n;
    sub(*hit_pos, hit_sph._center, &n);
    mulAV(1.0f / hit_sph._radius, n, hit_normal);

    // Save the color of the intersected sphere in hit_color and hit_spec
    *hit_color = hit_sph._color;
    *hit_spec = hit_sph._color_spec;

    return 1;
}


// Save the image in a raw buffer (texture)
// The memory for texture is allocated in this function. It needs to 
// be freed in the caller.
static void saveRaw(Color** image, int width, int height, GLubyte** texture) {
    int count = 0;
    int i;
    int j;
    *texture = (GLubyte*)malloc(sizeof(GLubyte) * 3 * width * height);

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            unsigned char red = (unsigned char)(image[i][j]._red * 255.0f);
            unsigned char green = (unsigned char)(image[i][j]._green * 255.0f);
            unsigned char blue = (unsigned char)(image[i][j]._blue * 255.0f);

            (*texture)[count] = red;
            count++;

            (*texture)[count] = green;
            count++;

            (*texture)[count] = blue;
            count++;
        }
    }
}


// Complete
// Given an intersection point (hit_pos),
// the normal to the surface at the intersection point (hit_normal),
// and the color (diffuse and specular) terms at the intersection point,
// compute the colot intensity at the point by applying the Phong
// shading model.
// Return the color intensity in *color.
static void
shade(Vector3 hit_pos, Vector3 hit_normal,
    Color hit_color, Color hit_spec, Scene scene, Color* color)
{
    // Complete
    // ambient component


     // for each light in the scene
    int l;
    for (l = 0; l < scene._number_lights; l++) {
        // Complete
        // Form a shadow ray and check if the hit point is under
        // direct illumination from the light source


        // Complete
        // diffuse component


        // Complete
        // specular component

    }
}


static void rayTrace(Vector3 origin, Vector3 direction_normalized,
    Scene scene, Color* color)
{
    Vector3 hit_pos;
    Vector3 hit_normal;
    Color hit_color;
    Color hit_spec;
    int hit;

    // does the ray intersect an object in the scene?
    hit =
        hitScene(origin, direction_normalized, scene,
            &hit_pos, &hit_normal, &hit_color,
            &hit_spec);

    // no hit
    if (!hit) {
        color->_red = scene._background_color._red;
        color->_green = scene._background_color._green;
        color->_blue = scene._background_color._blue;
        return;
    }

    // otherwise, apply the shading model at the intersection point
    shade(hit_pos, hit_normal, hit_color, hit_spec, scene, color);
}


void rayTraceScene(Scene scene, int width, int height, GLubyte** texture) {
    Color** image;
    int i;
    int j;

    Vector3 camera_pos;
    float screen_scale;

    image = (Color**)malloc(height * sizeof(Color*));
    for (i = 0; i < height; i++) {
        image[i] = (Color*)malloc(width * sizeof(Color));
    }

    // get parameters for the camera position and the screen fov
    camera_pos._x = scene._camera._x;
    camera_pos._y = scene._camera._y;
    camera_pos._z = scene._camera._z;

    screen_scale = scene._scale;

    // go through each pixel
    // and check for intersection between the ray and the scene
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            // Compute (x,y) coordinates for the current pixel 
            // in scene space
            float x = screen_scale * j - 0.5f * screen_scale * width;
            float y = screen_scale * i - 0.5f * screen_scale * height;

            // Form the vector camera to current pixel
            Vector3 direction;
            Vector3 direction_normalized;

            direction._x = x - camera_pos._x;
            direction._y = y - camera_pos._y;
            direction._z = -camera_pos._z;

            normalize(direction, &direction_normalized);

            Vector3 origin = scene._camera;
            Color color;
            color._red = 0.f;
            color._green = 0.f;
            color._blue = 0.f;
            rayTrace(origin, direction_normalized, scene, &color);

            // Gamma 
            color._red = color._red * 1.1f - 0.02f;
            color._green = color._green * 1.1f - 0.02f;
            color._blue = color._blue * 1.1f - 0.02f;
            clamp(&color, 0.f, 1.f);
            color._red = powf(color._red, 0.4545f);
            color._green = powf(color._green, 0.4545f);
            color._blue = powf(color._blue, 0.4545f);

            // Contrast 
            color._red = color._red * color._red * (3.f - 2.f*color._red);
            color._green = color._green * color._green * (3.f - 2.f*color._green);
            color._blue = color._blue * color._blue * (3.f - 2.f*color._blue);

            image[i][j] = color;
        }
    }

    // save image to texture buffer
    saveRaw(image, width, height, texture);

    for (i = 0; i < height; i++) {
        free(image[i]);
    }

    free(image);
}
