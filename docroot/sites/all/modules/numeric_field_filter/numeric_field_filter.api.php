<?php

/**
 * @file
 * Documentation for Numeric Field Filter module.
 */

/**
 * Hook to provide class names of Numeric Field Filter converters.
 *
 * Numeric Field Filter converter is a class that implements interface
 * NumericFieldFilterConvertableInterface, basically such class should be able
 * to convert a specific Field API field type into a SQL expression that later
 * on will be used in the filter handler defined by Numeric Field Filter module.
 * In this hook you are supposed to return an array of class names your module
 * provides. As a rule of thumb you should name your converter classes in the
 * following fashion NumericFieldFilter[FieldName], where [FieldName] is the
 * name of field type that your converter is capable of converting into a SQL
 * expression.
 *
 * @return array
 *   Array of class names of converters that your module provides
 */
function hook_numeric_field_filter_converter_info() {
  // See down below at the implementation of this dummy converter.
  return array('NumericFieldFilterDummy');
}

/**
 * Dummy converter class, just to show how to do a real one.
 */
class NumericFieldFilterDummy implements NumericFieldFilterConvertableInterface {

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
    // Let's assume there exist 2 field types: 'dummy1' and 'dummy2'.
    return array('dummy1', 'dummy2');
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
    // Let's assume for dummy1 field the actual DB column we want to filter on
    // is 'column_dummy1', whereas for dummy2 field the column is
    // 'column_dummy2'.
    switch ($field['type']) {
      case 'dummy1':
        return 'column_dummy1';

      case 'dummy2':
        return 'column_dummy2';
    }
  }

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
    // Let's say we want to let user preprocess the value.
    $element = array(
      '#type' => 'select',
      '#title' => t('Preprocess value'),
      '#options' => array(
        'no' => t('No preprocess'),
        'smaller' => t('Make the value 10 times smaller'),
      ),
      '#default_value' => $default_value,
    );

    return $element;
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
    $column = '';
    // As we remember, we have different columns for 2 different field types.
    switch ($field['type']) {
      case 'dummy1':
        $column = 'column_dummy1';
        break;

      case 'dummy2':
        $column = 'column_dummy2';
        break;
    }
    // Right now we've got the field's value, but as we've defined some extra
    // options in valueFormElement() method, it's time to implement them.
    $sql_expression = $table_alias . '.' . $column;
    if ($values == 'smaller') {
      // User has asked to make this field smaller.. Okay, not problems!
      $sql_expression = '0.1 * ' . $sql_expression;
    }

    return $sql_expression;
  }
}
