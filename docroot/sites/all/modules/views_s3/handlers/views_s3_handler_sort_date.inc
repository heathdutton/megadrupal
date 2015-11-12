<?php
/**
 * @defgroup views_sort_handlers Views' sort handlers
 * @{
 * Handlers to tell Views how to sort queries
 */

/**
 * Base sort handler that has no options and performs a simple sort
 */
class views_s3_handler_sort_date extends views_handler_sort_date {

  /**
   * Called to add the sort to a query.
   */
  function query() {
    // Add the field.
    $params = array(
      'granularity' => $this->options['granularity'],
      'sorting_function' => function ($sorter_item, $params) {
        switch ($params['granularity']) {
          case 'second':
          default:
            return strtotime($sorter_item);
          case 'minute':
            return strtotime(date('Y-m-d\TH:i:00.000\Z', strtotime($sorter_item)));
          case 'hour':
            return strtotime(date('Y-m-d\TH:00:00.000\Z', strtotime($sorter_item)));
          case 'day':
            return strtotime(date('Y-m-d\T01:00:00.000\Z', strtotime($sorter_item)));
          case 'month':
            return strtotime(date('Y-m-01\T01:00:00.000\Z', strtotime($sorter_item)));
          case 'year':
            return strtotime(date('Y-01-01\T01:00:00.000\Z', strtotime($sorter_item)));
        }
      },
    );
    
    $this->query->add_orderby($this->table_alias, $this->real_field, $this->options['order'], '', $params);
  }
}


/**
 * @}
 */
