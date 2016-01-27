<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class pollim_handler_edit_link_field extends pollim_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy pollim to check access against
    $dummy_pollim = (object) array('type' => $type);
    if (!pollim_access('edit', $dummy_pollim)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $pollim_id = $values->{$this->aliases['pollim_id']};
    
    return l($text, 'admin/content/pollims/pollim/' . $pollim_id . '/edit');
  }
}
