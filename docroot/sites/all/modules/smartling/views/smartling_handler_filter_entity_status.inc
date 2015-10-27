<?php

/**
 * @file
 * Definition of smartling handler filter entity status.
 */

/**
 * Filter by smartling status.
 *
 * @ingroup views_filter_handlers
 */
class SmartlingHandlerFilterEntityStatus extends views_handler_filter_in_operator {

  /**
   * Get value options.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Smartling status');
      $options = array(
        0 => 'New',
        1 => 'In Progress',
        2 => 'Completed',
        3 => 'Changed',
        4 => 'Failed',
        5 => 'Pending Cancel',
        6 => 'Canceled',
      );
      $this->value_options = $options;
    }
  }

}
