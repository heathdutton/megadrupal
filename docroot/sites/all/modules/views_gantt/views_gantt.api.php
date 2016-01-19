<?php

/**
 * @file
 * Hooks provided by the Views Gantt module.
 */


/**
 * Alter an array of tasks provided by Gantt view before other modifications.
 *
 * @param $tasks
 *  An array of tasks (array keys are ids of tasks nodes).
 */
function hook_views_gantt_tasks_prerender_alter(&$tasks) {

}


/**
 * @param $access
 * 	Boolean indicating whether user can update task/project.
 * @param $node
 * 	The node object on which the operation is to be performed.
 * @param $account
 * 	A user object representing the user for whom the operation is to be performed.
 */
function hook_views_gantt_update_access(&$access, $node, $account) {
	
}