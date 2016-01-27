<?php

/**
 * @file
 * Definition of NumericFieldFilterConverterNumber.
 */

/**
 * Class converting fields defined in Number module into a SQL expression.
 */
class NumericFieldFilterConverterNumber implements NumericFieldFilterConvertableInterface {

  /**
   * Generate extra value form elements to be merged into value form of handler.
   *
   * Converter classes should use this method to provide any extra form elements
   * that will be merged into filter's value form, required by the field type
   * they work with. Basically if your field type requires any thing more than
   * just formula textfield, it is here where you will be asked to provide those
   * additional form elements. Advice: probably all you'll want to do here is
   * to invoke 'widget_form' on your field type and, if necessary, slightly
   * alter it.
   *
   * @param array $form
   *   Array of form into where your output will be merged, you might get
   *   some useful configuration options from this array
   * @param array $form_state
   *   Array of form state that acompanies $form
   * @param array $field
   *   Field API field definition array for which the additional form elements
   *   are asked to be generated
   * @param array $instance
   *   Field API instance definition array of an instance of the supplied
   *   $field, trying to get you as much context as possible
   * @param mixed $default_value
   *   Previously submitted values of your form elements in the format as they
   *   are expected to be submitted. You are adviced to use this argument as a
   *   source of default values for your extra form elements
   *
   * @return array
   *   Extra value form elements that need to be merged into value form of
   *   filter handler by the supplied field type
   */
  static public function valueFormElement($form, $form_state, $field, $instance, $default_value) {
    // For Number module we would get only 'value' text field from Number module
    // however, it would need to be hidden, because in Numeric Field Filter we
    // use 'formula' text field instead.
    return array();
  }

  /**
   * Information on what Field API field types this class supports.
   *
   * Provide information on what field types this class is capable of converting
   * into a SQL expression.
   *
   * @return array
   *   Array of field types this class supports
   */
  static public function supportedFieldTypes() {
    // It works with all scalar numbers, like those defined in number.module.
    return array_keys(module_invoke('number', 'field_info'));
  }

  /**
   * Convert Field API field into SQL expression.
   *
   * Convert Field API field structure into SQL expression that can be used in a
   * Views filter. If for whatever reason you find it impossible to convert the
   * supplied field into a SQL expression, you are expected to throw an
   * exception of type NumericFieldFilterUnconvertableFieldException.
   *
   * @param array $field
   *   Field API field definition array for which the SQL expression should be
   *   generated
   * @param string $table_alias
   *   Table alias under which the table of the supplied field is referred to
   *   in the view for which you are asked to generate the SQL expression
   * @param mixed $values
   *   Data submitted into your extra value form elements defined in
   *   NumericFieldFilterConvertableInterface::valueFormElement(). Probably you
   *   will need these while generating the SQL expression
   * @param bool $revision
   *   Whether you are asked to generate the SQL expression on a revision of a
   *   field value or on the non-revisioned value of field. Mostly
   *   revision-wiseness is already taken care in $table_alias argument, because
   *   table alias will vary based on whether filter operates on revision or
   *   not, yet, we provide you extra context in case you need to know it
   *
   * @return string
   *   SQL expression that yeilds value of that field, it will be used to
   *   compose WHERE condition in a Views filter
   */
  static public function fieldConvertToExpression($field, $table_alias, $values, $revision = FALSE) {
    $revision = $revision ? 'FIELD_LOAD_REVISION' : 'FIELD_LOAD_CURRENT';
    $field = array_pop($field['storage']['details']['sql'][$revision]);
    return $table_alias . '.' . $field['value'];
  }

  /**
   * Provide column name on which Numeric Field Filter should filter.
   *
   * Filter handler for this column of each supported Field API field type will
   * be changed to the filter handler provided by Numeric Field Filter module.
   *
   * @param array $field
   *   Field API field definition array for which the column name should be
   *   returned
   *
   * @return string
   *   Column name for which the filter handler should be changed to the handler
   *   provided by Numeric Field Filter module
   */
  static public function columnName($field) {
    return 'value';
  }
}
