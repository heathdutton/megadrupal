<?php

/**
 * @file
 *  Contains the main cron function.
 */

define('ENTITY_EXT_RATING_REQUESTS_BATCH_SIZE', variable_get('entity_ext_rating_requests_batch_size', 20));

/**
 * Helper function for the cron hook.
 */
function _entity_ext_rating_cron() {
  // Delete expired entries from the queue.
  db_delete('entity_ext_rating_queue')
    ->condition('expire', REQUEST_TIME, "<")
    ->condition('expire', 0, '>')
    ->execute();
  // Check if we have to fetch the ratings.
  $last_rating_check = variable_get('entity_ext_rating_last_rating_check', 0);
  $rating_check_timeframe = variable_get('entity_ext_rating_check_timeframe', 1800);
  // If we have to check for new ratings, do it.
  if ($last_rating_check < REQUEST_TIME - $rating_check_timeframe) {
    // VERY IMPORTANT, flag that we made the check, or we are already in a check
    // process.
    variable_set('entity_ext_rating_last_rating_check', REQUEST_TIME);
    // Get all the enabled plugins.
    $enabled_plugins = variable_get('entity_ext_rating_active_plugins', array());
    foreach ($enabled_plugins as $key => $enabled_plugin) {
      // For each enabled plugin, get an instance of the class.
      if ($enabled_plugin) {
        $plugin_objects[$key] = entity_ext_rating_get_plugin($key);
      }
    }
    // For all rows in the queue, check for new ratings.
    // @todo: if the queue grows too much, maybe put a limit there, and use
    // something like a last_checked timestamp field for sorting.
    $result = db_select('entity_ext_rating_queue', 'queue')
      ->fields('queue')
      ->execute();
    $rows = array();
    foreach ($result as $row) {
      $row = (array)$row;
      $row['params'] = unserialize($row['params']);
      $rows[$row['rating_id']] = $row;
    }
    foreach ($plugin_objects as $plugin) {
      // if the plugin can retrieve multiple ratings at a time, request up to
      // ENTITY_EXT_RATING_REQUESTS_BATCH_SIZE URLs at a time.
      $count = 0;
      $count = ceil(count($rows) / ENTITY_EXT_RATING_REQUESTS_BATCH_SIZE);
      for ($i = 0; $i < $count; $i++) {
        $params = array_slice($rows, $i*ENTITY_EXT_RATING_REQUESTS_BATCH_SIZE, ENTITY_EXT_RATING_REQUESTS_BATCH_SIZE);
        $plugin->saveExternalRating($params);
      }
    }
    // Last step, aggregate the results, to have the total rating.
    $query = db_select('entity_ext_rating_values', 'rating_values')
            ->fields('rating_values', array('rating_id', 'entity_id', 'entity_type'));
    // Currently, the total rating is the sum of all ratings. Maybe in the
    // future we will need to have a more customized formula... but for the
    // moment, that's just fine.
    $query->addExpression('SUM(rating_values.rating)', 'rating_total');
    $query->groupBy('rating_id');
    $result = $query->execute();
    foreach ($result as $row) {
      // Update the old rate.
      // @todo: check if db_merge would be faster than the
      // (db_delete, db_insert) combination.
      db_delete('entity_ext_rating_total')
        ->condition('rating_id', $row->rating_id)->execute();
      db_insert('entity_ext_rating_total')
              ->fields(array(
                  'rating_id' => $row->rating_id,
                  'entity_id' => $row->entity_id,
                  'entity_type' => $row->entity_type,
                  'rating' => $row->rating_total,
              ))
              ->execute();
      $rows[$row->rating_id]['rating_total'] = $row->rating_total;
    }
    module_invoke_all('entity_ext_rating_updates', $rows);
  }
}