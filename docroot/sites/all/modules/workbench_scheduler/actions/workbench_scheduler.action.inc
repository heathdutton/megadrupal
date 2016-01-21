<?php
/**
 * Workbench scheduler actions form.
 * @param $options
 * @return array
 */
function workbench_scheduler_schedules_action_form($options) {
  $form = array();
  $types = node_type_get_types();

  // Get all schedules from all content types.
  $cts_schedules = workbench_scheduler_load_type_schedules(array_keys($types));
  foreach ($cts_schedules as $ct => $schedules) {
    foreach ($schedules as $sid => $schedule) {
      $type_schedules[$sid] = $schedule;
    }
  }

  // Add a scheduler section.
  $form['workbench_scheduler'] = array(
    '#type'         => 'fieldset',
    '#title'        => 'Workbench Schedule',
    '#description'  => t('Select a schedule for changing moderation states.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
    '#group'        => 'additional_settings',
    '#access'       => user_access('set workbench schedule'),
  );

  // Build an options array for which schedules to choose.
  $schedule_options = array();
  // Add default option of no schedule.
  $schedule_options[] = array(
    'label'       => t('No Schedule'),
    'start_state' => '',
    'end_state'   => '',
  );

  // Retrieve a list of human safe moderation state names.
  $moderation_states = workbench_scheduler_state_labels();

  // Add each type schedule to the options array.
  foreach ($type_schedules as $schedule) {
    $schedule_options[$schedule->sid] = array(
      'label'        => $schedule->label,
      'start_state'  => (!empty($schedule->start_state) ? $moderation_states[$schedule->start_state] : ''),
      'end_state'    => (!empty($schedule->end_state) ? $moderation_states[$schedule->end_state] : ''),
    );

    drupal_add_js(array('workbench_scheduler' => array('schedules' => $schedule_options)), 'setting');

    // Create table select for selecting schedule.
    $form['workbench_scheduler']['workbench_scheduler_sid'] = array(
      '#type'         => 'tableselect',
      '#title'        => t('Select Schedule'),
      '#description'  => t('Select the schedule to use for this node.'),
      '#options'      => $schedule_options,
      '#multiple'     => FALSE,
      '#header'       => array(
        'label'         => t('Name'),
        'start_state'   => t('Start State'),
        'end_state'     => t('End State'),
      ),
    );

    // Fieldset for collecting schedule dates.
    $form['workbench_scheduler']['dates'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('Schedule Dates'),
      '#description'  => t('Select the start and/or end dates to trigger states changes for this node.'),
      '#states'       => array(
        'invisible' => array(
          ':input[name=workbench_scheduler_sid]' => array('value' => '0'),
        ),
      ),
    );

    // Start date.
    if (!empty($schedule->start_state)) {
      $form['workbench_scheduler']['dates']['workbench_scheduler_start_date'] = array(
        '#type'         => 'date_popup',
        '#date_format'  => 'Y-m-d H:i',
        '#title'        => t('Start date'),
        '#description'  => t('Select the date to switch this node to the scheduled \'start state\''),
      );
    }

    // End date.
    if (!empty($schedule->end_state)) {
      $form['workbench_scheduler']['dates']['workbench_scheduler_end_date'] = array(
        '#type'         => 'date_popup',
        '#date_format'  => 'Y-m-d H:i',
        '#title'        => t('End date'),
        '#description'  => t('Select the date to switch this node to the scheduled \'end state\''),
      );
    }

    $node_schedule = isset($form['#node']->workbench_schedule) ? $form['#node']->workbench_schedule : FALSE;
    // Have a node schedule(editing a node)? Use for defaults.
    if ($node_schedule) {
      $form['workbench_scheduler']['workbench_scheduler_sid']['#default_value'] = $node_schedule->sid;

      if ($node_schedule->start_date) {
        // Format date for field.
        $start_date = format_date($node_schedule->start_date, 'custom', 'Y-m-d H:i:s');
        $form['workbench_scheduler']['dates']['workbench_scheduler_start_date']['#default_value'] = $start_date;
      }

      if ($node_schedule->end_date) {
        // Format date for field.
        $end_date = format_date($node_schedule->end_date, 'custom', 'Y-m-d H:i:s');
        $form['workbench_scheduler']['dates']['workbench_scheduler_end_date']['#default_value'] = $end_date;
      }
    }
  }

  // Add custom validation and submission hooks.
  $form['#validate'][] = 'workbench_scheduler_node_form_validate';
  // Add to submit button action so that will have access to the new.
  // Nid & vid values.
  $form['actions']['submit']['#submit'][] = 'workbench_scheduler_node_form_submit';

  return $form;
}

/**
 * Workbench scheduler action submit.
 * @param $form
 * @param $form_state
 * @return mixed
 */
function workbench_scheduler_schedules_action_submit($form, &$form_state) {

  if (isset($form_state['values']['workbench_scheduler_sid']))
    $context['workbench_scheduler_sid'] = $form_state['values']['workbench_scheduler_sid'];

  if (isset($form_state['values']['workbench_scheduler_start_date']))
    $context['workbench_scheduler_start_date'] = $form_state['values']['workbench_scheduler_start_date'];

  if (isset($form_state['values']['workbench_scheduler_end_date']))
    $context['workbench_scheduler_end_date'] = $form_state['values']['workbench_scheduler_end_date'];

  return $context;
}

/**
 * Implements the action that is applied to the revisions.
 * @param $entity
 * @param array $context
 */
function workbench_scheduler_schedules_action(&$entity, $context = array()) {

  $entity->workbench_schedule->sid = $context['workbench_scheduler_sid'];

  if (isset($context['workbench_scheduler_start_date'])) {
    $entity->workbench_schedule->start_date = strtotime($context['workbench_scheduler_start_date']);
  }

  if (isset($context['workbench_scheduler_end_date'])) {
    $entity->workbench_schedule->end_date = strtotime($context['workbench_scheduler_end_date']);
  }

  // Change vid to latest.
  $live_revision = workbench_moderation_node_current_load($entity);
  $entity->vid = $live_revision->vid;
}
