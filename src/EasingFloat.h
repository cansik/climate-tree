//
// Created by Florian on 19.11.17.
//

#ifndef CLIMATE_TREE_EASINGFLOAT_H
#define CLIMATE_TREE_EASINGFLOAT_H


class EasingFloat {
private:
    float value;
    float target;
    float easing;

public:
    explicit EasingFloat(float value = 0.0, float easing = 0.01);
    void set(float target);
    void setEasing(float easing);
    float getValue();
    void update();
};


#endif //CLIMATE_TREE_EASINGFLOAT_H
