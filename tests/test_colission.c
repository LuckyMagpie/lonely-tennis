#include <stdbool.h>

#include <check.h>
#include <cglm/cglm.h>

#include "utils/vector.h"
#include "engine/colission.h"

bool array_eq(float* arr1, float* arr2, int size)
{
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

void make_flat_square(dumb_opengl_vector_t* vertices)
{
    vertices->items[0] = 1.0f;
    vertices->items[1] = -1.0f;
    vertices->items[2] = 0.0f;
    vertices->items[3] = -1.0f;
    vertices->items[4] = -1.0f;
    vertices->items[5] = 0.0f;
    vertices->items[6] = -1.0f;
    vertices->items[7] = 1.0f;
    vertices->items[8] = 0.0f;
    vertices->items[9] = 1.0f;
    vertices->items[10] = 1.0f;
    vertices->items[11] = 0.0f;
}

bounding_volume_t* make_bounding_sphere(vec3 scale, vec3 translate, vec3 rotate_axis, float rotate_angle)
{
    dumb_opengl_vector_t* vertices = dumb_opengl_vector_init(12);
    mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;

    make_flat_square(vertices);
    glm_translate(model_matrix, translate);
    glm_rotate(model_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_scale(model_matrix, scale);

    bounding_volume_t* bounding_volume = colission_init_bounding_sphere(vertices, model_matrix, scale);

    free(vertices);
    return bounding_volume;

}

bounding_volume_t* make_bounding_obb(vec3 scale, vec3 translate, vec3 rotate_axis, float rotate_angle)
{
    dumb_opengl_vector_t* vertices = dumb_opengl_vector_init(12);
    mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;

    make_flat_square(vertices);
    glm_translate(model_matrix, translate);
    glm_rotate(model_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_scale(model_matrix, scale);

    bounding_volume_t* bounding_volume = colission_init_bounding_obb(vertices, model_matrix, scale, rotate_axis, rotate_angle);

    free(vertices);
    return bounding_volume;
}

START_TEST(test_colission_init_bounding_obb)
{
    vec3 scale = { 2.0f, 2.0f, 2.0f };
    vec3 translate = { 2.0f, 2.0f, 2.0f };
    vec3 rotate_axis = { 0.0f, 1.0f, 0.0f};
    float rotate_angle = 20.0f;

    bounding_volume_t* bounding_volume = make_bounding_obb(scale, translate, rotate_axis, rotate_angle);
    bounding_obb_t* obb = bounding_volume->data;

    vec3 center = { 2.0f, 2.0f, 2.0f };
    vec3 extents = { 2.0f, 2.0f, 0.0f };
    mat4 rotation_matrix = GLM_MAT4_IDENTITY_INIT;
    mat3 mat3_rotation_matrix;
    glm_rotate(rotation_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_mat4_pick3(rotation_matrix, mat3_rotation_matrix);

    ck_assert_msg(bounding_volume != NULL, "Failed to init bounding_volume");
    ck_assert_msg(bounding_volume->data != NULL, "Failed to init bounding_obb");
    ck_assert_msg(bounding_volume->kind == OBB , "Bounding volume should have OBB kind");
    ck_assert_msg(array_eq(obb->center, center, 3), "Bounding obb should have 2, 2, 2 center");
    ck_assert_msg(array_eq(obb->extents, extents, 3), "Bounding obb should have 2, 2, 0 extents");
    ck_assert_msg(array_eq(obb->rotation_matrix[0], mat3_rotation_matrix[0], 9), "Bounding obb has wrong rotation_matrix");

    colission_free_bounding_volume(bounding_volume);
}
END_TEST

START_TEST(test_colission_init_bounding_sphere)
{
    vec3 scale = { 2.0f, 2.0f, 2.0f };
    vec3 translate = { 2.0f, 2.0f, 2.0f };
    vec3 rotate_axis = { 0.0f, 1.0f, 0.0f};
    float rotate_angle = 20.0f;

    bounding_volume_t* bounding_volume = make_bounding_sphere(scale, translate, rotate_axis, rotate_angle);
    bounding_sphere_t* sphere = bounding_volume->data;

    vec3 center = { 2.0f, 2.0f, 2.0f };
    vec3 local_center = { 0.0f, 0.0f, 0.0f };
    float radius = 2.0f;

    ck_assert_msg(bounding_volume != NULL, "Failed to init bounding_volume");
    ck_assert_msg(bounding_volume->data != NULL, "Failed to init bounding_sphere");
    ck_assert_msg(bounding_volume->kind == SPHERE , "Bounding volume should have SPHERE kind");
    ck_assert_msg(array_eq(sphere->center, center, 3), "Bounding sphere should have 2, 2, 2 center");
    ck_assert_msg(array_eq(sphere->local_center, local_center, 3), "Bounding sphere should have 0, 0, 0 local center");
    ck_assert_msg(sphere->radius == radius, "Bounding sphere should have 2.0 radius got %f", sphere->radius);

    colission_free_bounding_volume(bounding_volume);
}
END_TEST

START_TEST(test_colission_update_bounding_sphere_center)
{
    vec3 scale = { 2.0f, 2.0f, 2.0f };
    vec3 translate = { 2.0f, 2.0f, 2.0f };
    vec3 rotate_axis = { 0.0f, 1.0f, 0.0f};
    float rotate_angle = 20.0f;

    bounding_volume_t* bounding_volume = make_bounding_sphere(scale, translate, rotate_axis, rotate_angle);
    bounding_sphere_t* sphere = bounding_volume->data;

    vec3 center = { 2.0f, 2.0f, 2.0f };
    vec3 new_center = { 3.0f, 3.0f, 3.0f };

    ck_assert_msg(array_eq(sphere->center, center, 3), "Bounding sphere should have 2, 2, 2 center");

    mat4 new_model_matrix = GLM_MAT4_IDENTITY_INIT;
    vec3 new_translate = { 3.0f, 3.0f, 3.0f };
    glm_translate(new_model_matrix, new_translate);
    glm_rotate(new_model_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_scale(new_model_matrix, scale);

    colission_update_bounding_sphere_center(sphere, new_model_matrix);

    ck_assert_msg(array_eq(sphere->center, new_center, 3), "Bounding sphere should have 3, 3, 3 center");

    colission_free_bounding_volume(bounding_volume);
}
END_TEST

START_TEST(test_colission_test_intersection_sphere_sphere_miss)
{
    vec3 sphere1_scale = { 1.0f, 1.0f, 1.0f };
    vec3 sphere1_translate = { 0.0f, 0.0f, 0.0f };
    vec3 sphere1_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float sphere1_rotate_angle = 0.0f;

    vec3 sphere2_scale = { 1.0f, 1.0f, 1.0f };
    vec3 sphere2_translate = { 10.0f, 10.0f, 10.0f };
    vec3 sphere2_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float sphere2_rotate_angle = 0.0f;

    bounding_volume_t* bounding_volume1 = make_bounding_sphere(sphere1_scale, sphere1_translate, sphere1_rotate_axis, sphere1_rotate_angle);
    bounding_volume_t* bounding_volume2 = make_bounding_sphere(sphere2_scale, sphere2_translate, sphere2_rotate_axis, sphere2_rotate_angle);

    ck_assert_msg(colission_test_intersection_sphere_sphere(bounding_volume1->data, bounding_volume2->data) == false, "Spheres shouldn't collide");

    colission_free_bounding_volume(bounding_volume1);
    colission_free_bounding_volume(bounding_volume2);
}
END_TEST

START_TEST(test_colission_test_intersection_sphere_sphere_collide)
{
    vec3 sphere1_scale = { 1.0f, 1.0f, 1.0f };
    vec3 sphere1_translate = { 0.0f, 0.0f, 0.0f };
    vec3 sphere1_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float sphere1_rotate_angle = 0.0f;

    vec3 sphere2_scale = { 100.0f, 100.0f, 100.0f };
    vec3 sphere2_translate = { 10.0f, 10.0f, 10.0f };
    vec3 sphere2_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float sphere2_rotate_angle = 0.0f;

    bounding_volume_t* bounding_volume1 = make_bounding_sphere(sphere1_scale, sphere1_translate, sphere1_rotate_axis, sphere1_rotate_angle);
    bounding_volume_t* bounding_volume2 = make_bounding_sphere(sphere2_scale, sphere2_translate, sphere2_rotate_axis, sphere2_rotate_angle);

    ck_assert_msg(colission_test_intersection_sphere_sphere(bounding_volume1->data, bounding_volume2->data) == true, "Spheres should collide");

    colission_free_bounding_volume(bounding_volume1);
    colission_free_bounding_volume(bounding_volume2);
}
END_TEST

START_TEST(test_colission_test_intersection_sphere_obb_miss)
{
    vec3 sphere_scale = { 1.0f, 1.0f, 1.0f };
    vec3 sphere_translate = { 0.0f, 0.0f, 0.0f };
    vec3 sphere_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float sphere_rotate_angle = 0.0f;

    vec3 obb_scale = { 1.0f, 1.0f, 1.0f };
    vec3 obb_translate = { 10.0f, 10.0f, 10.0f };
    vec3 obb_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float obb_rotate_angle = 0.0f;

    bounding_volume_t* bounding_volume1 = make_bounding_sphere(sphere_scale, sphere_translate, sphere_rotate_axis, sphere_rotate_angle);
    bounding_volume_t* bounding_volume2 = make_bounding_obb(obb_scale, obb_translate, obb_rotate_axis, obb_rotate_angle);

    ck_assert_msg(colission_test_intersection_sphere_obb(bounding_volume1->data, bounding_volume2->data) == false, "Sphere and obb shouldn't collide");

    colission_free_bounding_volume(bounding_volume1);
    colission_free_bounding_volume(bounding_volume2);
}
END_TEST

START_TEST(test_colission_test_intersection_sphere_obb_collide)
{
    vec3 sphere_scale = { 1.0f, 1.0f, 1.0f };
    vec3 sphere_translate = { 0.0f, 0.0f, 0.0f };
    vec3 sphere_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float sphere_rotate_angle = 0.0f;

    vec3 obb_scale = { 100.0f, 100.0f, 100.0f };
    vec3 obb_translate = { 10.0f, 10.0f, 0.0f };
    vec3 obb_rotate_axis = { 0.0f, 0.0f, 0.0f};
    float obb_rotate_angle = 0.0f;

    bounding_volume_t* bounding_volume1 = make_bounding_sphere(sphere_scale, sphere_translate, sphere_rotate_axis, sphere_rotate_angle);
    bounding_volume_t* bounding_volume2 = make_bounding_obb(obb_scale, obb_translate, obb_rotate_axis, obb_rotate_angle);

    ck_assert_msg(colission_test_intersection_sphere_obb(bounding_volume1->data, bounding_volume2->data) == true, "Sphere and obb should collide");

    colission_free_bounding_volume(bounding_volume1);
    colission_free_bounding_volume(bounding_volume2);
}
END_TEST

START_TEST(test_colission_query_closest_point_obb)
{
    vec3 scale = { 1.0f, 1.0f, 1.0f };
    vec3 translate = { 0.0f, 0.0f, 0.0f };
    vec3 rotate_axis = { 0.0f, 0.0f, 0.0f};
    float rotate_angle = 0.0f;
    vec3 point = { 2.0f, -1.0f, 0.0f };
    vec3 closest_point;
    vec3 result = { 1.0f, -1.0f, 0.0f };

    bounding_volume_t* bounding_volume = make_bounding_obb(scale, translate, rotate_axis, rotate_angle);

    colission_query_closest_point_obb(bounding_volume->data, point, closest_point);

    ck_assert_msg(array_eq(closest_point, result, 3), "Closest point should be 1, 0.5, 0");

    colission_free_bounding_volume(bounding_volume);
}
END_TEST

Suite* colission_suite()
{
    Suite* suite = suite_create("colission");

    TCase* tc1 = tcase_create("test_colission_init_bounding_obb");
    tcase_add_test(tc1, test_colission_init_bounding_obb);

    TCase* tc2 = tcase_create("test_colission_init_bounding_sphere");
    tcase_add_test(tc2, test_colission_init_bounding_sphere);

    TCase* tc3 = tcase_create("test_colission_update_bounding_sphere_center");
    tcase_add_test(tc3, test_colission_update_bounding_sphere_center);

    TCase* tc4 = tcase_create("test_colission_test_intersection_sphere");
    tcase_add_test(tc4, test_colission_test_intersection_sphere_sphere_miss);
    tcase_add_test(tc4, test_colission_test_intersection_sphere_sphere_collide);
    tcase_add_test(tc4, test_colission_test_intersection_sphere_obb_miss);
    tcase_add_test(tc4, test_colission_test_intersection_sphere_obb_collide);

    TCase* tc5 = tcase_create("test_colission_query_closest_point_obb");
    tcase_add_test(tc5, test_colission_query_closest_point_obb);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    suite_add_tcase(suite, tc3);
    suite_add_tcase(suite, tc4);
    suite_add_tcase(suite, tc5);
    return suite;
}

int main(void)
{
    Suite* suite = colission_suite();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
