#include <check.h>
#include <stdbool.h>

#include "vector.h"
#include "mesh.h"

float v0[3] = {-1.0f, 0.0f, 1.0f};
float v1[3] = {1.0f, 0.0f, 1.0f};
float v2[3] = {-1.0f, 0.0f, -1.0f};
float v3[3] = {1.0f, 0.0f, -1.f};
float uv0[2] = {1.0f, 0.0f};
float uv1[2] = {0.0f, 1.0f};
float uv2[2] = {0.0f, 0.0f};
float uv3[2] = {1.0f, 1.0f};
float n0[3] = {0.0f, 1.0f, 0.0f};

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
    vector_t* uvs = vector_init();
    vector_t* vertices = vector_init();
    vector_t* normals = vector_init();

    bool res = load_obj("mock_assets/test.obj", uvs, normals, vertices);

    ck_assert_msg(res == true, "load obj returned false");

    vector_foreach(uvs, &vector_generic_item_free);
    vector_free(uvs);

    vector_foreach(vertices, &vector_generic_item_free);
    vector_free(vertices);

    vector_foreach(normals, &vector_generic_item_free);
    vector_free(normals);
}
END_TEST

START_TEST(test_load_obj_order)
{
    vector_t* uvs = vector_init();
    vector_t* vertices = vector_init();
    vector_t* normals = vector_init();

    bool res = load_obj("mock_assets/test.obj", uvs, normals, vertices);

    ck_assert_msg(res == true, "load obj returned false");

    //face1 2/1/1 3/2/1 1/3/1 (Indices start at 1)
    ck_assert_msg(array_eq(vector_get(vertices, 0), v1, 3)  == true, "Vertex1/Triangle1 wrong vertex");
    ck_assert_msg(array_eq(vector_get(uvs, 0), uv0, 2)  == true, "Vertex1/Triangle1 wrong uv");
    ck_assert_msg(array_eq(vector_get(normals, 0), n0, 3)  == true, "Vertex1/Triangle1 wrong normal");

    ck_assert_msg(array_eq(vector_get(vertices, 1), v2, 3)  == true, "Vertex2/Triangle1 wrong vertex");
    ck_assert_msg(array_eq(vector_get(uvs, 1), uv1, 2)  == true, "Vertex2/Triangle1 wrong uv");
    ck_assert_msg(array_eq(vector_get(normals, 1), n0, 3)  == true, "Vertex2/Triangle1 wrong normal");

    ck_assert_msg(array_eq(vector_get(vertices, 2), v0, 3)  == true, "Vertex3/Triangle1 wrong vertex");
    ck_assert_msg(array_eq(vector_get(uvs, 2), uv2, 2)  == true, "Vertex3/Triangle1 wrong uv");
    ck_assert_msg(array_eq(vector_get(normals, 2), n0, 3)  == true, "Vertex3/Triangle1 wrong normal");

    //face2 2/1/1 4/4/1 3/2/1 (Indices start at 1)
    ck_assert_msg(array_eq(vector_get(vertices, 3), v1, 3)  == true, "Vertex1/Triangle2 wrong vertex");
    ck_assert_msg(array_eq(vector_get(uvs, 3), uv0, 2)  == true, "Vertex1/Triangle2 wrong uv");
    ck_assert_msg(array_eq(vector_get(normals, 3), n0, 3)  == true, "Vertex1/Triangle2 wrong normal");

    ck_assert_msg(array_eq(vector_get(vertices, 4), v3, 3)  == true, "Vertex2/Triangle2 wrong vertex");
    ck_assert_msg(array_eq(vector_get(uvs, 4), uv3, 2)  == true, "Vertex2/Triangle2 wrong uv");
    ck_assert_msg(array_eq(vector_get(normals, 4), n0, 3)  == true, "Vertex2/Triangle2 wrong normal");

    ck_assert_msg(array_eq(vector_get(vertices, 5), v2, 3)  == true, "Vertex3/Triangle2 wrong vertex");
    ck_assert_msg(array_eq(vector_get(uvs, 5), uv1, 2)  == true, "Vertex3/Triangle2 wrong uv");
    ck_assert_msg(array_eq(vector_get(normals, 5), n0, 3)  == true, "Vertex3/Triangle2 wrong normal");

    vector_foreach(uvs, &vector_generic_item_free);
    vector_free(uvs);

    vector_foreach(vertices, &vector_generic_item_free);
    vector_free(vertices);

    vector_foreach(normals, &vector_generic_item_free);
    vector_free(normals);
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

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
