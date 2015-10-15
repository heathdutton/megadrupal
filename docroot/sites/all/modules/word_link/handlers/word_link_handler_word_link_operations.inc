<?php

/**
 * @file
 * Definition of word_link_handler_word_link_operations.
 */

/**
 * Field handler to present an operations for editing/deleting a word.
 *
 * @ingroup views_field_handlers
 */
class word_link_handler_word_link_operations extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  function access() {
    return user_access('edit word link');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $id = $this->get_value($values, 'id');
    $id = $this->sanitize_value($id);

    $items = array(
      l(t('edit'), 'admin/config/content/word-link/edit/' . $id),
      l(t('delete'), 'admin/config/content/word-link/delete/' . $id),
    );
    $output = theme('item_list', array('items' => $items, 'attributes' => array('class' => array('links', 'inline'))));

    return $output;
  }
}
