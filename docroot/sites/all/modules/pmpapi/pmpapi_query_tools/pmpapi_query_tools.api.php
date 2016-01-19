<?php

/**
 * @file
 * Hooks provided by the PMPAPI query tools module.
 */

/**
 * Act on a PMP query before it is executed.
 *
 * @param array $options
 *   Key/value pairs of PMP query options.
 *
 * @param string $query
 *   Machine name of the query.
 */
function hook_pmpapi_query_tools_query_pre_execute_alter(&$options, $query) {
  if (!$options['limit']) {
    // Let's throttle!
    $options['limit'] = 1;
  }
}

/**
 * Respond to a query being deleted.
 *
 * @param string $query
 *   Machine name of the query.
 */
function hook_pmpapi_query_tools_query_delete($query) {
  variable_del('my_module_added_variable_' . $query);
}