<?php

/**
 * @file
 * Goal entities definitions.
 */

/**
 * Defining the Goal entity.
 *
 * @return array
 *   Entity information.
 */
function _goals_entity_info_goal() {
  return array(
    'label' => t('Goal'),
    'entity class' => 'Entity',
    'controller class' => 'EntityAPIController',
    'base table' => 'goals',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'goal_id',
      'bundle' => 'bundle_type',
      'label' => t('Goal ID'),
    ),
    'view modes' => array(
      'full' => array(
        'label' => t('Full'),
        'custom settings' => FALSE,
      ),
    ),
    'bundle keys' => array(
      'bundle' => 'bundle_type',
    ),
    // Make use the class' label() and uri() implementation by default.
    'label callback' => 'entity_class_label',
    'uri callback' => 'goals_goal_uri',
    'module' => 'goals',
    // Enable the Entity API admin UI.
    'bundles' => array(
      'goal_bundle' => array(
        'label' => 'Goals bundle',
        // 'admin' key is used by the Field UI to provide field and
        // display UI pages.
        'admin' => array(
          'path' => 'admin/config/goals/manage',
          'access arguments' => array('administer goals entities'),
        ),
      ),
    ),
  );
}

/**
 * Defining the Task entity.
 *
 * @return array
 *   Entity information.
 */
function _goals_entity_info_goal_task() {
  return array(
    'label' => t('Goal task'),
    'entity class' => 'Entity',
    'controller class' => 'EntityAPIController',
    'base table' => 'goals_tasks',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'task_id',
      'bundle' => 'bundle_type',
      'label' => t('Task ID'),
    ),
    'view modes' => array(
      'full' => array(
        'label' => t('Full'),
        'custom settings' => FALSE,
      ),
    ),
    'bundle keys' => array(
      'bundle' => 'bundle_type',
    ),
    // Make use the class' label() and uri() implementation by default.
    'label callback' => 'entity_class_label',
    'uri callback' => 'goals_task_uri',
    'module' => 'goals',
    // Enable the Entity API admin UI.
    'bundles' => array(
      'task_bundle' => array(
        'label' => 'Goals bundle',
        // 'admin' key is used by the Field UI to provide field and
        // display UI pages.
        'admin' => array(
          'path' => 'admin/config/goals/tasks/manage',
          'access arguments' => array('administer goals entities'),
        ),
      ),
    ),
  );
}

/**
 * Defining completed task entity.
 *
 * @return array
 *   Entity information.
 */
function _goals_entity_info_task_completed() {
  return array(
    'label' => t('Goal task - completed by users'),
    'entity class' => 'Entity',
    'controller class' => 'EntityApiController',
    'base table' => 'goals_completed_tasks',
    'fieldable' => FALSE,
    'entity keys' => array(
      'id' => 'id',
    ),
    'module' => 'goals',
  );
}

/**
 * Defining completed goals entity.
 *
 * @return array
 *   Entity information.
 */
function _goals_entity_info_goal_completed() {
  return array(
    'label' => t('Goal - completed by users'),
    'entity class' => 'Entity',
    'controller class' => 'EntityApiController',
    'base table' => 'goals_completed_goals',
    'fieldable' => FALSE,
    'entity keys' => array(
      'id' => 'id',
    ),
    'module' => 'goals',
  );
}

/**
 * Function called by goals_entity_property_info_alter().
 *
 * @param array $info
 *   The entity information array to be altered.
 */
