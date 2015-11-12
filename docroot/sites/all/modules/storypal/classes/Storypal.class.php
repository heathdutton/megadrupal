<?php
/**
 * @file
 * This class is just for Drupal.
 * It has been done to keep Storify.class.php clean from
 * Drupal functions, so it can be improved and use in
 * another project(s).
 *
 * @since  1.0
 *
 * @author Pol Dell'Aiera ( drupol | drupol@about.me )
 */

require_once(libraries_get_path('storify') . '/Storify/Main/Storify.php');
use Storify\Main\Storify;

class Storypal extends Storify {

  /**
   * Return the 'field_storify' name of the node type.
   *
   * @param string $type
   *   The node type.
   *
   * @return bool|string
   *   False if it is not found, the name of the field if found.
   */
  function getFieldName($type) {
    $fields = field_info_instances('node', $type);
    foreach ($fields as $field_name => $field_data) {
      $field_info = field_info_field_by_id($field_data['field_id']);
      if ($field_info['type'] == 'field_storify') {
        return $field_name;
        break;
      }
    }

    return FALSE;
  }

  /**
   * Get the URL while using Drupal's cache system.
   *
   * @param string $url
   *   The URL to get.
   *
   * @return bool|string
   *   Return false or the data fetched.
   */
  function query($url) {
    $data = drupal_http_request($url);
    if (isset($data->data)) {
      return $this->cache($data->data, $url);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Use the Drupal's cache system.
   *
   * @param mixed $data
   *   The data to get from cache.
   * @param string $identifier
   *   The unique identifier of the data.
   *
   * @return mixed
   *   Return the data.
   */
  function cache($data, $identifier) {
    if ($cache = cache_get(sha1($identifier))) {
      $data = $cache->data;
    }
    else {
      cache_set(sha1($identifier), $data, 'cache', CACHE_TEMPORARY);
    }

    return $data;
  }
}
