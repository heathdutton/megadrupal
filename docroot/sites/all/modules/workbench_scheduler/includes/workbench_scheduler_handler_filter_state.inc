<?php

/**
 * @file
 * Filter based on moderation state.
 */

class workbench_scheduler_handler_filter_state extends views_handler_filter_in_operator {
  /**
   * Return filter options for schedule states.
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Moderation status');
      $this->value_options = array_map('check_plain', workbench_scheduler_state_labels());
    }
  }
}
