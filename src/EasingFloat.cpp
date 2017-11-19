//
// Created by Florian on 19.11.17.
//

#include "EasingFloat.h"

EasingFloat::EasingFloat(float value, float easing) {
    this->target = value;
    this->value = value;

    this->easing = easing;
}

void EasingFloat::set(float target) {
    this->target = target;
}

float EasingFloat::getValue() {
    return value;
}

void EasingFloat::setEasing(float easing) {
    this->easing = easing;
}

void EasingFloat::update() {
    auto delta = target - value;
    value += (delta * easing);
}
