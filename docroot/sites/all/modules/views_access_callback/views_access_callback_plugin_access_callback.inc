<?php

/**
 * Access plugin that provides callback-function-based access control.
 */
class views_access_callback_plugin_access_callback extends views_plugin_access {

  function access($account) {
    return views_access_callback_access_callback($this->options['views_access_callbacks'], array($this->view->name, $this->display->id, $this->options['views_access_callback']));
  }

  function get_access_callback() {
    return array('views_access_callback_access_callback', array($this->view->name, $this->display->id, $this->options['views_access_callback']));
  }

  function summary_title() {
    return t('Callback function') . ': ' . $this->options['views_access_callback'];
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['views_access_callback'] = array('default' => 'TRUE');

    return $options;
  }


  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $callbacks = array();
    $module_info = system_get_info('module');

    // Get list of permissions
    foreach (module_implements('views_access_callbacks') as $module) {
      $functions = module_invoke($module, 'views_access_callbacks');
      foreach ($functions as $function => $name) {
        $callbacks[$module_info[$module]['name']][$function] = check_plain($name);
      }
    }

    ksort($callbacks);

    $form['views_access_callback'] = array(
      '#type' => 'select',
      '#options' => $callbacks,
      '#title' => t('Callbacks'),
      '#default_value' => $this->options['views_access_callback'],
      '#description' => t('Only users for which selected callback function returns TRUE will be able to access this display.'),
    );
  }
}
