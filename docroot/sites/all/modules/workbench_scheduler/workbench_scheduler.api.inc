<?php
/**
 * @file
 * workbench_scheduler.api.inc
 *
 * Defines workbench_scheduler hooks that can be used by other modules.
 */

/**
 * Perform action after saving a node schedule.
 *
 * This hook is invoked from workbench_scheduler_save_node_schedule() after a
 * schedule has been updated for a node.
 *
 * @param int $nid
 *   The node id.
 * @param int $vid
 *   The revision id.
 * @param array $schedule_data
 *   An associative array of schedule information
 *   The associative array contains the following data:
 *    - sid: the schedule id
 *    - name: the unique machine name for the schedule
 *    - start_state: the name of the starting moderation state (optional)
 *    - end_state: the name of the ending moderation state (optional)
 *    - start_date: the timestamp when to apply the start state
 *    - end_date: the timestamp when to apply the end date.
 *
 * @ingroup workbench_scheduler_hooks
 * @ingroup hooks
 */
function hook_workbench_scheduler_post_save_node_schedule($nid, $vid, $schedule_data) {
  // Load the node.
  $node = node_load($nid, $vid);
  // Set a watchdog log message.
  watchdog('workbench_scheduler', 'Schedule has been saved for @title.', array('@title' => $node->title), WATCHDOG_NOTICE);
}

/**
 * Perform action after deleting a node schedule.
 *
 * This hook is invoked from workbench_scheduler_delete_node_schedule() after a
 * schedule has been deleted form a node.
 *
 * @param int $nid
 *   The node id.
 * @param int $vid
 *   The revision id.
 * @param int $sid
 *   The schedule sid.
 *
 * @ingroup workbench_scheduler_hook
 * @ingroup hooks
 */
function hook_workbench_scheduler_post_delete_node_schedule($nid, $vid, $sid) {
  // Load the node.
  $node = node_load($nid, $vid);
  // Set a watchdog log message.
  watchdog('workbench_scheduler', 'Schedule has been deleted form @title.', array('@title' => $node->title), WATCHDOG_NOTICE);
}

/**
 * Perform action after starting schedules for nodes.
 *
 * This hook is invoked from workbench_scheduler_process_start_dates() after
 * starting schedules for nodes.
 *
 * @param array $scheduled_nodes
 *   An array of node schedule data, each element in the array is an object
 *   containing the following properties:
 *     - nid: The node nid
 *     - vid: The node revision id
 *     - sid: The schedule id
 *     - start_state: The state the node was set to.
 *
 * @ingroup workbench_scheduler_hooks
 * @ingroup hooks
 */
function hook_workbench_scheduler_post_process_start_dates(array $scheduled_nodes) {
  // Set a watchdog log message.
  watchdog('workbench_scheduler', '@count nodes had their schedules started.', array('@count' => count($scheduled_nodes)), WATCHDOG_NOTICE);
}

/**
 * Perform action after ending schedules for nodes.
 *
 * This hook is invoked from workbench_scheduler_process_end_dates() after
 * ending schedules for nodes.
 *
 * @param array $scheduled_nodes
 *   An array of node schedule data, each element in the array is an object
 *   containing the following properties:
 *     - nid: The node nid
 *     - vid: The node revision id
 *     - sid: The schedule id
 *     - end_state: The state the node was set to.
 *
 * @ingroup workbench_scheduler_hooks
 * @ingroup hooks
 */
function hook_workbench_scheduler_post_process_end_dates(array $scheduled_nodes) {
  // Set a watchdog log message.
  watchdog('workbench_scheduler', '@count nodes had their schedules ended.', array('@count' => count($scheduled_nodes)), WATCHDOG_NOTICE);
}
