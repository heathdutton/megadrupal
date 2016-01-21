<?php

/**
 * @file
 * Class kaltura_views_handler_filter_is_comment.
 */

/**
 * Is Comment filter handler class.
 *
 * We want to supply our set of options on which we will filter results.
 */
class kaltura_views_handler_filter_is_comment extends views_handler_filter_in_operator {

  /**
   * We use only one function to supply our own set of values.
   */
  function get_value_options() {
    $this->value_options = array(0 => t('No'), 1 => t('Yes'));
  }
}
