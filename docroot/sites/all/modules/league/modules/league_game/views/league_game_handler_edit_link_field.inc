<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class league_game_handler_edit_link_field extends league_game_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy league_game to check access against
    $dummy_league_game = (object) array('type' => $type);
    if (!league_game_access('edit', $dummy_league_game)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $league_game_id = $values->{$this->aliases['league_game_id']};
    
    return l($text, 'league_game/' . $league_game_id . '/edit');
  }
}
