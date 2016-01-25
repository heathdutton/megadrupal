<?php

/**
 * @file
 * Provide link to delete an address.
 */

class ec_address_handler_field_link_delete extends ec_address_handler_field_link_edit {
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $aid = $values->{$this->aliases['aid']};
    $uid = $values->{$this->aliases['uid']};
    return l($text, "user/$uid/address/$aid/delete");
  }
}
