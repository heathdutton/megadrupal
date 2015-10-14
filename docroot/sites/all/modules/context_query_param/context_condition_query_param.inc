<?php

/**
 * @file
 */

/**
 * Expose query parameters as a context condition.
 */
class context_condition_query_param extends context_condition {

  const op_equals = 'equals';
  const op_not_equals = 'not_equals';
  const op_empty = 'empty';
  const op_not_empty = 'not_empty';
  const op_set = 'set';
  const op_not_set = 'not_set';

  /**
   * Overrides context_condition::condition_form_submit().
   */
  function condition_form($context) {
    // Initalize.
    $delta = 0;
    $values = $this->fetch_from_context($context, 'values');
    $values['parameters'] = isset($values['parameters']) && !empty($values) ? unserialize($values['parameters']) : array();
    $notes = '<h3>' . t('Query parameters') . '</h3>';
    $notes .= '<p>' . t('Set as many parameter rulesets as you need. Uncheck "require all rulesets" to trigger this context when any one ruleset matches.') . '</p>';

    $form['parameters'] = array(
      '#theme' => 'context_query_param_settings_table',
      '#prefix' => '<div id="context-query-param-elements-table">' . $notes,
      '#suffix' => '</div>',
    );

    // Build existing entries.
    foreach ($values['parameters'] as $val) {
      // Some options (like not_set) will not have values, so set a placeholder
      $val['value'] = isset($val['value']) ? $val['value'] : NULL;
      $form['parameters'][$delta] = $this->build_parameter_row($delta, $val['name'], $val['operator'], $val['value']);
      $delta++;
    }

    // Add placeholder row.
    $form['parameters'][$delta] = $this->build_parameter_row($delta);

    // Add the "add parameter" button, handled via JS.
    $form['#attached']['js'] = array(drupal_get_path('module', 'context_query_param') . '/context_query_param.form.js');
    $form['add_another'] = array(
      '#type' => 'button',
      '#value' => t('Add parameter'),
      '#attributes' => array(
        // This requires JavaScript, so we'll un-hide via JS.
        'class' => array('element-hidden'),
      ),
    );

    // Allow global "AND" vs. "OR" logic on these parameters.
    $form['require_all_params'] = array(
      '#type' => 'checkbox',
      '#title' => t('Require all rulesets'),
      '#default_value' => isset($values['require_all_params']) ? $values['require_all_params'] : TRUE,
    );

    return $form;
  }

  /**
   * Overrides context_condition::condition_form_submit().
   */
  function condition_form_submit($values) {
    // Clean up the parameters array.
    foreach ($values['parameters'] as $key => $option)  {
      // Clean up empty values for storage in neat, tidy array.
      if ($option['name'] == '') {
        unset($values['parameters'][$key]);
      }

      // Do not save a value when it has no useful meaning.
      if (!in_array($option['operator'], array(self::op_equals, self::op_not_equals))) {
        unset($values['parameters'][$key]['value']);
      }
    }

    // Remove the "add another" button.
    unset($values['add_another']);

    // Serialize the parameters array...
    $values['parameters'] = serialize($values['parameters']);

    return $values;
  }

  /**
   * Evaluates the condition.
   *
   * @param $query_params
   *   An array of query parameters against which to check (e.g. $_GET).
   */
  function execute(array $query_params) {
    if ($this->condition_used()) {
      // Loop through contexts.
      foreach ($this->get_contexts() as $context) {
        $context_values = $this->fetch_from_context($context, 'values');
        $matches = 0;

        // Loop through all provided values.
        $context_values['parameters'] = unserialize($context_values['parameters']);
        foreach ($context_values['parameters'] as $parameter) {
          // If we require all values, increment a counter.
          if ($context_values['require_all_params']) {
            if ($this->check_query_params($query_params, $parameter)) {
              $matches++;
            }
          }
          // If any one value is okay, set the condition as met and break.
          else {
            if ($this->check_query_params($query_params, $parameter)) {
              $this->condition_met($context);
              break;
            }
          }
        }

        // If we're here, we should only meet the condition of all params match.
        if ($context_values['require_all_params'] && $matches == count($context_values['parameters'])) {
          $this->condition_met($context);
        }
      }
    }
  }

  /**
   * Checks an individual query parameter against a set of query parameters. If
   * the parameter is found
   *
   * @param $request
   *   An array of key value pairs representing, e.g., $_GET or $_POST
   *
   * @param $context
   *   The specific parameter we're testing (from our context plugin).
   *
   * @return
   *   TRUE if the given parameter matches the request params; FALSE otherwise.
   */
  protected function check_query_params($request, $context) {
    switch ($context['operator']) {
      case self::op_equals:
        $check = isset($request[$context['name']]) ? $request[$context['name']] : NULL;
        $response = $check == $context['value'];
        break;

      case self::op_not_equals:
        $check = isset($request[$context['name']]) ? $request[$context['name']] : NULL;
        $response = $check != $context['value'];
        break;

      case self::op_empty:
        $check = isset($request[$context['name']]) ? (string) $request[$context['name']] : NULL;
        $response = isset($check) && strlen($check) == 0;
        break;

      case self::op_not_empty:
        $check = isset($request[$context['name']]) ? (string) $request[$context['name']] : NULL;
        $response = isset($check) && strlen($check) > 0;
        break;

      case self::op_set:
        $response = isset($request[$context['name']]);
        break;

      case self::op_not_set:
        $response = !isset($request[$context['name']]);
        break;

      default:
        $response = FALSE;
    }

    return $response;
  }

  /**
   * Builds an individual form row.
   *
   * @param $delta
   *   The row number of this form.
   *
   * @param $name
   *   The default parameter name for this row.
   *
   * @param $operator
   *   The default operator for this row.
   *
   * @param $value
   *   The default parameter value for this row.
   *
   * @return
   *   A form array suitable for use in Drupal's Form API.
   */
  protected function build_parameter_row($delta, $name = NULL, $operator = NULL, $value = NULL) {
    $form['name'] = array(
      '#default_value' => $name,
      '#type' => 'textfield',
      '#size' => 30,
    );
    $form['operator'] = array(
      '#type' => 'select',
      '#default_value' => is_null($operator) ? self::op_equals : $operator,
      '#options' => array(
        self::op_equals => t('must equal'),
        self::op_not_equals => t('must not equal'),
        self::op_set => t('must be set'),
        self::op_not_empty => t('must be set and have some value'),
        self::op_empty => t('must be set and have no value'),
        self::op_not_set => t('must not be set'),
      ),
    );
    $check = ':input[name="conditions[plugins][query_param][values][parameters][' . $delta . '][operator]"]';
    $form['value'] = array(
      '#default_value' => $value,
      '#type' => 'textfield',
      '#size' => 30,
      '#states' => array(
        'disabled' => array(
          $check  => array(
            array('value' => self::op_set),
            array('value' => self::op_not_empty),
            array('value' => self::op_empty),
            array('value' => self::op_not_set),
          ),
        ),
      ),
    );

    return $form;
  }
}
