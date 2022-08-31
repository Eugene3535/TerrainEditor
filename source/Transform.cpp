#include "Transform.hpp"

#include <glm/gtc/type_ptr.hpp>

Transform::Transform()
{
}

Transform::Transform(const Transform& other):
    m_matrix(other.m_matrix)
{

}

Transform::Transform(const Transform&& other):
    m_matrix(other.m_matrix)
{

}

Transform& Transform::operator = (const Transform& other)
{
    m_matrix = other.m_matrix;

    return *this;
}

Transform& Transform::operator = (const Transform&& other)
{
    m_matrix = other.m_matrix;

    return *this;
}

Transform* Transform::translate(const glm::vec3& point)
{
    m_matrix[3] = m_matrix[0] * point.x + m_matrix[1] * point.y + m_matrix[2] * point.z + m_matrix[3];

    return this;
}

Transform* Transform::rotate(float angle, const glm::vec3& axis)
{
    m_matrix = glm::rotate(m_matrix, glm::radians(angle), axis);

    return this;
}

Transform* Transform::scale(const glm::vec3& factors)
{
	m_matrix[0] *= factors.x;
	m_matrix[1] *= factors.y;
	m_matrix[2] *= factors.z;

    return this;
}

Transform* Transform::combine(const Transform& transform)
{
    m_matrix *= transform.m_matrix;

    return this;
}

const float* Transform::getMatrix() const
{
    return glm::value_ptr(m_matrix);
}

Transform operator *(const Transform& left, const Transform& right)
{
    return *(Transform(left).combine(right));
}

Transform& operator *=(Transform& left, const Transform& right)
{
    return *(left.combine(right));
}
