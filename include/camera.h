#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
    public:
        // attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        // euler angles
        float yaw;
        float pitch;

        // camera options
        float movementSpeed;
        float sensitivity;
        float zoom;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) {
            this->movementSpeed = SPEED;
            this->sensitivity = SENSITIVITY;
            this->zoom = ZOOM;
            this->pitch = pitch;
            this->yaw = yaw;

            this->position = position;
            this->worldUp = up;

            updateCameraVectors();
        }

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
            this->movementSpeed = SPEED;
            this->sensitivity = SENSITIVITY;
            this->zoom = ZOOM;
            this->pitch = pitch;
            this->yaw = yaw;

            this->position = glm::vec3(posX, posY, posZ);
            this->worldUp = glm::vec3(upX, upY, upZ);

            updateCameraVectors();
        }

        glm::mat4 getViewMatrix() {
            return glm::lookAt(position, position + front, up);
        }

        void processKeyboard(CameraMovement direction, float deltaTime) {
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

        void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true) {
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

        void processMouseScroll(float yOffset) {
            zoom -= yOffset;
            if (zoom < 1.0f) {
                zoom = 1.0f;
            } else if (zoom > 45.0f) {
                zoom = 45.0f;
            }
        }
    
    private:
        void updateCameraVectors() {
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
};

#endif