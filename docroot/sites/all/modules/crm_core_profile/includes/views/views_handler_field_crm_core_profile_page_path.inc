<?php

/**
 * @file
 * Handler for crm_core_profile path and redirect fields.
 */

/**
 * Field handler to present the path to the profile page.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_crm_core_profile_page_path extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['link'] = array('default' => FALSE, 'bool' => TRUE);
    $options['absolute'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function construct() {
    parent::construct();
    $this->additional_fields['page_path'] = 'page_path';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Render this field as links?'),
      '#default_value' => $this->options['link'],
      '#description' => t('Enable this option to output this field as a link.'),
    );
    $form['absolute'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use absolute link (begins with "http://")'),
      '#default_value' => $this->options['absolute'],
      '#description' => t('Enable this option to output an absolute link. Required if you want to use the path as a link destination (as in "output this field as a link" above).'),
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $path = $this->get_value($values, 'page_path');
    if ($this->options['link']) {
      return l($path, $path, array('absolute' => $this->options['absolute']));
    }
    else {
      return $path;
    }
  }
}
