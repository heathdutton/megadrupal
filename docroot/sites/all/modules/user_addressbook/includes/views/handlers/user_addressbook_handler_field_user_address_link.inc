<?php

/**
 * Field handler to present a link to a customer profile.
 */
class user_addressbook_handler_field_user_address_link extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['address_id'] = 'address_id';
    $this->additional_fields['uid'] = 'uid';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $user_address = user_address_load($this->get_value($values, 'address_id'));
    $user_address_uri = entity_uri('user_address', $user_address);

    return l($text, $user_address_uri['path']);
  }
}
