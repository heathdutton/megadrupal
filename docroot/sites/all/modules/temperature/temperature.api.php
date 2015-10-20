<?php
/**
 * @file
 * Example API functions
 *
 * @ingroup temperature Temperature Field
 * @{
 */

/**
 * Implements hook_temperature_units_alter
 *
 * @param array &$units
 *   The units definitions.
 *
 * @see temperature_units()
 */
function hook_temperature_units_alter(&$units) {

  //expand the suffix to include the whole word
  $units['f']['suffix'] = html_entity_decode('&#176; Fahrenheit', ENT_NOQUOTES, 'UTF-8');
}


/** @} */ //end of group temperature
