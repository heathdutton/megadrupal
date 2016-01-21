<?php

/**
 * @file
 * Functions used to generate forms for CRUD operations.
 */


/**
 * Section I - Goals Forms
 */

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function goals_add() {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $entity = entity_create('goal', array('bundle_type' => 'goal_bundle'));
  return drupal_get_form('goals_form', $entity);
}
/**
 * Form function to create an goals entity.
 *
 * The pattern is:
 * - Set up the form for the data that is specific to your
 *   entity: the columns of your base table.
 * - Call on the Field API to pull in the form elements
 *   for fields attached to the entity.
 */
function goals_form($form, &$form_state, $entity) {

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Goal Title'),
    '#required' => TRUE,
    '#default_value' => (isset($entity->title) ? $entity->title : ''),
  );

  $form_state['goal_entity'] = $entity;

  field_attach_form('goal', $entity, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  if (isset($entity->goal_id)) {
    $form['delete'] = array(
      '#type' => 'link',
      '#title' => t('Delete this goal'),
      '#href' => 'goals/goal/' . $entity->goal_id . '/delete',
      '#weight' => 105,
    );
  }

  return $form;
}


/**
 * Validation handler for goals_add_form form.
 *
 * We pass things straight through to the Field API to handle validation
 * of the attached fields.
 */
function goals_form_validate($form, &$form_state) {
  field_attach_form_validate('goal', $form_state['goal_entity'], $form, $form_state);
}


/**
 * Form submit handler: submits basic_add_form information.
 */
function goals_form_submit($form, &$form_state) {
  $entity = $form_state['goal_entity'];
  // Add custom fields here.
  $entity->title = $form_state['values']['title'];

  field_attach_submit('goal', $entity, $form, $form_state);
  if (isset($entity->is_new)) {
    $entity->created = REQUEST_TIME;
  }
  $entity->changed = REQUEST_TIME;

  $success = entity_save('goal', $entity);
  if (isset($success)) {
    drupal_set_message(t('The goal %title has been saved.',
        array('%title' => $entity->title))
    );
    $form_state['redirect'] = 'goals/goal/' . $entity->goal_id;
  }
}

/**
 * We save the entity by calling the controller.
 */
function goals_goal_save(&$entity) {
  return entity_get_controller('goal')->save($entity);
}

/**
 * Goal delete form.
 */
function goals_delete_form($form, &$form_state, $goal) {
  $form = array();
  $form['goal_id'] = array('#type' => 'value', '#value' => $goal->goal_id);
  return confirm_form($form, t('Are you sure you want to delete the goal %title? This will also cause any tasks associated to this goal to be deleted as well.', array('%title' => $goal->title)), GOALS_ADMIN_PATH);
}

/**
 * Process the submitted delete form.
 */
function goals_delete_form_submit($form, &$form_state) {
  $goal_id = $form_state['values']['goal_id'];
  // Load the task.
  $goal = goals_load($goal_id);
  // Get tasks with this goal.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'goal_task')
      ->entityCondition('bundle', 'task_bundle')
      ->propertyCondition('goal_id', $goal_id);
  $result = $query->execute();
  if (isset($result['goal_task'])) {
    $task_ids = array_keys($result['goal_task']);
    entity_delete_multiple('goal_task', $task_ids);
  }

  entity_delete('goal', $goal->goal_id);
  drupal_set_message(t('The task %title has been deleted.',
      array('%title' => $goal->title))
  );
  $form_state['redirect'] = GOALS_ADMIN_PATH;
}

/**
 * Section II - Goal Task Forms
 */

/**
 * Provides a wrapper on the edit form to add a new task.
 */
function goals_task_add($goal_id = NULL) {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $entity = entity_create('goal_task', array('bundle_type' => 'task_bundle'));
  if (isset($goal_id)) {
    $entity->goal_id = $goal_id;
  }
  return drupal_get_form('goals_task_form', $entity);
}

/**
 * Form function to create an goals entity.
 *
 * The pattern is:
 * - Set up the form for the data that is specific to your
 *   entity: the columns of your base table.
 * - Call on the Field API to pull in the form elements
 *   for fields attached to the entity.
 */
