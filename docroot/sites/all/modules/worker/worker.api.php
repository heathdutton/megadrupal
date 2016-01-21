<?php
/**
 * @file
 * Document worker hooks.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Preload any files that may be required for a job.
 */
function hook_worker_preload() {
  require_once drupal_get_path('module', 'my_module') . '/my_module.inc';
}

/**
 * @} End of "addtogroup hooks".
 */
