<?php
/**
 * @file
 * Handler for the doctor listen item data.
 */

class doctor_handler_data_formatter extends views_handler_field {
  function render($values) {
    static $shown = FALSE;

    // Loading the listen item from the DB. The data is already un serialized.
    $doctor = doctor_load($values->doctor_id);

    if (!empty($this->options['formatter_data'])) {
      if (function_exists($this->options['formatter_data'])) {
        return call_user_func($this->options['formatter_data'], $doctor->data);
      }
      else {
        if (!$shown) {
          // Notify the user that the callback function is not exists.
          drupal_set_message(t('The function @name was not found, rendering with print_r.', array('@name' => $this->options['formatter_data'])), 'error');
          $shown = TRUE;
        }
      }
    }

    // Finally, when nothing invoked - present the data with simple print_r().
    return print_r($doctor->data, TRUE);
  }

  /**
   * Let the user select the callback function that will present the data.
   * Similar to the drush just in the UI.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // No multiple selection please.
    $form['expose']['multiple']['#access'] = FALSE;

    $form['formatter_data'] = array(
      '#type' => 'textfield',
      '#title' => t('Formatter function name'),
      '#description' => t("Select a function that will present the data if the listen item in the way you'd like to."),
      '#default_value' => $this->options['formatter_data'],
    );
  }

  /**
   * Pre defined settings.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['formatter_data'] = array('default' => '');

    return $options;
  }
}
