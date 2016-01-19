<?php

/**
 * The entity recur form
 */
function entity_recur_entity_recur_form($form, $form_state, $entity_type, $entity) {
  // Check if the form was already submitted
  if (isset($form_state['values'])) {
    return entity_recur_entity_recur_confirm($form, $form_state, $entity_type, $entity);
  }

  // Display this entity's date
  $start = entity_recur_get_entity_date_field_value($entity_type, $entity);
  $end = entity_recur_get_entity_date_field_value($entity_type, $entity, FALSE);
  $form['#tree'] = TRUE;
  $form['entity_date'] = array(
    '#type' => 'item',
    '#title' => t('Date'),
    '#markup' => entity_recur_format_date($start, $end),
  );
  $form['entity_recur'] = array(
    '#type' => 'container',
    '#attributes' => array(),
  );
  $form['entity_recur'] = array_merge($form['entity_recur'], _entity_recur_entity_recur_form($entity_type, $entity));

  // Remove the "no repeat" option
  unset($form['entity_recur']['option']['#options']['none']);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );

  $form['#entity'] = $entity;
  $form['#entity_type'] = $entity_type;

  return $form;
}

/**
 * Helper function to provide the basics of the form
 *
 * This helps make it available for other modules to use when we might
 * not have a full entity yet
 *
 * @param $entity_type
 *   The entity type
 * @param $entity
 *   The entity
 */
function _entity_recur_entity_recur_form($entity_type, $entity) {
  $form = array();
  $settings = entity_recur_get_settings($entity_type, $entity);
  $form['option'] = array(
    '#type' => 'radios',
    '#options' => array(
      'none' => t('Do not repeat'),
      'days' => t('Pick days of the week'),
      'rules' => t('Every day, every 2 weeks, etc...'),
    ),
    '#default_value' => 'days',
    '#description' => t('Selecting a choice above will reveal more options.'),
  );

  $form['days'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Days of the week'),
    '#options' => array(
      'monday' => t('Monday'),
      'tuesday' => t('Tuesday'),
      'wednesday' => t('Wednesday'),
      'thursday' => t('Thursday'),
      'friday' => t('Friday'),
      'saturday' => t('Saturday'),
      'sunday' => t('Sunday'),
    ),
    '#states' => array(
      'visible' => array(':input[name="entity_recur[option]"]' => array('value' => 'days')),
    ),
  );

  $form['rules'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(':input[name="entity_recur[option]"]' => array('value' => 'rules')),
    ),
  );

  $options = array();
  for ($i = 1; $i < 11; $i++) {
    $options[$i] = t('Every !i', array('!i' => ($i == 1) ? '' : $i));
  }
  $form['rules']['frequency'] = array(
    '#type' => 'select',
    '#title' => t('Repeat'),
    '#options' => $options,
  );

  $form['rules']['period'] = array(
    '#type' => 'select',
    '#options' => array(
      'day' => t('Day(s)'),
      'week' => t('Week(s)'),
      'month' => t('Month(s)'),
    ),
  );

  $form['rules']['exclude_weekends'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude weekends'),
    '#description' => t('If checked, weekends will not be included.'),
  );

  $form['until'] = array(
    '#type' => module_exists('date_popup') ? 'date_popup' : 'date_text',
    '#title' => t('Recur until'),
    '#description' => t('Repeat this class until the specified date.'),
    '#required' => TRUE,
    '#date_format' => DATE_FORMAT_DATE,
    '#states' => array(
      'required' => array(':input[name="entity_recur[option]"]' => array(array('value' => 'rules'), array('value' => 'days'))),
      'visible' => array(':input[name="entity_recur[option]"]' => array(array('value' => 'rules'), array('value' => 'days'))),
    ),
  );
  if ($settings->max_duration) {
    $form['until']['#description'] .= '&nbsp;' . t('This date can only be up to %max in the future.', array('%max' => $settings->max_duration));
  }

  $form['#recurring_entity'] = $entity;
  $form['#recurring_entity_type'] = $entity_type;
  return $form;
}

/**
 * Validate the entity recur form
 */
