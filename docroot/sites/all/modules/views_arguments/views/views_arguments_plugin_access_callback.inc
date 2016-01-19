<?php

class views_arguments_plugin_access_callback extends views_plugin_access {

  /**
   * Overrides views_plugin_access::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['callback'] = array('default' => '');
    $options['arguments'] = array('default' => array());

    return $options;
  }

  /**
   * Overrides views_plugin_access::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    _views_arguments_handler_access_form($form, $this->options);
    $form['callback']['#required'] = TRUE;
  }

  /**
   * Overrides views_plugin_access::summary_title().
   */
  function summary_title() {
    if ($this->options['callback']) {
      $callbacks = _views_arguments_access_callbacks();
      if (isset($callbacks[ $this->options['callback'] ])) {
        $callback = $callbacks[ $this->options['callback'] ];
        return check_plain($callback['title']) . ' (' . count($this->options['arguments']) . ')';
      }
    }

    return '<strong>' . t('No callback!') . '</strong>';
  }

  /**
   * Overrides views_plugin_access::access().
   */
  function access($account) {
    $callbacks = _views_arguments_access_callbacks();
    $callback = $callbacks[ $this->options['callback'] ];
    return call_user_func_array($callback['callback'], $this->options['arguments']);
  }

  /**
   * Overrides views_plugin_access::get_access_callback().
   */
  function get_access_callback() {
    $callbacks = _views_arguments_access_callbacks();
    $callback = $callbacks[ $this->options['callback'] ];
    return array($callback['callback'], $this->options['arguments']);
  }

}
