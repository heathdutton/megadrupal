<?php

/**
 * @file
 * Integer plugin.
 *
 * This plugin format data so that it can be stored in fields of type integer.
 * In fact, it replace all non-numeric characters by '0'.
 */


/**
 * Callback 'value' for Integer plugin.
 */
function opac_plugin_integer($record, $mapping) {
  $field = array('und' => array());
  $remotefield = $mapping['mapped_with'];
  if (isset($record[$remotefield])) {
    foreach ($record[$remotefield] as $value) {
      if (preg_match("/[0-9]/", $value)) {
        $value = preg_replace('/[^0-9]/', '0', $value);
        $field['und'][] = array(
          'value' => $value,
        );
      }
    }
  }
  return $field;
}

/**
 * Callback 'available' for Integer plugin.
 */
function opac_plugin_integer_available($server, $field) {
  if (isset($field['columns']['value'])) {
    return TRUE;
  }
  return FALSE;
}
