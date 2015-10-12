<?php

/**
 * @file
 * Administration pages for Goals module.
 */

/**
 * Basic goals administration page, with ability to manage fields and display.
 *
 * @return array
 *   Returns the content for the goals management page.
 */
function goals_goal_management_page() {
  $content = array();
  $content[] = array(
    '#type' => 'item',
    '#markup' => t('This is the administration page for goals. On this page you can add and edit goals for your site. You can also add the tasks which make up a goal.'),
  );

  $content[] = array(
    '#type' => 'item',
    '#markup' => l(t('Add a goal'), 'goals/goal/add'),
  );

  $content['table'] = _goals_goal_table();

  return $content;
}

/**
 * Generate a table of goals.
 *
 * @return array
 *   Returns a table array for the list of goals.
 */
function _goals_goal_table() {
  $content = array();
  // Load all of our goals.
  $goals = goals_load_multiple();
  if (!empty($goals)) {
    foreach ($goals as $goal) {
      // Create tabular rows for goals.
      $rows[] = array(
        'data' => array(
          'title' => l($goal->title, 'goals/goal/' . $goal->goal_id),
          'edit' => l(t('Edit'), 'goals/goal/' . $goal->goal_id . '/edit'),
        ),
      );
    }
    // Put our entities into a themed table. See theme_table() for details.
    $content['entity_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(
        t('Goal'),
        t('Edit'),
      ),
    );
  }
  else {
    // There were no entities. Tell the user.
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No goals currently exist.'),
    );
  }
  return $content;
}


/**
 * Basic tasks administration page, with ability to manage fields and display.
 *
 * @return array
 *   Returns an array of content for the task management page.
 */
function goals_task_management_page() {
  $content = array();
  $content[] = array(
    '#type' => 'item',
    '#markup' => t('This is the administration page for tasks.'),
  );

  $content[] = array(
    '#type' => 'item',
    '#markup' => l(t('Add a task'), 'goals/task/add'),
  );

  $content['table'] = _goals_task_table();

  return $content;
}

/**
 * Generate a table of tasks.
 *
 * @return array
 *   Returns an array of tasks for formatting as table.
 */
function _goals_task_table() {
  $content = array();
  // Load all of our entities.
  $tasks = goals_tasks_load_multiple();
  if (!empty($tasks)) {
    foreach ($tasks as $task) {
      // Create tabular rows for tasks.
      // Get the associated goal.
      $goal = goals_load($task->goal_id);

      $rows[] = array(
        'data' => array(
          'title' => l($task->title, 'goals/task/' . $task->task_id),
          'requirement' => goals_task_requirements($task),
          'goal' => l($goal->title, 'goals/goal/' . $goal->goal_id),
          'edit' => l(t('Edit'), 'goals/task/' . $task->task_id . '/edit'),
        ),
      );
    }
    // Put our entities into a themed table. See theme_table() for details.
    $content['entity_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(
        t('Task'),
        t('Task Requirement'),
        t('Associated Goal'),
        t('Edit'),
      ),
    );
  }
  else {
    // There were no entities. Tell the user.
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No goals currently exist.'),
    );
  }
  return $content;
}

/**
 * Provides a list of existing Goals and Tasks and the ability to add more.
 *
 * @return array
 *   Returns the content for the main goals admin page.
 */
function goals_admin_summary_page() {
  $content = array();
  $content[] = array(
    '#type' => 'item',
    '#markup' => t('This is the administration page for goals. On this page you can add and edit goals for your site. You can also add the tasks which make up a goal.'),
  );

  $content[] = array(
    '#type' => 'item',
    '#markup' => l(t('Add a goal'), 'goals/goal/add'),
  );

  $content['table'] = goals_admin_summary_list_goals();

  return $content;
}

/**
 * Returns a render array with all goals and tasks.
 *
 * In this basic example we know that there won't be many entities,
 * so we'll just load them all for display. See pager_example.module
 * to implement a pager. Most implementations would probably do this
 * with the contrib Entity API module, or a view using views module,
 * but we avoid using non-core features in the Examples project.
 *
 * @return array
 *   Returns an array for the table of the main summary page.
 *
 * @see pager_example.module
 */
function goals_admin_summary_list_goals() {
  $content = array();
  // Load all of our entities.
  $goals = goals_load_multiple();
  if (!empty($goals)) {
    foreach ($goals as $goal) {
      // Create tabular rows for our entities.
      $rows[] = array(
        'data' => array(
          'title' => l($goal->title, 'goals/goal/' . $goal->goal_id),
          'edit' => l(t('Edit'), 'goals/goal/' . $goal->goal_id . '/edit'),
        ),
      );
      $rows[] = array(
        'data' => array(
          'data' => array(
            'data' => goals_load_task_list($goal->goal_id, TRUE),
            'colspan' => 2,
          ),
        ),
      );
    }
    // Put our entities into a themed table. See theme_table() for details.
    $content['entity_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(
        t('Goal'),
        t('Edit'),
      ),
    );
  }
  else {
    // There were no entities. Tell the user.
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No goals currently exist.'),
    );
  }
  return $content;
}

/**
 * Generate a list of Tasks for the goal.
 *
 * @param null $goal_id
 *   The goal ID to be loaded by the function.
 *
 * @param bool $show_edit_link
 *   Should edit links be generated?
 *
 * @return string
 *   Themed item list of tasks for the goal.
 */
function goals_load_task_list($goal_id = NULL, $show_edit_link = FALSE) {
  $list = array();
  $sql = "SELECT * FROM {goals_tasks} WHERE goal_id = :goal_id ORDER BY title";
  $result = db_query($sql, array(':goal_id' => $goal_id));

  foreach ($result as $row) {
    unset($task, $within, $limit);
    $task = goals_task_requirements($row);

    $list[] = $task . ($show_edit_link ? " (" . l(t('edit'), 'goals/task/' . $row->task_id . '/edit') . ")" : '');
  }

  if (count($list) > 0) {
    $list[] = l(t('Add goal task'), 'goals/task/add/' . $goal_id);
  }
  else {
    $list[] = t('There are currently no tasks for this goal.');
    $list[] = l(t('Add goal task'), 'goals/task/add/' . $goal_id);
  }
  return theme('item_list', array('items' => $list));

}
