<?php
/**
 * @file
 * Functions which should be in core somewhere
 */

$plugin = array (
  'title' => t('Core'),
  'description' => t('Library of functions which enhance core.'),
  'xu_type' => 'library'
);

/**
 * Get info for modules.
 *
 * This is based on context_get_info()
 *
 * @param null $type
 * @param null $name
 * @param bool $reset
 *
 * @return bool
 */
function xu_module_get_info($type = NULL, $name = NULL, $reset = FALSE) {
  static $info;
  if (!isset($info) || $reset) {
    $result = db_query("SELECT name,type,info FROM {system}");
    while ($row = db_fetch_object($result)) {
      $info[$row->type][$row->name] = unserialize($row->info);
    }
  }
  if (isset($type, $name)) {
    return isset($info[$type][$name]) ? $info[$type][$name] : FALSE;
  }
  else if (isset($type)) {
    return isset($info[$type]) ? $info[$type] : FALSE;
  }
  return $info;
}
