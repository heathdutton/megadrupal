<?php

/**
 * @file
 * substring plugin.
 *
 * This plugin allows to get a substring of incoming data.
 */

/**
 * Callback for substring plugin.
 */
function koha_connector_plugin_substring($from, $conf) {
  list($tag, $subtag) = array_pad(explode('$', $conf['value']), 2, NULL);
  $start = $conf['plugin_params']['start'];
  $length = $conf['plugin_params']['length'];

  $values = array();

  if (isset($from['datafields'][$tag])) {
    $fields = $from['datafields'][$tag];
    foreach ($fields as $field) {
      $subfields = $field['subfields'];
      foreach ($subfields as $subfield) {
        // If $subtag is NULL, all subfields are used to build $values.
        $subfieldtag = isset($subtag) ? $subtag : array_shift(array_keys($subfield));
        if (array_key_exists($subfieldtag, $subfield)) {
          $value = $subfield[$subfieldtag];
          $value = substr($value, $start, $length);
          $values[] = $value;
        }
      }
    }
  }

  return $values;
}
