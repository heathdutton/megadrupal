<?php

/**
 * @file
 * Hooks defined by the Variable log module.
 */

/**
 * Allows cancel saving a log to database.
 *
 * @param bool $updated
 *   TRUE if variable was updated and need save log to database,
 *   otherwise FALSE.
 * @param array $context
 *   An associative array containing:
 *   - name: A variable name.
 *   - value: A new value of variable.
 *   - old_value: An old value of variable.
 *   - options: An options ov variable.
 */
function hook_variable_log_variable_update_alter(&$updated, $context) {
  // If updated variable name is "site_name", then no save log to database.
  if ($context['name'] == 'site_name') {
    $updated = FALSE;
  }
}
