<?php
/**
 * @file
 * Hooks for arte_opa_broadcasts module.
 */

/**
 * This hook is invoked after broadcast data has been fetched from OPA.
 *
 * @param  Array $broadcasts multilingual broadcasts with program data keyed by
 *                           program ID.
 */
function hook_arte_opa_broadcasts_sync($broadcasts) {
  foreach ($broadcasts as $program_id => $data) {
    // Do something with the broadcast data.
  }
}

/**
 * Allow modules to alter the OPA query used by 'drush opa-broadcasts-sync'.
 *
 * @param array &$parameters
 *   An associative array of all the query filters.
 */
function hook_arte_opa_broadcasts_sync_params(&$parameters) {
  // Add some specific filters
  $parameters['programType'] = 'INTERNET';
}
