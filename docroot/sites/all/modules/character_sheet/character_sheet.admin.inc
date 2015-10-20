<?php

/**
 * @file
 * Adds administrative settings for character sheet, moved here to save on code footprint.
 */

/**
 * Implementation of admin settings form.
 */
function character_sheet_admin_settings($form, &$form_state) {
  $workflows = workflow_get_workflows(); // From workflow.module
  $options = array();
  foreach ($workflows as $workflow) {
    $options[$workflow->name] = $workflow->name;
  }
  $default = _character_sheet_get_workflow();
  $wid = isset($default->wid) ? $default->wid : '';
  if (empty($wid)) {
    drupal_set_message(t('You need a workflow for your character sheets. One should have been ' .
      'built out as part of this feature install. Please refresh your feature or be sure to select ' .
      'as default workflow for your character sheets.'), 'error');
  }
  $form['character_sheet_default_workflow'] = array(
    '#title' => t('Default character evaluation workflow'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => isset($default->name) ? $default->name : '',
    '#description' => t('Character sheets provides moderation features via the Workflow module. ' .
    'Check that your character sheet permissions are set correctly for user roles under !workflow.',
    array('!workflow' => l(t('the Workflow module'), 'admin/config/workflow/workflow/edit/' . $wid))),
  );
  $systems = module_invoke_all('character_sheet_register_systems');
  if (count($systems) > 0) {
    foreach ($systems as $key => $system) {
      $system_keys[$key] = $system['name'] . ' : ' . $system['description'];
    }
    $default_systems = variable_get('character_sheet_systems', array());
    $form['character_sheet_systems'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Enable game systems'),
      '#required' => TRUE,
      '#default_value' => $default_systems,
      '#options' => $system_keys,
      '#description' => t('Select the systems available to use with character sheets.'),
    );
  }
  if (!empty($default_systems)) {
    $workflow_options = array();
    $states = workflow_get_workflow_states_by_wid($default->wid);
    foreach ($states as $state) {
      $workflow_options[$state->sid] = $default->name . ':' . $state->state;
    }
    foreach ($default_systems as $system) {
      $form[$system] = array(
        '#type' => 'fieldset',
        '#title' => $systems[$system]['name'],
        '#description' => $systems[$system]['description'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form[$system]['character_sheet_' . $system . '_playable'] = array(
        '#title' => 'Playable states',
        '#description' => 'Workflow state(s) for playable characters. Character sheets in this state may ' .
          'be improved with XP and access to field values are limited by system field permissions.',
        '#type' => 'select',
        '#options' => $workflow_options,
        '#multiple' => TRUE,
        '#default_value' => variable_get('character_sheet_' . $system . '_playable', ''),
      );
      if (isset($systems[$system]['admin_callback']) && function_exists($systems[$system]['admin_callback'])) {
        $additional_elements = call_user_func_array($systems[$system]['admin_callback'], array());
        foreach ($additional_elements as $key => $element) {
          $form[$system][$key] = $element;
        }
      }
    }
  }
  return system_settings_form($form);
}
