<?php

/**
 * Filter by item state
 */
class queue_runner_handler_filter_state extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('State');

      $this->value_options = array(
        QUEUERUNNER_STATE_WAITING => t('Waiting'),
        QUEUERUNNER_STATE_CLAIMED => t('Claimed'),
        QUEUERUNNER_STATE_RETRYING => t('Retrying'),
        QUEUERUNNER_STATE_FAILED => t('Failed'),
      );
    }
  }
}

