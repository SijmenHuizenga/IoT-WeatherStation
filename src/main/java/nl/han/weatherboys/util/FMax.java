package nl.han.weatherboys.util;

import javax.validation.Constraint;
import javax.validation.Payload;

import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.Target;

import static java.lang.annotation.ElementType.ANNOTATION_TYPE;
import static java.lang.annotation.ElementType.FIELD;
import static java.lang.annotation.ElementType.METHOD;
import static java.lang.annotation.RetentionPolicy.RUNTIME;

/**
 * Float Maxiumum validator
 */
@Target({METHOD, FIELD, ANNOTATION_TYPE})
@Retention(RUNTIME)
@Constraint(validatedBy = FloatMaximumValidator.class)
@Documented
public @interface FMax {

    public String message() default "{java.math.BigDecimal.range.error}";
    public Class<?>[] groups() default {};
    public Class<? extends Payload>[] payload() default {};

    float value();

}
