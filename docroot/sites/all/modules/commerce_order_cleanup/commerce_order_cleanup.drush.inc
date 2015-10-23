<?php

/**
 * @file
 * Drush integration for the Commerce order cleanup module.
 */

/**
 * Implements hook_drush_help().
 */
function commerce_order_cleanup_drush_help($section) {
  switch ($section) {
    case 'commerce-order-cleanup-delete-orders':
      return dt("This command will delete all commerce orders by various order properties like status, last changed.");
  }
}

/**
 * Implements hook_drush_command().
 */
function commerce_order_cleanup_drush_command() {
  $items = array();
  $date_format = 'd-m-Y h:i:s';
  $items['commerce-order-cleanup-delete-orders'] = array(
    'callback' => 'drush_commerce_order_cleanup_delete_orders',
    'description' => dt('Deletes commerce orders by various order properties like status, last changed.'),
    'arguments' => array(
      'type' =>  dt('Type of line item in an order.'),
    ),
    'options' => array(
      'status' =>  dt('Status of an order. try drush cco --list-options="status" to get available status'),
      'changed' => dt('Order changed timestamp. Use timestamp or time string e.g. "-2 months". '),
    ),
    'examples' => array(
      'drush coc product --status=checkout_complete --changed="-2 months"' => dt("List all the orders have line item of type product with status 'checkout_complete' that are changed on or before 2 months. Currently you can delete the orders changed between @start_date and @end_date.", array('@start_date' => date($date_format, strtotime('October 15, 2009 9:41pm')), '@end_date' =>  date($date_format, strtotime('-2 months')))),
      'drush coc product --status=checkout_complete --changed="-2 months" -y' => dt("Delete all the orders have line item of type product with status 'checkout_complete' that are changed on or before 2 months. Currently you can delete the orders changed between @start_date and @end_date.", array('@start_date' => date($date_format, strtotime('October 15, 2009 9:41pm')), '@end_date' =>  date($date_format, strtotime('-2 months')))),
    ),
    'aliases' => array('coc'),
  );

  $items['commerce-order-cleanup-list-options'] = array(
    'description' => dt('Lists values available for given order property.'),
    'arguments' => array(
      'name' =>  dt('Property name. e.g. status'),
     ),
    'examples' => array(
      'drush cocl status' => "Provides list of status available.",
    ),
    'aliases' => array('cocl'),
  );

  return $items;
}

/**
 * Deletes orders.
 *
 * @param integer $interval
 *   Duration the orders must have been in an unmodified unfinished state to be
 *   cleaned, in seconds.
 */
function drush_commerce_order_cleanup_delete_orders($type) {
  $status = drush_get_option('status', NULL);
  $changed_raw = drush_get_option('changed', NULL);
  if (empty($type) || empty($status) || empty($changed_raw)) {
   return drush_set_error(dt('Invalid value. Please provide valid type, status and changed values.'));
  }

  // Make sure max date set for this filter.
  $max_variable = _commerce_order_cleanup_get_variable('max', $type, $status);
  if (!variable_get($max_variable, FALSE)) {
    return drush_set_error(dt('Please set the allowed date limit for orders with line item type "!type" and status "!status". Try "drush vget commerce_order_cleanup_max" for list of status set.', array('!type' => $type, '!status' => $status)));
  }

  // Date validation.
  $changed = _commerce_order_cleanup_calculate_changed($changed_raw, $type, $status);
  if ($changed == NULL) {
    $ts_range = _commerce_order_cleanup_get_timestamp_range($type, $status);
    $params = array(
      '!start_date' => date(DATE_ATOM, $ts_range['start']),
      '!end_date' => date(DATE_ATOM, $ts_range['end'])
    );
    return drush_set_error(dt('Invalid changed value. Make sure the changed date is between !start_date and !end_date. set commerce_order_cleanup_min_date and/or commerce_order_cleanup_max_date varailbes to change the range.', $params));
  }

  /** @var SelectQuery $query */
  $query = db_select('commerce_order', 'co')
    ->fields('co', array('order_id'))
    ->condition('ci.type', $type)
    ->condition('co.status', $status)
    ->condition('co.changed', $changed , '<');
  $query->join('commerce_line_item', 'ci', 'co.order_id = ci.order_id');

  $result = $query->execute();
  foreach ($result as $row) {
    $order_ids[] = $row->order_id;
  }
  if (empty($order_ids)) {
    return drush_set_error(dt('No orders to delete for given search. Generated query is: !query', array('!query' => strtr((string) $query, $query->arguments()))));
  }

  // Display the state of the records before delete.
  drush_commerce_order_cleanup_display_state($type, $status, $changed);

  // Confirm delete.
  drush_print(dt("Found !count records to delete.", array('!count' => count($order_ids))));

  // Just list, if no '-y'.
  // Trying to avoid drush_confirm as it can allow to select wrong option on a
  // high volume order base.
  $confirm = drush_get_option('yes');

  if ($confirm) {
    return _commerce_order_cleanup_delete_orders($order_ids);
  }
  return ;

}

