<?php

/**
 * @file
 * Party Activity Type editing UI
 */

/**
 * UI Controller
 */
class PartyActivityTypeUIController extends EntityDefaultUIController {

}

/**
 * Generates the party activty type editing form.
 */
function party_activity_type_form($form, &$form_state, $activity_type, $op = 'edit') {

  if ($op == 'clone') {
    $activity_type->label .= ' (cloned)';
    $activity_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $activity_type->label,
    '#description' => t('The human-readable name of this activity type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($activity_type->type) ? $activity_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'party_activity_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this party activity type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function party_activity_type_form_submit(&$form, &$form_state) {
  $activity_type = entity_ui_form_submit_build_entity($form, $form_state);
  $activity_type->save();
  $form_state['redirect'] = 'admin/structure/activity_types';
}

/**
 * Form API submit callback for the delete button.
 */
function party_activity_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/activity_types/manage/' . $form_state['activity_type']->type . '/delete';
}
