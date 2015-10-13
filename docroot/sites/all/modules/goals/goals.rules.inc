<?php
/**
 * @file
 * Rules implementations for Goals Module.
 */

/**
 * Implements hook_rules_action_info().
 */
function goals_rules_action_info() {
  $actions['goals_task_record'] = array(
    'label' => t('Record a task for Goals'),
    'base' => 'goals_rules_record_task',
    'group' => 'Goal',
    'type' => 'user',
    'save' => TRUE,
    'parameter' => array(
      'task_name' => array(
        'type' => 'text',
        'label' => 'Task to record',
        'restriction' => 'input',
        'options list' => 'goals_rules_action_task_name_list',

      ),
      'user_to_save_for' => array(
        'type' => 'text',
        'label' => 'Record for user',
        'restriction' => 'input',
        'rows' => 1,
        'description' => t('User name of user for which to record activity.'),
      ),
      'task_id' => array(
        'type' => 'text',
        'label' => 'ID Related to this task',
        'restriction' => 'input',
        'optional' => TRUE,
        'description' => t('Optional additional ID to record related to this task (e.g. a node, user, etc. being acted upon). Useful when needing to delete tasks, if necessary.'),
      ),
    ),
  );
  $actions['goals_task_remove'] = array(
    'label' => t('Remove a task for Goals'),
    'base' => 'goals_rules_remove_task',
    'group' => 'Goal',
    'type' => 'user',
    'save' => TRUE,
    'parameter' => array(
      'task_name' => array(
        'type' => 'text',
        'label' => 'Task to remove',
        'restriction' => 'input',
        'options list' => 'goals_rules_action_task_name_list',
      ),
      'user_to_remove_for' => array(
        'type' => 'text',
        'label' => 'Remove for user',
        'restriction' => 'input',
        'description' => t('User name of user for which to remove activity.'),
      ),
      'task_id' => array(
        'type' => 'text',
        'label' => 'ID Related to this task',
        'restriction' => 'input',
        'optional' => TRUE,
        'description' => t('Optional additional ID related to this task (e.g. a node, user, etc. being acted upon). If no ID is provided, all tasks of this type for this user will be removed.'),
      ),
    ),
  );
  return $actions;
}

/**
 * Recording activity to the goals_activity table.
 *
 * @param string $task_type
 *   The task type to record.
 * @param string $user_to_save_for
 *   User Name user to record for.
 * @param string $external_id
 *   External ID to record with activity.
 */
function goals_rules_record_task($task_type, $user_to_save_for, $external_id) {
  // Attempt to convert user_name to UID.
  $uid = _goals_rules_username_to_uid($user_to_save_for);
  if ($uid) {
    $sql = db_insert('goals_activity')
        ->fields(array(
      'uid' => $uid,
      'task_type' => $task_type,
      'external_id' => $external_id,
      'created' => REQUEST_TIME,
    ))
        ->execute();
    goals_calculate_user($uid);
  }

}

/**
 * Removing a task from the goals_activity table.
 *
 * @param string $task_type
 *   The task type to remove.
 * @param string $user_to_save_for
 *   User Name user to remove.
 * @param string $external_id
 *   External ID to remove with activity.
 */
function goals_rules_remove_task($task_type, $user_to_save_for, $external_id) {
  // Attempt to convert user_name to UID.
  $uid = _goals_rules_username_to_uid($user_to_save_for);
  if ($uid) {
    $sql = db_delete('goals_activity')
        ->condition('uid', $uid)
        ->condition('task_type', $task_type)
        ->condition('external_id', $external_id)
        ->execute();
  }
}

/**
 * Provide a list of tasks to the rules action list.
 *
 * @return array
 *   Array to task types.
 */
function goals_rules_action_task_name_list() {
  $arr = array();
  $s = "SELECT type FROM {goals_tasks} GROUP BY type ORDER BY type";
  $result = db_query($s);
  foreach ($result as $record) {
    $arr[$record->type] = $record->type;
  }
  return $arr;
}

/**
 * Look up the user name and convert to uid.
 *
 * @param string $username
 *   User Name to look up.
 *
 * @return int
 *   User ID or FALSE if not found.
 */
function _goals_rules_username_to_uid($username) {
  // Look up by username.
  $u = user_load_by_name($username);
  if ($u) {
    return $u->uid;
  }
  elseif (is_numeric($username)) {
    // Try by id, in case a number is passed.
    $u_by_id = user_load($username);
    if ($u_by_id) {
      return $u_by_id->uid;
    }
  }
  // Return false since we can't find the user.
  return FALSE;
}

