<?php

/**
 * @file
 * Hooks implemented on behalf of the Automatic Nodetitles module to integrate
 * with Entity Xliff.
 */


if (!function_exists('auto_nodetitle_entity_xliff_translatable_fields_alter')) {

  /**
   * Implements hook_entity_xliff_translatable_fields_alter().
   */
  function auto_nodetitle_entity_xliff_translatable_fields_alter(&$fields, $wrapper) {
    // If this is a node and automatic node titles is enabled...
    if ($wrapper->type() === 'node' && auto_nodetitle_get_setting($wrapper->getBundle()) == AUTO_NODETITLE_ENABLED) {
      // Do not allow the title to be translatable.
      $key = array_search('title', $fields);
      if ($key !== FALSE) {
        unset($fields[$key]);
      }
    }
  }

}
