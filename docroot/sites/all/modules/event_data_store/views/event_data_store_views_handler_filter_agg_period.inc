<?php

/**
 * @file
 * Definition of event_data_store_views_handler_filter_agg_period.
 */

/**
 * Filter by node type.
 *
 * @ingroup views_filter_handlers
 */
class event_data_store_views_handler_filter_agg_period extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Aggregation interval');
      $this->value_options = array(
        EVENT_DATA_STORE_AGGREGATED_INTERVAL_HOURLY => t('Hourly'),
        EVENT_DATA_STORE_AGGREGATED_INTERVAL_DAILY => t('Daily'),
        EVENT_DATA_STORE_AGGREGATED_INTERVAL_WEEKLY => t('Weekly'),
        EVENT_DATA_STORE_AGGREGATED_INTERVAL_MONTHLY => t('Monthly'),
        EVENT_DATA_STORE_AGGREGATED_INTERVAL_YEARLY => t('Yearly'),
      );
    }
  }
}
