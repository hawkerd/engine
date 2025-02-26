#include <camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    this->movementSpeed = SPEED;
    this->sensitivity = SENSITIVITY;
    this->zoom = ZOOM;
    this->pitch = pitch;
    this->yaw = yaw;

    this->position = position;
    this->worldUp = up;

    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    this->movementSpeed = SPEED;
    this->sensitivity = SENSITIVITY;
    this->zoom = ZOOM;
    this->pitch = pitch;
    this->yaw = yaw;

    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);

    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    // ensure consistent movement across frame rates
    float velocity = movementSpeed * deltaTime;

    // update position vector
    if (direction == FORWARD) {
        position += front * velocity;
    }
    if (direction == BACKWARD) {
        position -= front * velocity;
    }
    if (direction == LEFT) {
        position -= right * velocity;
    }
    if (direction == RIGHT) {
        position += right * velocity;
    }
    if (direction == UP) {
        position += worldUp * velocity;
    }
    if (direction == DOWN) {
        position -= worldUp * velocity;
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    // account for sensitivity
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrainPitch) {
        pitch = glm::clamp(pitch, -89.0f, 89.0f);
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
    zoom -= yOffset;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    } else if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::updateCameraVectors() {
    // calculate new front vector
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);

    // recalculate right and up vector
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
