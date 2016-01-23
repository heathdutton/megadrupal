<?php

/**
 * Filter handler for title.
 */
class cd_sunlight_handler_filter_title extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = array(
        'Sen' => t('Senator'),
        'Rep' => t('Representative'),
        'Del' => t('Delegate'),
        'Com' => t('Commissioner'),
      );
    }
  }
}
