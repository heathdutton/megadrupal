<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class league_person_handler_delete_link_field extends league_person_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy league_person to check access against
    $dummy_league_person = (object) array('type' => $type);
    if (!league_person_access('edit', $dummy_league_person)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $league_person_id = $values->{$this->aliases['league_person_id']};
    
    return l($text, 'league_person/' . $league_person_id . '/delete');
  }
}
