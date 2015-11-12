<?php
/**
 * @file
 * Admin page callbacks and forms.
 */

/**
 * Form callback for main tasks form.
 */
function og_tasks_form($form, &$form_state, $entity_type, $etid) {
  og_set_breadcrumb($entity_type, $etid, array(l(t('Group'), "$entity_type/$etid/group")));

  $entities = entity_load($entity_type, array($etid));
  $entity = reset($entities);
  $tasks = og_tasks_get_tasks($entity_type, $entity);

  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );

  $form['tasks'] = array(
    '#type' => 'tableselect',
    '#header' => array(t('Task'), t('Status')),
    '#empty' => t('There are no tasks defined for this group.'),
  );

  $weight = array(
    OG_TASKS_REQUIRED => -1,
    OG_TASKS_OPTIONAL => 0,
    OG_TASKS_COMPLETED => 1,
  );

  $status = array(
    OG_TASKS_REQUIRED => t('Required'),
    OG_TASKS_OPTIONAL => t('Optional'),
    OG_TASKS_COMPLETED => t('Completed'),
  );

  foreach($tasks as $key => $task) {
    // Manually creating the checkboxes to be able to set #disabled.
    $form['tasks'][$key] = array(
      '#type' => $task->completed ? 'value' : 'checkbox',
      '#title' => $task->title,
      '#title_display' => 'invisible',
      '#return_value' => $key,
      '#default_value' => $task->required && !$task->completed,
      '#disabled' => $task->required || $task->completed,
      '#weight' => $weight[$task->get_status()],
    );
    $form['tasks']['#options'][$key] = array(
      '<strong>'. $task->title .'</strong><br/>'. $task->description,
      $status[$task->get_status()],
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run tasks'),
  );

  return $form;
}

/**
 * Submit handler for og_tasks_form.
 */
function og_tasks_form_submit($form, &$form_state) {
  list($entity_type, $etid) = $form_state['build_info']['args'];
  $entities = entity_load($entity_type, array($etid));
  $entity = reset($entities);
  $all_tasks = og_tasks_get_tasks($entity_type, $entity);

  $tasks = array();
  foreach ($form_state['values']['tasks'] as $key => $selected) {
    if ($selected) {
      $tasks[$key] = $all_tasks[$key];
    }
  }

  if (!empty($tasks)) {
    $batch = og_tasks_get_batch($entity, $tasks);
    batch_set($batch);
  }
  else {
    drupal_set_message(t('Please select a task first.'));
  }
}

/**
 * Batch API to run processes.
 */
function og_tasks_get_batch($entity, $tasks) {
  // Use Batch API to run the make site process
  // Add operations
  $operations = array();

  foreach ($tasks as $key => $task) {
    $operations[] = array('og_tasks_batch_operation', array($entity, $task));
  }

  // Set up the batch
  $batch = array(
    'operations' => $operations,
    'finished' => 'og_tasks_batch_finished',
    'title' => t('Running group tasks'),
    'init_message' => t('Preparing to run tasks'),
    'progress_message' => t('Processed @current of out @total.'),
    'file' => drupal_get_path('module', 'og_tasks') .'/includes/admin.inc',
  );

  return $batch;
}

/**
 * Execute batch operation on entity.
 */
function og_tasks_batch_operation($entity, $task, &$context) {
  $task->execute($entity, $context);
}

/**
 * Batch completion callback
 */
function og_tasks_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('done'));
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }
}