/**
 * @param $order_ids
 */
function _commerce_order_cleanup_delete_orders($order_ids) {
  $total_count = count($order_ids);

  // get the batch size from config.
  $batch_size = variable_get('commerce_order_cleanup_batch_size', 100);

  // chunk the nids, instead of foreach.
  $batches = array_chunk($order_ids, $batch_size);

  $operations = array();
  foreach ($batches as $nids) {
    $operations[] = array('commerce_order_cleanup_delete_multiple', array($nids));
  }
  $_SESSION['commerce_order_deleted'] = 0;
  $_SESSION['commerce_order_total'] = $total_count;

  $batch = array(
    'operations' => $operations,
    'error_message' => t('Error while trying to delete your order. Please try again (BATCH_INIT_FAIL).'),
    'finished' => 'commerce_order_cleanup_delete_finished',
    'file' => drupal_get_path('module', 'commerce_order_cleanup') . '/commerce_order_cleanup.admin.inc',
  );

  $batch['progressive'] = FALSE;
  batch_set($batch);
  drush_backend_batch_process();
}

/**
 * provides list of values available for given property.
 *
 * @param $property
 */
function drush_commerce_order_cleanup_list_options($property) {
  // For now, only property that has this option is 'status'.
  $allowed_properties = array('status');
  if (!empty($property)) {
    if (in_array($property, $allowed_properties)) {
      $list = _commerce_order_cleanup_get_options($property);
      drush_print_table($list, TRUE);
      return;
    }
    else {
      drush_print(dt('There is no option available for given order property.'), 'error');
      return;
    }
  }
}

/**
 * Provides current status of orders in the system.
 *
 * @return mixed
 */
function drush_commerce_order_cleanup_display_state($type, $status, $changed) {

  /** @var SelectQuery $query */
  $query = db_select('commerce_order', 'co')
    ->fields('co', array('status'))
    ->fields('ci', array('type'))
    ->condition('ci.type', $type)
    ->condition('co.status', $status)
    ->condition('co.changed', $changed , '<');
  $query->addExpression('COUNT(1)', 'count');
  $query->join('commerce_line_item', 'ci', 'co.order_id = ci.order_id');
  $query->groupBy('co.status');

  $result = $query->execute();
  foreach ($result as $row) {
    $status_list[] = array($row->status, $row->type, $row->count);
  }
  if (!empty($status_list)) {
    array_unshift($status_list, array('Status', 'Line item type', 'Count'));
    drush_print_table($status_list, TRUE);
  }
  else {
    return drush_set_error('No orders to list for given search. sql is: ' . $query);
  }
}

/**
 * Helper to calculate changed timestamp.
 *
 * @param int|string $changed
 *   A integer timestamp or date string.
 * @param string $date_variable
 *
 * @return int|null
 *   timestamp if valid, NULL otherwise.
 */
function _commerce_order_cleanup_calculate_changed($changed, $type, $status) {
  if (is_int($changed) && _commerce_order_cleanup_is_valid_date($changed, $type, $status)) {
    return $changed;
  }
  elseif (is_string($changed) && _commerce_order_cleanup_is_valid_date(strtotime($changed), $type, $status)) {
    return strtotime($changed);
  }
  return NULL;
}

/**
 * Validates given date range is valid.
 *
 * @param $timestamp
 * @param string $date_variable
 *
 * @return bool
 */
function _commerce_order_cleanup_is_valid_date($timestamp, $type, $status) {
  // if it is timestamp, make sure it is in a range.
  // Start of commerce module in drupal.org
  $date_range = _commerce_order_cleanup_get_timestamp_range($type, $status);
  if ($timestamp < $date_range['start'] || $timestamp > $date_range['end']) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Helper to provide date range (in timestamp).
 *
 * @param string $date_variable
 *
 * @return array
 */
function _commerce_order_cleanup_get_timestamp_range($type, $status) {
  $min_var = _commerce_order_cleanup_get_variable('min', $type, $status);
  $max_var = _commerce_order_cleanup_get_variable('max', $type, $status);
  $start_var =  variable_get($min_var);
  $start = $start_var ? '-' . $start_var : 'October 15, 2009 9:41pm';
  $date_range['start'] =  strtotime($start);
  // Max range has no fallback as it never comes to this stage, if it is not set.
  $date_range['end'] = strtotime('-' . variable_get($max_var));
  return $date_range;
}

function _commerce_order_cleanup_get_variable($range_type, $type, $status) {
  return format_string('commerce_order_cleanup_!range_!type_!status', array('!range' => $range_type, '!type' => $type, '!status' => $status));
}
