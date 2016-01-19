<?php

function time_entry_type_form($form, &$form_state, $time_entry_type, $op = 'edit') {
  if ($op == 'clone') {
    $time_entry_type->label .= ' (cloned)';
    $time_entry_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $time_entry_type->label,
    '#description' => t('The human-readable name of this profile type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($time_entry_type->type) ? $time_entry_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $time_entry_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'time_entry_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this profile type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile type'),
    '#weight' => 40,
  );

  if (!$time_entry_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete profile type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('time_entry_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function time_entry_type_form_submit(&$form, &$form_state) {
  $profile_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $profile_type->save();
  $form_state['redirect'] = 'admin/structure/time_entries';
}

/**
 * Form API submit callback for the delete button.
 */
function time_entry_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/time_entries/manage/' . $form_state['time_entry_type']->type . '/delete';
}


