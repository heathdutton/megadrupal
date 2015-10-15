<?php

/**
 * @file
 * Definition of linkchecker_handler_filter_in_code.
 */

/**
 * Simple filter to exclude http status codes that cannot fixed by a user.
 *
 * @ingroup views_filter_handlers
 */
class linkchecker_views_handler_filter_in_code extends views_handler_filter {

  /**
   * Provide text for the administrative summary.
   */
  function admin_summary() {
    $linkchecker_ignore_response_codes = preg_split('/(\r\n?|\n)/', variable_get('linkchecker_ignore_response_codes', "200\n206\n302\n304\n401\n403"));
    return check_plain((string) $this->operator) . ' ' . check_plain((string) implode(',', $linkchecker_ignore_response_codes));
  }

  /**
   * Disable expose form items.
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * Set defaults for the filter options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['operator']['default'] = 'not in';
    $options['linkchecker_ignore_response_codes']['default'] = preg_split('/(\r\n?|\n)/', variable_get('linkchecker_ignore_response_codes', "200\n206\n302\n304\n401\n403"));

    return $options;
  }

  /**
   * Define the operators supported for code.
   */
  function operators() {
    $operators = array(
      'not in' => array(
        'title' => t('Is not one of'),
        'short' => t('not in'),
        'short_single' => t('<>'),
        'method' => 'op_code',
        'values' => 1,
      ),
    );

    return $operators;
  }

  function op_code() {
    $this->value = preg_split('/(\r\n?|\n)/', variable_get('linkchecker_ignore_response_codes', "200\n206\n302\n304\n401\n403"));

    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", $this->value, $this->operator);
  }

  function query() {
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}();
    }
  }

  /**
   * Overrides views_handler_filter::value_form.
   */
  function value_form(&$form, &$form_state) {
    $form['linkchecker_ignore_response_codes'] = array(
      '#default_value' => variable_get('linkchecker_ignore_response_codes', "200\n206\n302\n304\n401\n403"),
      '#type' => 'textarea',
      '#title' => t("Don't treat these response codes as errors"),
      '#description' => t('One HTTP status code per line, e.g. 403.'),
    );
  }

  /**
   * Overrides views_handler_filter::value_submit.
   */
  function value_submit($form, &$form_state) {
    $options = &$form_state['values']['options'];
    variable_set('linkchecker_ignore_response_codes', $options['linkchecker_ignore_response_codes']);
  }

  /**
   * Overrides views_handler_filter::value_validate.
   */
  function value_validate($form, &$form_state) {
    $options = &$form_state['values']['options'];
    $options['linkchecker_ignore_response_codes'] = trim($options['linkchecker_ignore_response_codes']);

    $ignore_response_codes = preg_split('/(\r\n?|\n)/', $options['linkchecker_ignore_response_codes']);
    foreach ($ignore_response_codes as $ignore_response_code) {
      if (!_linkchecker_isvalid_response_code($ignore_response_code)) {
        form_set_error('options][linkchecker_ignore_response_codes', t('Invalid response code %code found.', array('%code' => $ignore_response_code)));
      }
    }
  }
}
