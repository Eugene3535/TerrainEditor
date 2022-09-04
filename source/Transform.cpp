#include "Transform.hpp"

#include <glm/gtc/type_ptr.hpp>

Transform::Transform():
    m_matrix(1.0f),
    m_position(0.0f, 0.0f, 0.0f),
    m_rotation_axis(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f),
    m_origin(0.0f, 0.0f, 0.0f),
    m_angle(0.0f)
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

Transform* Transform::setPosition(const glm::vec3& position)
{
    m_position = position;
                         
    return this;
}

Transform* Transform::move(const glm::vec3& offset)
{
    m_position += offset;
                         
    return this;
}

Transform* Transform::rotate(float angle, const glm::vec3& axis)
{
    m_angle = glm::radians(angle);
    m_rotation_axis = axis;

    return this;
}

Transform* Transform::scale(const glm::vec3& scale)
{
    m_scale = scale;

    return this;
}

Transform* Transform::setOrigin(const glm::vec3& origin)
{
    m_origin = origin;

    return this;
}

const float* Transform::getMatrix() 
{   
    m_matrix = glm::translate(glm::mat4(1.0f), m_position);
    m_matrix = glm::rotate(m_matrix, m_angle, m_rotation_axis);
    m_matrix = glm::scale(m_matrix, m_scale);

    return glm::value_ptr(m_matrix);
}