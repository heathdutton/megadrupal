<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class invites_handler_delete_link_field extends invites_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy invites to check access against
    $dummy_invites = (object) array('type' => $type);
    if (!invites_access('edit', $dummy_invites)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $invites_id = $values->{$this->aliases['invites_id']};
    
    return l($text, 'admin/content/invites/invites/' . $invites_id . '/delete');
  }
}
