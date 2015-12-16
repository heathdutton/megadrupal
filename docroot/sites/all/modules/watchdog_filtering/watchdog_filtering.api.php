<?php

/**
 * @file
 * Hooks provided by the Watchdog Filtering module
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Filter watchdog log entry. Works similar to hook_node_access().
 *
 * @param array $log_entry
 *   The watchdog log entry
 */
function hook_watchdog_filtering(array $log_entry) {
  // Never watchdog page not found messages.
  if ($log_entry['type'] == 'page not found') {
    return WATCHDOG_FILTERING_EXCLUDE;
  }

  // "Always" watchdog php messages.
  if ($log_entry['type'] == 'php') {
    return WATCHDOG_FILTERING_INCLUDE;
  }

  // Don't affect filtering on other messages.
  return WATCHDOG_FILTERING_IGNORE;
}

