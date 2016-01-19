<?php

class views_exposed_autohide_plugin_exposed_form_autohide extends views_plugin_exposed_form {

  function option_definition() {
    $options = parent::option_definition();
    $options['min_result_count'] = array('default' => 10);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['min_result_count'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum result count'),
      '#description' => t('Minimum count of result rows to show the exposed form.'),
      '#default_value' => $this->options['min_result_count'],
      '#required' => TRUE,
    );
  }

  function options_validate(&$form, &$form_state) {
    $min_result_count = $form_state['values']['exposed_form_options']['min_result_count'];
    if (!is_numeric($min_result_count) || intval($min_result_count) <= 0) {
      form_set_error('exposed_form_options][min_result_count', t('Please insert a positive integer value'));
    }
  }

  function views_exposed_autohide_exposed_filter_applied() {
    static $cache = NULL;
    if (!isset($cache)) {
      $view = $this->view;
      if (is_array($view->filter) && count($view->filter)) {
        foreach ($view->filter as $filter_id => $filter) {
          if ($filter->is_exposed()) {
            $identifier = $filter->options['expose']['identifier'];
            if (isset($view->exposed_input[$identifier])) {
              $cache = TRUE;
              return $cache;
            }
          }
        }
      }
      $cache = FALSE;
    }

    return $cache;
  }

  function pre_render($values) {
    $total_rows = $this->view->total_rows;
    if (is_null($total_rows)) {
      $total_rows = count($values);
    }
    if ($total_rows >= intval($this->options['min_result_count'])) {
      return;
    }
    if ($this->views_exposed_autohide_exposed_filter_applied()) {
      return;
    }
    $this->view->exposed_widgets = '';
  }

}
