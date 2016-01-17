<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class things_handler_edit_link_field extends things_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy things to check access against
    $dummy_things = (object) array('type' => $type);
    if (!things_access('edit', $dummy_things)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $things_id = $values->{$this->aliases['things_id']};
    
    return l($text, 'admin/content/things/things/' . $things_id . '/edit');
  }
}
