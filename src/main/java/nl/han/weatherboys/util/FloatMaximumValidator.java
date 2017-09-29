package nl.han.weatherboys.util;

import javax.validation.ConstraintValidator;
import javax.validation.ConstraintValidatorContext;

public class FloatMaximumValidator implements ConstraintValidator<FMax, Float> {

    private float max;

    @Override
    public void initialize(FMax constraintAnnotation) {
        this.max = constraintAnnotation.value();
    }

    @Override
    public boolean isValid(Float value, ConstraintValidatorContext context) {
        return value == null || value <= max;
    }
}
