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
class SmartlingHandlerFilterEntityLocale extends views_handler_filter_in_operator {

  /**
   * Get value options.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Smartling locales');
      $this->value_options = smartling_language_options_list();
    }
  }

}
