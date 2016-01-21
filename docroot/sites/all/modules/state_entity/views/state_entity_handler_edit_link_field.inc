<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class state_entity_handler_edit_link_field extends state_entity_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy state_entity to check access against
    $dummy_state_entity = (object) array('type' => $type);
    if (!state_entity_access('edit', $dummy_state_entity)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $state_entity_id = $values->{$this->aliases['state_entity_id']};
    
    return l($text, 'admin/content/state_entity/state_entity/' . $state_entity_id . '/edit');
  }
}
