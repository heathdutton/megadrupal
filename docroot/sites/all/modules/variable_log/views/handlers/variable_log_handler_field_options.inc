<?php

/**
 * @file
 * Contains the field handler for render options of the variable.
 */

/**
 * Field handler to provide options of the variable.
 */
class variable_log_handler_field_options extends views_handler_field {

  /**
   * Information about options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['option'] = array('default' => 'realm');
    return $options;
  }

  /**
   * Build the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['option'] = array(
      '#type' => 'select',
      '#title' => t('Option of variable'),
      '#options' => array(
        'realm' => t('Realm'),
        'key' => t('Realm key'),
      ),
      '#default_value' => !empty($this->options['option']),
      '#description' => t('Select the option, which need to show.'),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render the field.
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_option('', $value);
  }

  /**
   * Render the field as link.
   */
  public function render_option($data, $value) {
    if (!empty($this->options['option'])) {
      $value = unserialize($value);
      if (!empty($value[$this->options['option']])) {
        $data = $value[$this->options['option']];
      }
    }

    return $data;
  }
}
