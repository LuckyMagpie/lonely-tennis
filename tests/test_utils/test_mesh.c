#include <check.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "mesh.h"
#include "vector.h"

float v0[3] = { -1.0f, 0.0f, 1.0f };
float v1[3] = { 1.0f, 0.0f, 1.0f };
float v2[3] = { -1.0f, 0.0f, -1.0f };
float v3[3] = { 1.0f, 0.0f, -1.f };
float uv0[2] = { 1.0f, 0.0f };
float uv1[2] = { 0.0f, 1.0f };
float uv2[2] = { 0.0f, 0.0f };
float uv3[2] = { 1.0f, 1.0f };
float n0[3] = { 0.0f, 1.0f, 0.0f };

bool array_eq(float* arr1, float* arr2, int size)
{
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

START_TEST(test_load_obj)
{
    dumb_opengl_vector_t* vertices;
    dumb_opengl_vector_t* uvs;
    dumb_opengl_vector_t* normals;

    bool res = load_obj("mock_assets/test.obj", &vertices, &uvs, &normals);

    ck_assert_msg(res == true, "load obj returned false");

    free(uvs);
    free(vertices);
    free(normals);
}
END_TEST

START_TEST(test_load_obj_order)
{
    dumb_opengl_vector_t* vertices;
    dumb_opengl_vector_t* uvs;
    dumb_opengl_vector_t* normals;

    bool res = load_obj("mock_assets/test.obj", &vertices, &uvs, &normals);

    ck_assert_msg(res == true, "load obj returned false");

    //face1 2/1/1 3/2/1 1/3/1 (Indices start at 1)
    ck_assert_msg(array_eq(&vertices->items[0], v1, 3) == true, "Vertex1/Triangle1 wrong vertex");
    ck_assert_msg(array_eq(&uvs->items[0], uv0, 2) == true, "Vertex1/Triangle1 wrong uv");
    ck_assert_msg(array_eq(&normals->items[0], n0, 3) == true, "Vertex1/Triangle1 wrong normal");

    ck_assert_msg(array_eq(&vertices->items[3], v2, 3) == true, "Vertex2/Triangle1 wrong vertex");
    ck_assert_msg(array_eq(&uvs->items[2], uv1, 2) == true, "Vertex2/Triangle1 wrong uv");
    ck_assert_msg(array_eq(&normals->items[3], n0, 3) == true, "Vertex2/Triangle1 wrong normal");

    ck_assert_msg(array_eq(&vertices->items[6], v0, 3) == true, "Vertex3/Triangle1 wrong vertex");
    ck_assert_msg(array_eq(&uvs->items[4], uv2, 2) == true, "Vertex3/Triangle1 wrong uv");
    ck_assert_msg(array_eq(&normals->items[6], n0, 3) == true, "Vertex3/Triangle1 wrong normal");

    //face2 2/1/1 4/4/1 3/2/1 (Indices start at 1)
    ck_assert_msg(array_eq(&vertices->items[9], v1, 3) == true, "Vertex1/Triangle2 wrong vertex");
    ck_assert_msg(array_eq(&uvs->items[6], uv0, 2) == true, "Vertex1/Triangle2 wrong uv");
    ck_assert_msg(array_eq(&normals->items[9], n0, 3) == true, "Vertex1/Triangle2 wrong normal");

    ck_assert_msg(array_eq(&vertices->items[12], v3, 3) == true, "Vertex2/Triangle2 wrong vertex");
    ck_assert_msg(array_eq(&uvs->items[8], uv3, 2) == true, "Vertex2/Triangle2 wrong uv");
    ck_assert_msg(array_eq(&normals->items[12], n0, 3) == true, "Vertex2/Triangle2 wrong normal");

    ck_assert_msg(array_eq(&vertices->items[15], v2, 3) == true, "Vertex3/Triangle2 wrong vertex");
    ck_assert_msg(array_eq(&uvs->items[10], uv1, 2) == true, "Vertex3/Triangle2 wrong uv");
    ck_assert_msg(array_eq(&normals->items[15], n0, 3) == true, "Vertex3/Triangle2 wrong normal");

    free(uvs);
    free(vertices);
    free(normals);
}
END_TEST

Suite* mesh_suit()
{
    Suite* suite = suite_create("mesh");

    TCase* tc1 = tcase_create("test_load_obj");
    tcase_add_test(tc1, test_load_obj);
    tcase_add_test(tc1, test_load_obj_order);

    suite_add_tcase(suite, tc1);
    return suite;
}

int main(void)
{
    Suite* suite = mesh_suit();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
