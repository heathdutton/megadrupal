<?php

/**
 * @file
 * Expose returned status code as a context condition.
 */
class status_code_context_condition extends context_condition {
  /**
   * Omit condition values. We will provide a custom input form for our conditions.
   */
  function condition_values() {
    return array();
  }

  /**
   * Condition form.
   */
  function condition_form($context) {
    $form = parent::condition_form($context);
    unset($form['#options']);

    $form['#type'] = 'textarea';
    $form['#default_value'] = implode("\n", $this->fetch_from_context($context, 'values'));
    return $form;
  }

  /**
   * Condition form submit handler.
   */
  function condition_form_submit($values) {
    $parsed = array();
    $items = explode("\n", $values);
    if (!empty($items)) {
      foreach ($items as $v) {
        $v = trim($v);
        if (!empty($v)) {
          $parsed[$v] = $v;
        }
      }
    }
    return $parsed;
  }

  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {
      // Check the status code of current page request
      $current_status = explode(' ', drupal_get_http_header('Status'));
      $status_code = isset($current_status[0]) ? $current_status[0] : NULL;

      foreach ($this->get_contexts() as $context) {
        $paths = $this->fetch_from_context($context, 'values');
        if (in_array($status_code, $paths)) {
          $this->condition_met($context);
        }
      }
    }
  }
}