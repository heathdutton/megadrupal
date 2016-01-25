<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class things_ids_handler_edit_link_field extends things_ids_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy things_ids to check access against
    $dummy_things_ids = (object) array('type' => $type);
    if (!things_ids_access('edit', $dummy_things_ids)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $things_ids_id = $values->{$this->aliases['things_ids_id']};
    
    return l($text, 'admin/content/things_ids/things_ids/' . $things_ids_id . '/edit');
  }
}
