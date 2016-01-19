<?php
/**
 * @file
 * supercookie.api.php
 */

/**
 * The items returned by this hook will be stored in the serialized
 * supercookie.custom field and keyed by module name.
 */
function hook_supercookie_custom() {

  $items = array();

  $items['var_1'] = TRUE;
  $items['var_2'] = 'foo';
  $items['var_3'] = 'bar';

  return $items;
}
