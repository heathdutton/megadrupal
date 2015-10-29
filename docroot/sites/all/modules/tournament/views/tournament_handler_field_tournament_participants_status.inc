<?php

/**
 * @file
 * Views handler for Tournament module
 */

class tournament_handler_field_tournament_participants_status extends views_handler_field {
  function render($values) {
    if (!empty($values->{$this->field_alias})) {
      return t('Active');
    }

    return t('Not active');
  }
}
