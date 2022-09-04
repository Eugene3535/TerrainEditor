#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    Transform();
    Transform(const Transform& other);
	Transform(const Transform&& other);
	Transform& operator = (const Transform& other);
	Transform& operator = (const Transform&& other);

    Transform* setPosition(const glm::vec3& position);
    Transform* move(const glm::vec3& offset);
    Transform* rotate(float angle, const glm::vec3& axis);
    Transform* scale(const glm::vec3& scale);
    Transform* setOrigin(const glm::vec3& origin);

    const float* getMatrix();

private:
    glm::mat4 m_matrix;
    glm::vec3 m_position;
    glm::vec3 m_rotation_axis;
    glm::vec3 m_scale;
    glm::vec3 m_origin;
    float m_angle;
};

