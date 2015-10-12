<?php

/**
 * Field handler to present an user address edit link.
 */
class user_addressbook_handler_field_user_address_link_edit extends user_addressbook_handler_field_user_address_link {

  function render($values) {
    // Ensure the user has access to edit this user address.
    $user_address = user_address_load($this->get_value($values, 'address_id'));
    $user_address_uri = entity_uri('user_address', $user_address);

    if (!user_address_access('edit', $user_address)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');

    return l($text, $user_address_uri['edit_path'], array('query' => drupal_get_destination()));
  }
}
