<?php

/**
 * @file
 */

class ec_common_views_handler_filter_exposed_fieldset extends views_handler_filter {

  function query() { }

  function can_expose() {
    return FALSE;
  }

  function admin_summary() {
    $fields = !empty($this->options['fields']) ? array_filter($this->options['fields']) : array();
    return empty($fields) ? t('No fields selected') : implode(',', $fields);
  }

  function operator_form(&$form, &$form_state) { }

  function value_form(&$form, &$form_state) {
    $this->options += array(
      'fields' => array(),
    );

    $form['fieldset_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Fieldset'),
      '#default_value' => $this->options['fieldset_title'],
    );
    $form['fieldset_collapsible'] = array(
      '#type' => 'checkbox',
      '#title' => t('Collapsible'),
      '#default_value' => $this->options['fieldset_collapsible'],
    );
    $form['fieldset_collapsed'] = array(
      '#type' => 'checkbox',
      '#title' => t('Collapsed'),
      '#default_value' => $this->options['fieldset_collapsed'],
    );

    $fields = $this->view->display[$this->view->current_display]->display_options['filters'];
    if (!is_array($fields)) {
      $fields = $this->view->display['default']->display_options['filters'];
    }
    $options = array_filter($fields, array($this, '_get_exposed_filters'));
    $options = array_map(array($this, '_map_options'), $options);

    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields'),
      '#default_value' => $this->options['fields'],
      '#options' => $options,
      '#description' => t('Select fields which will be moved into the fieldset'),
    );
  }

  function _get_exposed_filters($a) {
    return $a['exposed'];
  }

  function _map_options($a) {
    return $a['expose']['label'];
  }

}