function goals_task_form($form, &$form_state, $entity) {

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => (isset($entity->title) ? $entity->title : ''),
  );

  $form['goal_id'] = array(
    '#title' => t('Goal'),
    '#type' => 'select',
    '#required' => TRUE,
    '#default_value' => (isset($entity->goal_id) ? $entity->goal_id : ''),
    '#description' => t('Select the goal to which this task applies'),
    '#options' => goals_task_goal_options(),
  );
  $form['type'] = array(
    '#title' => t('Task Type'),
    '#type' => 'machine_name',
    '#machine_name' => array(
      'exists' => 'goals_task_name_exists',
    ),
    '#maxlength' => 30,
    '#required' => TRUE,
    '#description' => t('A machine name to identify the task (e.g. user_login), which can be used in other tasks to make recording task activity more efficient. Must contain only lower case letters and underscores.'),
    '#default_value' => (isset($entity->type) ? $entity->type : ''),
  );

  $form['task_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Task count'),
    '#size' => 4,
    '#maxlength' => 10,
    '#default_value' => isset($entity->task_count) ? $entity->task_count : 1,
    '#description' => t('The number of times this task type must be completed before the task is complete. Must be a positive number greater than zero.'),
    '#required' => TRUE,
    '#field_suffix' => 'time(s)',
  );
  $form['within_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Within'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#description' => t('If the task must be completed within a specific time frame, provide that information.'),
  );
  $form['within_field']['within'] = array(
    '#type' => 'textfield',
    '#title' => t('Within'),
    '#size' => 4,
    '#maxlength' => 10,
    '#default_value' => isset($entity->within) ? $entity->within : 0,
    '#description' => t('Enter zero if there is no within time constraint.'),
  );
  $form['within_field']['within_interval'] = array(
    '#type' => 'select',
    '#title' => t('Within interval'),
    '#default_value' => isset($entity->within_interval) ? $entity->within_interval : '',
    '#options' => goals_within_intervals(),
  );

  $form['limit_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Limits'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#description' => t('If there is a limit to the number of times a task can count toward the goal per interval (e.g. Only one login per day counts toward the goal), provide that information.'),
  );

  $form['limit_field']['limit_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit'),
    '#size' => 4,
    '#maxlength' => 10,
    '#default_value' => isset($entity->limit_count) ? $entity->limit_count : 0,
    '#description' => t('Enter zero if there is no limit constraint.'),
  );
  $form['limit_field']['limit_interval'] = array(
    '#type' => 'select',
    '#title' => t('Per calendar'),
    '#default_value' => isset($entity->limit_interval) ? $entity->limit_interval : '',
    '#options' => goals_limit_intervals(),
  );
  $form_state['task_entity'] = $entity;

  field_attach_form('goal_task', $entity, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  if (isset($entity->task_id)) {
    $form['delete'] = array(
      '#type' => 'link',
      '#title' => t('Delete this task'),
      '#href' => 'goals/task/' . $entity->task_id . '/delete',
      '#weight' => 105,
    );
  }
  $form_state['redirect'] = GOALS_ADMIN_PATH;
  return $form;
}

/**
 * Validation handler for goals_tasks_add_form form.
 *
 * We pass things straight through to the Field API to handle validation
 * of the attached fields.
 */
function goals_task_form_validate($form, &$form_state) {
  field_attach_form_validate('goal_task', $form_state['task_entity'], $form, $form_state);
}

/**
 * Checks if a machine name exists.
 *
 * Returns FALSE in all cases since it's ok for the machine name
 * to be used more than once. We just want to check the format.
 */
function goals_task_name_exists($name) {
  return FALSE;
}

/**
 * Form submit handler: submits basic_add_form information.
 */
function goals_task_form_submit($form, &$form_state) {
  $entity = $form_state['task_entity'];
  $entity->title = $form_state['values']['title'];
  $entity->type = $form_state['values']['type'];
  $entity->goal_id = $form_state['values']['goal_id'];
  $entity->task_count = $form_state['values']['task_count'];
  $entity->within = $form_state['values']['within'];
  $entity->within_interval = $form_state['values']['within_interval'];
  $entity->limit_count = $form_state['values']['limit_count'];
  $entity->limit_interval = $form_state['values']['limit_interval'];
  field_attach_submit('goal_task', $entity, $form, $form_state);
  if (isset($entity->is_new)) {
    $entity->created = REQUEST_TIME;
  }

  $entity->changed = REQUEST_TIME;

  $success = entity_save('goal_task', $entity);
  if (isset($success)) {
    drupal_set_message(t('The task %task has been saved.',
        array('%task' => $entity->title))
    );
    $form_state['redirect'] = 'goals/task/' . $entity->task_id;
  }
}
