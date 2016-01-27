<?php

/**
 * @file
 * Handler for Node registration path field.
 */

class views_handler_field_node_registration_path extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['absolute'] = array('default' => FALSE);

    return $options;
  }

  function construct() {
    parent::construct();
    $this->additional_fields['registration_id'] = 'registration_id';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['absolute'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use absolute link ("http://...")'),
      '#default_value' => $this->options['absolute'],
      '#description' => t('Enable this option to output an absolute link. Required if you want to use the path as a link destination (as in "output this field as a link" above).'),
      '#fieldset' => 'alter',
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $registration_id = $this->get_value($values, 'registration_id');

    $uri = node_registration_uri($registration_id);
    $path = $uri['path'];

    return url($path, array('absolute' => $this->options['absolute']));
  }
}
