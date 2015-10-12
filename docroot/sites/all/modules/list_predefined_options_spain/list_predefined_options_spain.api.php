<?php

/**
 * @file
 * Hooks provided by the List predefined options spain module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the predefined list of provinces.
 *
 * @param array $options
 *   The associative array of predefined list option values.
 */
function hook_list_predefined_options_spain_provinces_alter(array &$options) {
  // Change a specific value.
  if (!empty($options['ES-GI'])) {
    $options['ES-GI'] = 'Girona';
  }
}

/**
 * Alter the predefined list of states (comunidades autonomas).
 *
 * @param array $options
 *   The associative array of predefined list option values.
 */
function hook_list_predefined_options_spain_ccaa_alter(array &$options) {
  // Change a specific value.
  if (!empty($options['ES-MD'])) {
    $options['ES-MD'] = 'Madrid';
  }
}

/**
 * @} End of "addtogroup hooks".
 */
