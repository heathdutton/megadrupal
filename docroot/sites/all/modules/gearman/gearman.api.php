<?php


/**
 * @file
 * API documentation file for Gearman integration module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Declare worker functions so that a drush-based worker can know about them
 * automatically.
 *
 * @return an associative array with the data for GearmanWorker::addFunction.
 *
 * This array has the following parameters
 *
 * - 'function_name': The name of a function to register with the job server
 * - 'function': A callback that gets called when a job for the registered
 *               function name is submitted
 * - 'context': A reference to arbitrary application context data that can be
 *              modified by the worker function
 * - 'timeout': An interval of time in seconds
 *
 */
function hook_gearman_drush_function() {
  return array(
    array(
      'function_name' => 'reverse',
      'function' => 'gearman_reverse_fn',
      'context' => array(),
      'timeout' => 60,
    ),
  );
}

/**
 * Alter Gearmanized cron queue item before processing.
 *
 * This hook is invoked by gearman_cron_queue_fn(). When a cron queue item is
 * submitted to Gearman, it is encoded in JSON. The item is decoded back from
 * JSON when received by a Gearman worker. This will turn associative arrays to
 * objects and break processing code expecting arrays. This hook can be used
 * to fix decoded items and cast objects to arrays where required.
 *
 * @param $item The queue item received by the Gearman worker.
 * @param array $queue_info Description of the queue the item belongs to.
 */
function hook_gearman_cron_queue_item_alter(&$item, $queue_info) {

}

/**
 * @} End of "addtogroup hooks".
 */