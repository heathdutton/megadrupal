<?php

/**
 * @file
 * Context condition plugin implementation.
 */

/**
 * Expose current date as a context condition.
 */
class context_date_condition extends context_condition {

  function condition_form($context) {
    $defaults = $this->fetch_from_context($context, 'values');

    $form['after'] = $this->condition_form_date_field('after', t('From'), $defaults);
    $form['before'] = $this->condition_form_date_field('before', t('Until'), $defaults);

    return $form;
  }

  /**
   * Generate a date field for using in condition form.
   *
   * @param $name
   *   Field machine name.
   * @param $title
   *   Field title.
   * @param $defaults
   *   Array with default form values.
   */
  function condition_form_date_field($name, $title, $defaults) {
    // Use a datepicker from date_popup module if available.
    if (module_exists('date_popup')) {
      // Need the is_int() check since the first time it gets called,
      // date_popup will pass an array with date and time as items.
      if (!empty($defaults[$name]) && is_int($defaults[$name])) {
        $date = new DateTime();
        $date->setTimestamp($defaults[$name]);
      }
      $form = array(
        '#title' => $title,
        '#type' => 'date_popup',
        '#default_value' => isset($date) ? $date->format('Y-m-d H:i:s') : '',
        '#date_format' => variable_get('date_format_short', 'm/d/Y - H:i'),
      );
    }
    // Fallback to a simple text field similar to node form one.
    else {
      $form = array(
        '#title' => $title,
        '#type' => 'textfield',
        '#description' => t('Format: %time.', array('%time' => !empty($defaults[$name]) ? format_date($defaults[$name], 'custom', 'Y-m-d H:i:s O') : format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s O'))),
        '#default_value' => isset($defaults[$name]) ? format_date($defaults[$name], 'custom', 'Y-m-d H:i:s O') : '',
      );
    }

    return $form;
  }

  function condition_form_submit($values) {
    foreach (array('before', 'after') as $field) {
      // Need the is_string() check since the first time it gets called,
      // date_popup will pass an array with date and time as items.    
      if (!empty($values[$field]) && is_string($values[$field])) {
        $values[$field] = strtotime($values[$field]);
      }
    }
    return $values;
  }

  function execute() {
    if ($this->condition_used()) {
      $timezone = variable_get('date_default_timezone', 'UTC');
      date_default_timezone_set($timezone);
      $current_timestamp = function_exists('force_timestamp_get') ? force_timestamp_get() : REQUEST_TIME;
      $current_date = $this->get_datetime_object($current_timestamp);

      foreach ($this->get_contexts() as $context) {
        $values = $this->fetch_from_context($context, 'values');

        // If a 'before' date wasn't set, assume date is before for later AND
        // check. If it's set check to see if current date is before the
        // entered value.
        $before = empty($values['before']) ? TRUE : ($current_date < $this->get_datetime_object($values['before']));

        // If an 'after' date wasn't set, assume date is after for later AND
        // check. If it's set check to see if current date is after the
        // entered value.
        $after = empty($values['after']) ? TRUE : ($current_date > $this->get_datetime_object($values['after']));

        if ($before && $after) {
          $this->condition_met($context);
        }

      }
    }
  }

  /**
   * Given an Unix timestamp, return a date object.
   */
  function get_datetime_object($timestamp) {
    $date = new DateTime();
    $date->setTimestamp($timestamp);
    return $date;
  }

}