function _goals_entity_property_info_alter(&$info) {

  $goals = &$info['goal']['properties'];

  $goals['created'] = array(
    'label' => t("Created"),
    'description' => t("The date the goal was created."),
    'type' => 'date',
    'schema field' => 'created',
  );
  $goals['changed'] = array(
    'label' => t("Changed"),
    'description' => t("The date the goal was changed."),
    'type' => 'date',
    'schema field' => 'changed',
  );

  $tasks = &$info['goal_task']['properties'];
  $tasks['created'] = array(
    'label' => t("Created"),
    'description' => t("The date the task was created."),
    'type' => 'date',
    'schema field' => 'created',
  );
  $tasks['changed'] = array(
    'label' => t("Changed"),
    'description' => t("The date the task was changed."),
    'type' => 'date',
    'schema field' => 'changed',
  );
  $tasks['goal_id'] = array(
    'label' => t('Goal'),
    'description' => t('The goal associated to this task.'),
    'schema field' => 'goal_id',
    'type' => 'goal',
  );

  $tasks_completed = &$info['goal_task_completed']['properties'];
  $tasks_completed['created'] = array(
    'label' => t("Created"),
    'description' => t("The date the task was completed by the user."),
    'type' => 'date',
    'schema field' => 'created',
  );
  $tasks_completed['goal_id'] = array(
    'label' => t('Goal'),
    'description' => t('The goal associated to this task.'),
    'schema field' => 'goal_id',
    'type' => 'goal',
  );
  $tasks_completed['task_id'] = array(
    'label' => t('Task'),
    'description' => t('The task which was completed.'),
    'schema field' => 'task_id',
    'type' => 'goal_task',
  );
  $tasks_completed['uid'] = array(
    'label' => t('User'),
    'description' => t('The user completing the task.'),
    'schema field' => 'uid',
    'type' => 'user',
  );

  $goals_completed = &$info['goal_completed']['properties'];
  $goals_completed['created'] = array(
    'label' => t("Created"),
    'description' => t("The date the goal was completed by the user."),
    'type' => 'date',
    'schema field' => 'created',
  );
  $goals_completed['goal_id'] = array(
    'label' => t('Goal'),
    'description' => t('The goal which was completed.'),
    'schema field' => 'goal_id',
    'type' => 'goal',
  );
  $goals_completed['uid'] = array(
    'label' => t('User'),
    'description' => t('The user completing the goal.'),
    'schema field' => 'uid',
    'type' => 'user',
  );
}

/**
 * Defining extra Goals fields.
 *
 * @return array
 *   Extra field info for entities.
 */
function _goals_field_extra_fields() {

  $goal_form_elements['title'] = array(
    'label' => t('Title'),
    'description' => t('The title of the goal.'),
    'weight' => -5,
  );
  $goal_display_elements['created'] = array(
    'label' => t('Creation date'),
    'description' => t('Creation date (an extra display field)'),
    'weight' => 0,
  );
  $goal_display_elements['title'] = array(
    'label' => t('Goal Title'),
    'description' => t('Just like title, but trying to point out that it is a separate property'),
    'weight' => 0,
  );
  $task_form_elements['title'] = array(
    'label' => t('Title'),
    'description' => t('The title of the task.'),
    'weight' => -10,
  );
  $task_form_elements['goal_id'] = array(
    'label' => t('Goal ID'),
    'description' => t('Goal to which this task is associated'),
    'weight' => -9,
  );
  $task_form_elements['type'] = array(
    'label' => t('Task Type'),
    'description' => t('The machine name of the task to be performed.'),
    'weight' => -8,
  );
  $task_form_elements['task_count'] = array(
    'label' => t('Task Count'),
    'description' => t('The number of times the task must be performed'),
    'weight' => -7,
  );
  $task_form_elements['within'] = array(
    'label' => t('Within (Number)'),
    'description' => t('Part of the Within constraint of this Task'),
    'weight' => -6,
  );
  $task_form_elements['within_interval'] = array(
    'label' => t('Within Interval'),
    'description' => t('Part of the Within constraint of this Task'),
    'weight' => -3,
  );
  $task_form_elements['limit_count'] = array(
    'label' => t('Limit (Number)'),
    'description' => t('Part of the Limit constraint of this Task'),
    'weight' => -7,
  );
  $task_form_elements['limit_interval'] = array(
    'label' => t('Limit Interval'),
    'description' => t('Part of the Limit constraint of this Task'),
    'weight' => -5,
  );
  $task_display_elements['type'] = array(
    'label' => t('Task Type'),
    'description' => t('The machine name of the task to be performed.'),
    'weight' => -8,
  );
  $task_display_elements['title'] = array(
    'label' => t('Title'),
    'description' => t('Title'),
    'weight' => -10,
  );
  $task_display_elements['requirement'] = array(
    'label' => t('Task Requirement'),
    'description' => t('A short summary of the requirement to complete this task'),
    'weight' => 0,
  );
  // Since we have only one bundle type, we'll just provide the extra_fields
  // for it here.
  $extra_fields['goal']['goal_bundle']['form'] = $goal_form_elements;
  $extra_fields['goal']['goal_bundle']['display'] = $goal_display_elements;
  $extra_fields['goal_task']['task_bundle']['form'] = $task_form_elements;
  $extra_fields['goal_task']['task_bundle']['display'] = $task_display_elements;

  return $extra_fields;
}
