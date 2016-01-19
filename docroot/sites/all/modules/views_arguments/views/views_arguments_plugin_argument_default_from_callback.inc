<?php

class views_arguments_plugin_argument_default_from_callback extends views_plugin_argument_default {

  /**
   * Overrides views_plugin_argument_default::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['callback'] = array('default' => '');

    return $options;
  }

  /**
   * Overrides views_plugin_argument_default::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $callbacks = _views_arguments_argument_callbacks();
    $form['callback'] = array(
      '#type' => 'select',
      '#title' => t('Callback function'),
      '#options' => array_map(function($callback) {
        return $callback['title'];
      }, $callbacks),
      '#default_value' => @$this->options['callback'],
      '#empty_option' => t('- Select -'),
      '#states' => array(
        'required' => array(
          'select[name="options[default_argument_type]"]' => array('value' => 'from_callback'),
        ),
      ),
    );
  }

  /**
   * Overrides views_plugin_argument_default::options_validate().
   */
  function options_validate(&$form, &$form_state) {
    $options = $form_state['values']['options'];
    if ($options['default_action'] == 'default' && $options['default_argument_type'] == 'from_callback') {
      if (!trim($options['argument_default']['from_callback']['callback'])) {
        return form_error($form['callback'], t('You must select a callback'));
      }
    }
  }

  /**
   * Overrides views_plugin_argument_default::get_argument().
   */
  function get_argument() {
    $callbacks = _views_arguments_argument_callbacks();
    $callback = $callbacks[ $this->options['callback'] ];

    $arguments = $callback['arguments'];
    array_unshift($arguments, $this);

    $argument = call_user_func_array($callback['callback'], $arguments);
    return $argument;
  }

}
