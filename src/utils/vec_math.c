#include <cglm/cglm.h>

void vec3_bounce(vec3 vector, vec3 normal, float friction, vec3 bounced_vector)
{
    float projection = glm_vec_dot(vector, normal);

    vec3 scaled_normal;
    glm_vec_scale(normal, -2.0f * projection, scaled_normal);

    glm_vec_add(scaled_normal, vector, bounced_vector);
    glm_vec_scale(bounced_vector, friction, bounced_vector);
}
