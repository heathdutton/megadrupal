<?php

/**
 * @file
 * Fortissimmo description handler
 */
class views_handler_field_description extends views_handler_field {

  /**
   * Options definition
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['description_input_format'] = array('default' => NULL);
    return $options;
  }

  /**
   * Provide formatter option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->options;
    $default_format = variable_get('filter_fallback_format', NULL);
    $format_options = array();
    $formats = filter_formats();
    foreach ($formats as $id => $format) {
      $format_options[$id] = $format->name;
    }

    $form['description_input_format'] = array(
      '#type' => 'select',
      '#title' => t('Input format'),
      '#description' => t('Specify the input format to be used for the description'),
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#options' => $format_options,
      '#default_value' => isset($options['description_input_format']) ? $options['description_input_format'] : $default_format,
    );
  }

  function render($values) {
    $options = $this->options;
    $format = $options['description_input_format'];
    return check_markup($values->{$this->field_alias}, $format, FALSE);
  }
}