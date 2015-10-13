<?php
/**
 * @file
 * Hooks provided by Goals module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Act on Goals being assembled before rendering.
 *
 * @param object $entity
 *   The entity object.
 * @param string $type
 *   The type of entity being rendered (i.e. node, user, comment).
 * @param string $view_mode
 *   The view mode the entity is rendered in.
 * @param string $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $entity->content prior to rendering. The
 * structure of $entity->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_view_alter()
 * @see hook_comment_view()
 * @see hook_node_view()
 * @see hook_user_view()
 */
function hook_goals_goal_view($entity, $type, $view_mode, $langcode) {
  $entity->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Act on tasks being assembled before rendering.
 *
 * @param object $entity
 *   The entity object.
 * @param string $type
 *   The type of entity being rendered (i.e. node, user, comment).
 * @param string $view_mode
 *   The view mode the entity is rendered in.
 * @param string $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $entity->content prior to rendering. The
 * structure of $entity->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_view_alter()
 * @see hook_comment_view()
 * @see hook_node_view()
 * @see hook_user_view()
 */
function hook_goals_task_view($entity, $type, $view_mode, $langcode) {
  $entity->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Notification to modules that a goal is completed.
 *
 * @param int $goal_id
 *   Goal being completed.
 * @param int $uid
 *   UserID of user completing the goal.
 */
function hook_goals_completed_goal($goal_id, $uid) {
  if (field_info_instance('goal', 'goal_userpoints', 'goal_bundle') && function_exists('userpoints_userpointsapi')) {
    $goals = entity_load('goal', array($goal_id));
    $goal = $goals[$goal_id];
    $point_array = field_get_items('goal', $goal, 'goal_userpoints');
    $points = $point_array[0]['value'];

    if ($points) {
      $params = array(
        'uid' => $uid,
        'points' => $points,
        'description' => t('Goal @goal completed.', array('@goal' => $goal->title)),
      );
      userpoints_userpointsapi($params);
    }
  }
}

/**
 * Notification to modules that a task is completed.
 *
 * @param int $task_id
 *   Task being completed.
 * @param int $uid
 *   UserID of the user completing the task.
 */
function hook_goals_completed_task($task_id, $uid) {
  $tasks = entity_load('goal_task', array($task_id));
  $task = $tasks[$task_id];
  $message = t('You have completed the task @task.', array('@task' => $task->title));
  drupal_set_message($message);
}
