<?php

/**
 * @file
 * Redhen Campaign type editing UI.
 */

/**
 * Generates the Redhen Campaign type editing form.
 */
function redhen_campaign_type_form($form, &$form_state, $redhen_campaign_type, $op = 'edit') {
  if ($op == 'clone') {
    $redhen_campaign_type->label .= ' (cloned)';
    $redhen_campaign_type->name = '';
  }

  $form['#redhen_campaign_type'] = $redhen_campaign_type;

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $redhen_campaign_type->label,
    '#description' => t('The name of this RedHen Campaign type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($redhen_campaign_type->name) ? $redhen_campaign_type->name : '',
    '#maxlength' => 32,
    '#disabled' => $redhen_campaign_type->locked && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'redhen_campaign_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this redhen campaign type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional Settings'),
    '#tree' => TRUE,
  );
  $types = redhen_donation_get_types();
  $type_options = array('' => '-- select --');
  foreach ($types as $id => $type) {
    $type_options[$id] = $type->label;
  }
  $form['data']['donation_type'] = array(
    '#type' => 'select',
    '#title' => t('Default Donation Type'),
    '#description' => t('By defaulting the Donation Type and hiding the field with the checkbox below, you can dictate a specific Donation Type for all Campaigns of this type.'),
    '#options' => $type_options,
    '#default_value' => isset($redhen_campaign_type->donation_type) ? $redhen_campaign_type->donation_type : '',
  );
  $form['data']['hide_donation_type'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide donation type field on Campaigns from non-admin users.'),
    '#default_value' => isset($redhen_campaign_type->hide_donation_type) ? $redhen_campaign_type->hide_donation_type : FALSE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save campaign type'),
    '#weight' => 40,
  );

  if (!$redhen_campaign_type->locked && $op == 'edit') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete campaign type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('redhen_campaign_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function redhen_campaign_type_form_submit(&$form, &$form_state) {
  $redhen_campaign_type = entity_ui_form_submit_build_entity($form, $form_state);
  $redhen_campaign_type->save();
  drupal_set_message(t('Campaign type has been saved.'));
  menu_rebuild();
  $form_state['redirect'] = 'admin/structure/redhen/campaign_types';
}

/**
 * Form API submit callback for the delete button.
 */
function redhen_campaign_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/redhen/campaign_types/manage/' . $form_state['redhen_campaign_type']->name . '/delete';
}
