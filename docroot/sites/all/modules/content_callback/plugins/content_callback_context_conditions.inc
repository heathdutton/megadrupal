<?php
/**
 * @file
 * Context condition plugin to provide a trigger for content callbacks.
 */

/**
 * Expose the content callback as the context conditions.
 */
class content_callback_context_condition_content_callback extends context_condition {

  function condition_values() {
    $values = array();
    $callbacks = content_callback_callbacks_get();
    foreach ($callbacks as $key => $callback) {
      $values[$key] = $callback['title'];
    }
    asort($values);
    return $values;
  }

  /**
   * Condition form.
   */
  function condition_form($context) {
    return array(
      '#title' => $this->title,
      '#description' => t('Select which content callbacks should this condition activate.'),
      '#options' => $this->condition_values(),
      '#type' => 'checkboxes',
      '#default_value' => $this->fetch_from_context($context, 'values'),
    );
  }

  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {
      foreach ($this->get_contexts() as $context) {
        $values = $this->fetch_from_context($context, 'values');

        $active_callbacks = content_callback_active();
        foreach ($values as $value) {
          if (in_array($value, $active_callbacks)) {
            $this->condition_met($context);
            break;
          }
        }
      }
    }
  }
}
