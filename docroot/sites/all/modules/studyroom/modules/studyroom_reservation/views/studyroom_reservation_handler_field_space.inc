<?php

/**
 * @file
 * Definition of studyroom_reservation_handler_field_space.
 */

/**
 * Contains a field handler to take care of displaying spaces.
 */
class studyroom_reservation_handler_field_space extends views_handler_field {
  /**
   * Add space_name checkbox to options list.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['space_name'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Construct space_name form element.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['space_name'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display name'),
      '#description' => t('Check this box to display the spaces actual name instead of the space ID value.'),
      '#default_value' => $this->options['space_name'],
    );
  }

  /**
   * Render the field.
   */
  public function render($values) {
    if ($this->options['space_name']) {
      $studyroom_space = studyroom_space_load($values->studyroom_reservation_space_id);
      if (is_object($studyroom_space)) {
        return $studyroom_space->label();
      }
      else {
        return 'Invalid space';
      }
    }
    else {
      return $values->studyroom_reservation_space_id;
    }
  }
}
