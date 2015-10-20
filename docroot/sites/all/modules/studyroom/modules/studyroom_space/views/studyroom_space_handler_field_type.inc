<?php

/**
 * @file
 * Definition of studyroom_space_handler_field_type.
 */

/**
 * Field handler to translate a location into its readable form.
 */
class studyroom_space_handler_field_type extends views_handler_field {
  /**
   * Add use_raw_value to options list.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['use_raw_value'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Construct use_raw_value form element.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['use_raw_value'] = array(
      '#title' => t('Use raw value'),
      '#description' => t('Check if you want to display the raw value instead of the human readable value.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['use_raw_value']),
    );
  }

  /**
   * Render the field.
   */
  public function render($values) {
    $type = $this->get_value($values);

    // Return the raw value if specified.
    if (!empty($this->options['use_raw_value'])) {
      return $this->sanitize_value($type);
    }

    $location = studyroom_space_location_load($type);
    if (is_object($location)) {
      return $location->label();
    }
  }
}
