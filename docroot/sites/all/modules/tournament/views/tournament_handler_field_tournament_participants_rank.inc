<?php

/**
 * @file
 * Views handler for Tournament module
 */

class tournament_handler_field_tournament_participants_rank extends views_handler_field {
  function render($values) {
    if (!empty($values->{$this->field_alias}) && !empty($values->node_type)) {
      $rank = module_invoke($values->node_type, 'tournament_participant_rank', $values->{$this->field_alias});
      return $rank;
    }

    // No rank set yet
    return '';
  }
}
