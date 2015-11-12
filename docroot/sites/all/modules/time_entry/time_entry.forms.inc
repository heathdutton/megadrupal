<?php

/**
 * Provides a form to create and edit time entries.
 */
function time_entry_form($form, &$form_state, $type = NULL, $time_entry = NULL, $options = array()) {
  form_load_include($form_state, 'inc', 'time_entry', 'time_entry.forms');

  if (is_null($time_entry)) {
    $values = array('type' => $time_entry);
    if(isset($form_state['values'])) {
      $values += $form_state['values'];
    }
    $time_entry = time_entry_create($values);
    
    
    $time_entry = time_entry_create(array('type' => $type));
  }
  
  if (is_null($type)) {
    $type = $time_entry->type;
  }

  $form_state['time_entry'] = $time_entry;

  $form['type'] = array(
    '#type' => 'value',
    '#default_value' => $type
  );

  $form['id'] = array(
    '#type' => 'value',
    '#default_value' => $time_entry->id ? $time_entry->id : 0
  );

  $form['#attributes']['class'][] = 'time-entry-form';

  field_attach_form('time_entry', $time_entry, $form, $form_state);

  $start = $time_entry->time? format_date($time_entry->time, 'custom', 'c') : '';
  $form['start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start'),
    '#description' => t('Format: %format', array('%format' => format_date(time(), 'custom', 'c'))),
    '#required' => TRUE,
    '#default_value' => $start,
  );

  $end = $time_entry->time && $time_entry->duration? format_date($time_entry->time + $time_entry->duration, 'custom', 'c') : '';
  $form['end'] = array(
    '#type' => 'textfield',
    '#title' => t('End'),
    '#description' => t('Format: %format', array('%format' => format_date(time(), 'custom', 'c'))),
    '#required' => TRUE,
    '#default_value' => $end
  );

  $form['actions'] = array(
    '#type' => 'actions'
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create Time Entry'),
    '#weight' => 40,
  );

  if (isset($options['ajax']) && $options['ajax']) {
    $form['actions']['submit']['#ajax'] = array(
      'callback' => 'time_entry_form_ajax_callback',
      'wrapper' => 'time-entry-form',
      'method' => 'replace',
    );
    $form['#process'][] = 'time_entry_form_ajax_process';
  }

  return $form;
}

/**
 * Validate time entry creation form.
 */
function time_entry_form_validate($form, &$form_state) {
  $form_state['values']['time'] = strtotime($form_state['values']['start']);
  $form_state['values']['duration'] = strtotime($form_state['values']['end']) - $form_state['values']['time'];

  if ($form_state['values']['duration'] < 0) {
    form_set_error('start', t('Start date must be lower than end date.'));
  }

  if (empty($form_state['values']['start'])) {
    form_set_error($time_field, t('You have to specify a valid date.'));
  }
}

/**
 * Save submitted time entry.
 */
function time_entry_form_submit($form, &$form_state) {
  entity_form_submit_build_entity('time_entry', $form_state['time_entry'], $form, $form_state);
  entity_save('time_entry', $form_state['time_entry']);
}

/**
 * Ajax process for time_entry_form.
 *
 * When the form is regenerated, the form element gets a new id with a
 * incremented number to avoid conflicts. Since we are using the form as a
 * replacement wrapper, selecting it by it id, we need to change the wrapper
 * property of the ajax setting to reflect the new generated id.
 */
function time_entry_form_ajax_process($form, $form_state) {
  $form['actions']['submit']['#ajax']['wrapper'] = $form['#id'];

  return $form;
}

/**
 * Ajax callback response for time_entry_form.
 */
function time_entry_form_ajax_callback($form, $form_state) {
  $new_form_state = array(
    'build_info' => $form_state['build_info'],
    'rebuild' => TRUE,
    'values' => array()
  );
  $new_form_state += form_state_defaults();

  $new_form = drupal_rebuild_form('time_entry_form', $new_form_state);

  return drupal_render($new_form);
}

/**
 * Confirm delete form.
 */
function time_entry_delete_confirm($form, &$form_state, $time_entry) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $time_entry->id,
  );

  return confirm_form($form, t('Are you sure you want to delete this Time Entry?'), '<front>', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Delete confirmed time entry.
 */
function time_entry_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $time_entry = time_entry_load($form_state['values']['id']);

    time_entry_delete($time_entry);

    drupal_set_message(t('Time Entry has been deleted.'));
  }

  $form_state['redirect'] = '<front>';
}