function entity_recur_entity_recur_form_validate(&$form, &$form_state) {
  // If this is the confirm form, then skip validation
  if (isset($form_state['values']['confirm'])) {
    return;
  }
  // If option is set to nothing, then skip validation
  if ($form_state['values']['entity_recur']['option'] == 'none') {
    return;
  }

  $entity = $form['#entity'];
  $entity_type = $form['#entity_type'];
  $settings = entity_recur_get_settings($entity_type, $entity);

  // If days option is selected, make sure we have at least one day
  if ($form_state['values']['entity_recur']['option'] == 'days') {
    $days = 0;
    foreach ($form_state['values']['entity_recur']['days'] as $day => $value) {
      if ($value) {
        $days++;
      }
    }
    if ($days == 0) {
      form_error($form['entity_recur']['days'], t('At least one day must be selected.'));
    }
  }

  // Check until date format
  $until = strtotime($form_state['values']['entity_recur']['until']);
  if (!is_numeric($until)) {
    form_error($form['entity_recur']['until'], t('You must supply a valid end date.'));
    // Stop here
    return;
  }
  // Make sure until date is in the future
  if (!$settings->allow_past && ($until < REQUEST_TIME)) {
    form_error($form['entity_recur']['until'], t('The end date must be in the future.'));
  }
  // Check that until date isn't too far in the future, according
  // to entity settings (if any)
  if ($settings->max_duration) {
    if ($until > strtotime($settings->max_duration, REQUEST_TIME)) {
      form_error($form['entity_recur']['until'], t('The end date can only be up to %max in the future.', array('%max' => $settings->max_duration)));
    }
  }

  // If there were no errors, allow other modules to validate the dates
  if (!form_get_errors()) {
    $errors = module_invoke_all('entity_recur_validate_dates', $entity_type, $entity, $form_state);
    foreach ($errors as $error) {
      form_set_error($error['field'], $error['message']);
    }
  }
}

/**
 * Submit the entity recur form
 */
function entity_recur_entity_recur_form_submit(&$form, &$form_state) {
  // Rebuild so we can present the confirm form
  $form_state['rebuild'] = TRUE;
}

/**
 * Confirm form for entity recur
 */
function entity_recur_entity_recur_confirm($form, $form_state, $entity_type, $entity) {
  $dates = entity_recur_generate_dates_from_form($entity_type, $entity, $form_state);

  // Make sure we have dates to work with
  if (empty($dates['start'])) {
    drupal_set_message(t('No dates were generated with the information you supplied.'), 'warning');
    //drupal_goto("entity/{$entity->nid}/recur");
  }

  // Store the dates
  $form['#start_dates'] = $dates['start'];
  $form['#end_dates'] = $dates['end'];

  // Display the dates to the user
  $form['message'] = array(
    '#markup' => t('The following dates will be generated. Please review them before continuing.'),
  );
  $form['dates']['#markup'] = '<ul>';
  foreach ($dates['start'] as $key => $start_date) {
    $end_date = isset($dates['end'][$key]) ? $dates['end'][$key] : NULL;
    $form['dates']['#markup'] .= '<li>' . entity_recur_format_date($start_date, $end_date) . '</li>';
  }
  $form['dates']['#markup'] .= '</ul>';

  // Store the entity
  $form['entity_recur'] = array(
    '#type' => 'container',
    '#attributes' => array(),
  );
  $form['entity_recur']['#recurring_entity'] = $entity;
  $form['entity_recur']['#recurring_entity_type'] = $entity_type;


  // Add the submit handler
  $form['#submit'][] = 'entity_recur_entity_recur_confirm_submit';

  // Entity bundle name
  $bundle_name = strtolower(_entity_recur_get_bundle_name($entity_type, $entity));
  $uri = entity_uri($entity_type, $entity);
  // Supply the confirm form
  return confirm_form(
    $form,
    t('Are you sure you want to generate these items?'),
    $uri['path'],
    '<strong>' . t('This action cannot be undone. Please confirm that the dates above are accurate and that the %type information is correct. Editing this %type afterwards will not edit every %type generated here.', array('%type' => $bundle_name)) . '</strong>',
    t('Submit')
  );
}

/**
 * Submit the entity recur confirmation form
 */
function entity_recur_entity_recur_confirm_submit($form, &$form_state) {
  $entity = $form['entity_recur']['#recurring_entity'];
  $entity_type = $form['entity_recur']['#recurring_entity_type'];
  $settings = entity_recur_get_settings($entity_type, $entity);
  $bundle_name = _entity_recur_get_bundle_name($entity_type, $entity);
  // Log this action
  watchdog('entity_recur', 'Recurring the %type "%title" %count times.', array('%type' => $bundle_name, '%title' => check_plain(entity_label($entity_type, $entity)), '%count' => count($form['#start_dates'])));
  if (count($form['#start_dates']) < $settings->batch_limit) {
    foreach ($form['#start_dates'] as $key => $start_date) {
      entity_recur_entity_create_entity($entity_type, $entity, $start_date, isset($form['#end_dates'][$key]) ? $form['#end_dates'][$key] : NULL);
    }
  }
  else {
    module_load_include('batch.inc', 'entity_recur');
    entity_recur_entity_batch_start($entity_type, $entity, $form['#start_dates'], $form['#end_dates']);
  }
}

/**
 * Get the human-readable name for an entity bundle.
 *
 * @param $entity_type
 *   The entity type
 * @param $entity
 *   The entity
 */
function _entity_recur_get_bundle_name($entity_type, $entity) {
  $entity_info = entity_get_info($entity_type);
  list(, , $bundle) = entity_extract_ids($entity_type, $entity);
  if ($entity_info && isset($entity_info['bundles'][$bundle]['label'])) {
    return $entity_info['bundles'][$bundle]['label'];
  }
  return $bundle;
}
