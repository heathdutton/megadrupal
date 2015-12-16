<?php

/**
 * @file
 * Class definition for the Views datetime filter.
 */

/**
 * Class eventbrite_api_handler_filter_datetime
 */
class eventbrite_api_handler_filter_datetime extends views_handler_filter_date {

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
  }

  function exposed_validate(&$form, &$form_state) {
    parent::exposed_validate($form, $form_state);
  }

  function validate_valid_time(&$form, $operator, $value) {
    parent::validate_valid_time($form, $operator, $value);
  }

  function build_group_validate($form, &$form_state) {
    parent::build_group_validate($form, $form_state);
  }

  function accept_exposed_input($input) {
    $rc = parent::accept_exposed_input($input);
    return $rc;
  }

  function op_between($field) {
    // Use the substitutions to ensure a consistent timestamp.
    $query_substitutions = views_views_query_substitutions($this->view);
    $a = intval(strtotime($this->value['min'], $query_substitutions['***CURRENT_TIME***']));
    $b = intval(strtotime($this->value['max'], $query_substitutions['***CURRENT_TIME***']));
    // Format the dates properly.
    $a = date('Y-m-d H:i:s', $a);
    $b = date('Y-m-d H:i:s', $b);

    // This is safe because we are manually scrubbing the values.
    // It is necessary to do it this way because $a and $b are formulas when using an offset.
    $operator = strtoupper($this->operator);
    $this->query->add_where_expression($this->options['group'], "$field $operator '$a' AND '$b'");
  }

  function op_simple($field) {
    // Use the substitutions to ensure a consistent timestamp.
    $query_substitutions = views_views_query_substitutions($this->view);
    $value = intval(strtotime($this->value['value'], $query_substitutions['***CURRENT_TIME***']));
    // Format the date properly.
    $value = date('Y-m-d H:i:s', $value);

    // This is safe because we are manually scrubbing the value.
    // It is necessary to do it this way because $value is a formula when using an offset.
    $this->query->add_where_expression($this->options['group'], "$field $this->operator '$value'");
  }

}