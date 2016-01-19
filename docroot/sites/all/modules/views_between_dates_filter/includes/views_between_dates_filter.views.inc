<?php
/**
 * @file
 * Defines Views Between dates filter
 *
 */

/**
 * Implements hook_views_data()
 */
function views_between_dates_filter_views_data() {
  $data = array();

  $tables = date_views_base_tables();

  foreach ($tables as $base_table => $entity) {
    // The flexible date filter.
    $data[$base_table]['between_date_filter'] = array(
      'group' => t('Date'),
      'title' => t('Between Dates (!base_table)', array('!base_table' => $base_table)),
      'help' => t('Filter between two Views !base_table date fields.', array('!base_table' => $base_table)),
      'filter' => array(
        'handler' => 'views_between_dates_filter_handler',
        'empty field name' => t('Undated'),
        'is date' => TRUE,
      ),
    );
  }

  return $data;
}
