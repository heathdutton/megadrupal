<?php

/**
 * Implements hook_iib_page_item()
 *
 * @return array
 */
function state_flow_iib_entity_item($entity, $entity_type, $view_mode) {
  $output = $events = array();

  switch ($entity_type) {
    case 'node':
      if (is_object($entity) && $view_mode == 'full') {
        $state_flow = state_flow_load_state_machine($entity);
        $state = $state_flow->get_current_state();
        $output[] = t('Revision Status: @state', array('@state' => $state));

        $all_events = $state_flow->get_available_events();
        foreach ($all_events as $event_machine_name => $event) {
          if (state_flow_access($entity, $event_machine_name)) {
            $events[$event_machine_name] = $event->get_option('label');
          }
        }

        if (!empty($events)) {
          $iib_state_form = drupal_get_form('state_flow_iib_entity_state_form', $events, $entity, $state_flow);
          $output[] = drupal_render($iib_state_form);
        }
      }
      break;
  }

  if (!empty($output)) {
    $items['center'] = array(
      '#weight' => -10,
      '#prefix' => '<div class="state-flow-iib">',
      '#markup' => implode("\n", $output),
      '#suffix' => '</div>',
    );
    return $items;
  }
}

/**
 * Form to generate events for IIB
 *
 * @param $form
 * @param $form_state
 * @param $events
 * @param $entity
 * @param $state_flow
 * @return array
 */
function state_flow_iib_entity_state_form($form, &$form_state, $events, $entity, $state_flow) {
  $form = array();

  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['event'] = array(
    '#type' => 'select',
    '#options' => array('' => t('- Choose Revision Action -')) + $events,
    '#id' => 'edit-event',
  );

  if (in_array('Schedule', $events)) {
    $form['selected-date'] = array(
      '#type' => 'date_popup',
      '#title' => 'Select a date and time (Schedule only)',
      '#date_year_range' => '-0:+3',
      '#date_label_position' => 'within',
      '#date_format' => 'm/d/Y h:i a',
      '#dependency' => array('edit-event' => array('schedule')),
    );
    $form['event-name'] = array(
      '#type' => 'hidden',
      '#default_value' => 'schedule',
    );
    $form['nid'] = array(
      '#type' => 'hidden',
      '#default_value' => $entity->nid,
    );
    $form['vid'] = array(
      '#type' => 'hidden',
      '#default_value' => $entity->vid,
    );
    $form['#validate'][] = 'state_flow_scheduled_content_validate';
  }

  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  $form['state_flow'] = array(
    '#type' => 'value',
    '#value' => $state_flow,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set'),
  );

  return $form;
}

/**
 * Submit handler for IIB Entity State Form
 *
 * @param $form
 * @param $form_state
 */
function state_flow_iib_entity_state_form_submit($form, &$form_state) {
  global $user;
  $entity = $form_state['values']['entity'];
  $state_flow = $form_state['values']['state_flow'];
  $event = $form_state['values']['event'];

  if (!empty($event)) {
    $rv = $state_flow->fire_event($event, $user->uid, 'Transitioned via IIB.');
    if ($rv !== FALSE) {
      $state = $state_flow->get_current_state();
      $message = t('%title transitioned to the @state state.', array('%title' => $entity->title, '@state' => $state));

      if ($event == 'schedule') {
        $selected_date = strtotime($form_state['values']['selected-date']);
        $nid = $form_state['values']['nid'];
        $vid = $form_state['values']['vid'];
        state_flow_schedule_schedule($nid, $vid, $selected_date);

        //set message
        $message = t('This content has been scheduled to be published on !date', array('!date' => $form_state['values']['selected-date']));
        $state_flow->write_history($user->uid, $message);
      }
      drupal_set_message($message);
    }
  }
}