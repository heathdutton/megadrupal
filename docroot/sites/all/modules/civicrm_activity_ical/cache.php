<?php
/**
 * @file
 * Cache functions for CiviCRM activity iCalendar feed module
 */

/**
 * Cache object for cached iCalendar feed data. Caching is per CiviCRM contact,
 * as defined by the submitted Contact ID.
 */
class CivicrmActivityIcalCache {

  /**
   * Gets cached data for the given contact.
   *
   * @param $contact_id
   *   The CiviCRM contact ID.
   *
   * @return
   *   FALSE if no valid cache is found, or if found, an array of cached
   *   data, with these elements:
   *   - cache: Text body of the cached feed.
   *   - cached: Unix timestamp indicating when cache was stored.
   */
  static function get($contact_id) {
    $min_timestamp = time() - (variable_get('civicrm_activity_ical_cache_interval', 0) * 60);
    $result = db_select('civicrm_activity_ical', 'c')
      ->fields('c', array('cache', 'cached'))
      ->condition('cid', $contact_id)
      ->condition('cached', $min_timestamp, '>')
      ->execute();
    return $result->fetchAssoc();
  }

  /**
   * Stores data in cache.
   *
   * @param $contact_id
   *   The CiviCRM contact ID.
   * @param $data
   *   Text body of the feed, to be stored.
   *
   * @return
   *   Return value of db_merge() when setting the cache data.
   */
  static function set($contact_id, $data) {
    $result = db_merge('civicrm_activity_ical')
      ->key(array('cid' => $contact_id))
      ->fields(
        array(
          'cid' => $contact_id,
          'cache' => $data,
          'cached' => time(),
        )
      )
      ->execute();
    
    return $result;
  }

  /**
   * Clears expired cache for the given user
   *
   * @param $contact_id
   *   The CiviCRM contact ID.
   * @param $force
   *   If TRUE, clear the cache for this user even if it's not expired.
   *   (default FALSE)
   *
   * @return
   *   Return value of db_update() when clearing the cache.
   */
  static function clear($contact_id, $force = FALSE) {
    $conditions = db_and();
    $conditions->condition('cid', $contact_id);
    if (!$force) {
      $timestamp = time() - (variable_get('civicrm_activity_ical_cache_interval', 0) * 60);
      $conditions->condition('cached', $timestamp, '>');
    }
    $result = db_update('civicrm_activity_ical')
      ->fields(
        array(
          'cache' => NULL,
          'cached' => NULL,
        )
      )
      ->condition($conditions)
      ->execute();
    return $result;
  }
}