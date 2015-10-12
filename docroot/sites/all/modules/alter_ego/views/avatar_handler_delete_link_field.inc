<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class avatar_handler_delete_link_field extends avatar_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy mmotoon to check access against
    $dummy_mmotoon = (object) array('type' => $type);
    if (!alter_ego_access('edit', $dummy_mmotoon)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $aid = $values->{$this->aliases['aid']};
    
    return l($text, 'avatar/' . $aid . '/delete');
  }
}
