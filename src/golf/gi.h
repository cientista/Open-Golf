#ifndef _GOLF_LIGHTMAP_H
#define _GOLF_LIGHTMAP_H

#include "mattiasgustavsson_libs/thread.h"
#include "vec/vec.h"
#include "golf/level.h"
#include "golf/maths.h"

typedef struct golf_gi_entity {
    mat4 model_mat;
    vec_vec3_t positions, normals;
    vec_vec2_t lightmap_uvs;
    int resolution, image_width, image_height;
    float *image_data;
    golf_lightmap_t *lightmap;
    int gl_position_vbo, gl_lightmap_uv_vbo, gl_tex;
} golf_gi_entity_t;
typedef vec_t(golf_gi_entity_t) vec_golf_gi_entity_t;

typedef struct golf_gi {
    bool reset_lightmaps, create_uvs;

    float gamma, z_near, z_far, interpolation_threshold, camera_to_surface_distance_modifier;
    int num_iterations, num_dilates, num_smooths, hemisphere_size, interpolation_passes;
    vec_golf_gi_entity_t entities;

    thread_ptr_t thread;
    thread_mutex_t lock;
    bool is_running;
    int uv_gen_progress, lm_gen_progress, lm_gen_progress_pct;
} golf_gi_t;

void golf_gi_init(golf_gi_t *generator,
        bool reset_lightmaps, bool create_uvs, float gamma, 
        int num_iterations, int num_dilates, int num_smooths,
        int hemisphere_size, float z_near, float z_far,
        int interpolation_passes, float interpolation_threshold,
        float camera_to_surface_distance_modifier);
void golf_gi_deinit(golf_gi_t *generator);
void golf_gi_add_entity(golf_gi_t *generator, golf_model_t *model, mat4 model_mat, golf_lightmap_t *lightmap);
int golf_gi_get_lm_gen_progress(golf_gi_t *generator);
int golf_gi_get_uv_gen_progress(golf_gi_t *generator);
bool golf_gi_is_running(golf_gi_t *generator);
void golf_gi_start(golf_gi_t *generator);

#endif