/**
 * Implements hook_rules_event_info().
 */
function goals_rules_event_info() {
  $items = array();

  $goal_variables = array(
    'goal' => array(
      'type' => 'goals_goal_datatype',
      'label' => t('Goal being completed'),
    ),
    'awarded_user' => array(
      'type' => 'user',
      'label' => t('goals/tasks user'),
    ),
  );

  $items['goal_awarded'] = array(
    'group' => t('Goal'),
    'label' => t('A goal is completed by a user.'),
    'variables' => $goal_variables,
  );

  $task_variables = array(
    'task' => array(
      'type' => 'goals_task_datatype',
      'label' => t('Task being completed'),
    ),
    'awarded_user' => array(
      'type' => 'user',
      'label' => t('goals/tasks user'),
    ),
  );

  $items['task_awarded'] = array(
    'group' => t('Goal task'),
    'label' => t('A task is completed by a user.'),
    'variables' => $task_variables,
  );

  return $items;
}

/**
 * Implements hook_rules_data_info().
 */
function goals_rules_data_info() {
  return array(
    'goals_goal_datatype' => array(
      'label' => t('Goal'),
    ),
    'goals_task_datatype' => array(
      'label' => t('Task'),
    ),
  );
}
/**
 * Implements hook_rules_condition_info().
 */
function goals_rules_condition_info() {
  $conditions['goals_goal_condition'] = array(
    'label' => t('The goal completed by a user'),
    'parameter' => array(
      'goal_id' => array(
        'type' => 'text',
        'label' => t('Goal'),
        'options list' => '_goals_list_goals',
        'description' => t('Select the goal being completed by the user'),
        'restriction' => 'input',
      ),
    ),
    'group' => t('Goals'),
  );
  $conditions['goals_task_condition'] = array(
    'label' => t('The task completed by a user'),
    'parameter' => array(
      'task_id' => array(
        'type' => 'text',
        'label' => t('Goal Task'),
        'options list' => '_goals_list_tasks',
        'description' => t('Select the task being completed by the user'),
        'restriction' => 'input',
      ),
    ),
    'group' => t('Goals'),
  );
  $conditions['goals_task_goal_condition'] = array(
    'label' => t('The task completed by a user belongs to a goal.'),
    'parameter' => array(
      'goal_id' => array(
        'type' => 'text',
        'label' => t('Goal'),
        'options list' => '_goals_list_goals',
        'description' => t('Select the goal the task belongs to.'),
        'restriction' => 'input',
      ),
    ),
    'group' => t('Goals'),
  );
  return $conditions;
}

/**
 * Helper function providing an array of goal info.
 */
function _goals_list_goals() {
  $goals = db_query("SELECT goal_id, title from {goals} ORDER BY title")->fetchAll();

  foreach ($goals as $goal) {
    $items[$goal->goal_id] = t($goal->title);
  }
  return $items;
}

/**
 * Helper function providing an array of task info.
 */
function _goals_list_tasks() {
  $tasks = db_query("SELECT task_id, title from {goals_tasks} ORDER BY title")->fetchAll();

  foreach ($tasks as $task) {
    $items[$task->task_id] = t($task->title);
  }
  return $items;
}

/**
 * Check a task condition.
 */
function goals_task_condition($arg1, $cond_task, $rulesstate) {
  // The task within the condition.
  $cond_task_id = $cond_task['task_id'];

  // The goal that was awarded.
  $this_task_id = $rulesstate->variables['task']->task_id;

  if ($cond_task_id == $this_task_id) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Check a goal condition.
 */
function goals_goal_condition($arg1, $cond_goal, $rulesstate) {

  // The goal within the condition.
  $cond_goal_id = $cond_goal['goal_id'];

  // The goal that was awarded.
  $this_goal_id = $rulesstate->variables['goal']->goal_id;

  if ($cond_goal_id == $this_goal_id) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Check a goal condition for the task.
 */
function goals_task_goal_condition($arg1, $cond_goal, $rulesstate) {

  // The goal within the condition.
  $cond_goal_id = $cond_goal['goal_id'];

  // The goal that was awarded.
  $this_goal_id = $rulesstate->variables['task']->goal_id;

  if ($cond_goal_id == $this_goal_id) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
