<?php

/**
 * @file
 * Profile type editing UI.
 */


/**
 * Generates the past_event type editing form.
 */
function past_event_type_form($form, &$form_state, $past_event_type, $op = 'edit') {

  if ($op == 'clone') {
    $past_event_type->label .= ' (cloned)';
    $past_event_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $past_event_type->label,
    '#description' => t('The human-readable name of this past_event type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($past_event_type->type) ? $past_event_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $past_event_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'past_event_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this past_event type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  // @todo do we need this?
  $form['data']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save past event type'),
    '#weight' => 40,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $past_event_type->weight,
    '#description' => t('When showing past_events, those with lighter (smaller) weights get listed before past_events with heavier (larger) weights.'),
    '#weight' => 10,
  );

  if (!$past_event_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete past_event type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('past_event_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function past_event_type_form_submit(&$form, &$form_state) {
  $past_event_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $past_event_type->save();
  $form_state['redirect'] = 'admin/config/development/past-types';
}

/**
 * Form API submit callback for the delete button.
 */
function past_event_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/config/development/past-types/manage/' . $form_state['past_event_type']->type . '/delete';
}

