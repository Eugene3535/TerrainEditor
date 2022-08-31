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

    Transform* translate(const glm::vec3& point);
    Transform* rotate(float angle, const glm::vec3& axis);
    Transform* scale(const glm::vec3& factors);
    Transform* combine(const Transform& transform);

    const float* getMatrix() const;

private:
    glm::mat4 m_matrix = glm::mat4(1.0f);
};

Transform operator *(const Transform& left, const Transform& right);
Transform& operator *=(Transform& left, const Transform& right);
