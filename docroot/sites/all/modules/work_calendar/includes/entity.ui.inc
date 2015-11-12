<?php

/**
 * @file
 * Work calendar editing UI.
 */

/**
 * Generates the work_canlendar editing form.
 *
 * @see work_calendar_form_submit().
 * @see work_calendar_form_submit_delete().
 */
function work_calendar_form($form, &$form_state, $work_calendar, $op = 'edit') {
  if ($op == 'clone') {
    $work_calendar->label .= ' (cloned)';
    $work_calendar->name .= '_clone';
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $work_calendar->label,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $work_calendar->name,
    '#disabled' => $work_calendar->isLocked() && $op != 'clone',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'work_calendar_load',
      'source' => array('label'),
     ),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $work_calendar->description,
  );
  $week_days_keys   = date_week_days_ordered(date_week_days_untranslated());
  $week_days_values = date_week_days_ordered(date_week_days(TRUE));
  $form['week_days'] = array(
    '#type' => 'checkboxes',
    '#options' => array_combine($week_days_keys, $week_days_values),
    '#default_value' => $work_calendar->week_days,
    '#title' => t('Choose opening week days'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save work calendar'),
    '#weight' => 40,
  );
  if (!$work_calendar->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete work calendar'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('work_calendar_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Form API submit callback for the save button.
 *
 * @see work_calendar_form().
 */
function work_calendar_form_submit($form, &$form_state) {
  $work_calendar = entity_ui_form_submit_build_entity($form, $form_state);
  // Update availability before save.
  $work_calendar->updateWeek($form_state['values']['week_days']);
  // Save and go back.
  $work_calendar->save();
  $form_state['redirect'] = 'admin/structure/work-calendars';
}

/**
 * Form API submit callback for the delete button.
 *
 * @see work_calendar_form().
 */
function work_calendar_form_submit_delete($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/work-calendars/manage/' . $form_state['work_calendar']->name . '/delete';
}
