<?php

/**
 * @file
 * Views handler for Match module
 */

class team_handler_filter_member_role extends views_handler_filter_in_operator {
 function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Member role');
      $this->value_options = team_get_member_roles();
    }
  }
}
