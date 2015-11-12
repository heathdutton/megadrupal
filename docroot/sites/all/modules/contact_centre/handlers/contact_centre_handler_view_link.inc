<?php

/**
 * @file
 * Definition of contact_centre_handler_view_link.
 */

/**
 * Field handler to create a link to view a message.
 *
 * @ingroup views_field_handlers
 */
class contact_centre_handler_view_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  function access() {
    return user_access('administer contact centre');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $value = $this->get_value($values, 'id');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function render_link($data, $values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = 'admin/contact/' . $data . '/view';

    return $text;
  }
}
