<?php

/**
 * @file
 * Redhen Campaign Page type editing UI.
 */

/**
 * Generates the Redhen campaign page type editing form.
 */
function redhen_campaign_page_type_form($form, &$form_state, $redhen_campaign_page_type, $op = 'edit') {
  if ($op == 'clone') {
    $redhen_campaign_page_type->label .= ' (cloned)';
    $redhen_campaign_page_type->name = '';
  }

  $form['#redhen_campaign_page_type'] = $redhen_campaign_page_type;

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $redhen_campaign_page_type->label,
    '#description' => t('The name of this RedHen campaign page type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($redhen_campaign_page_type->name) ? $redhen_campaign_page_type->name : '',
    '#maxlength' => 32,
    '#disabled' => $redhen_campaign_page_type->locked && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'redhen_campaign_page_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this redhen campaign page type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional Settings'),
    '#tree' => TRUE,
  );
  $form['data']['page_type'] = array(
    '#type' => 'select',
    '#title' => t('Fundraising Page Type'),
    '#options' => array(
      REDHEN_CAMPAIGN_PAGE_INDIVIDUAL => 'Individual',
      REDHEN_CAMPAIGN_PAGE_TEAM => 'Team',
    ),
    '#default_value' => isset($redhen_campaign_page_type->page_type) ? $redhen_campaign_page_type->page_type : array(),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save campaign page type'),
    '#weight' => 40,
  );

  if (!$redhen_campaign_page_type->locked && $op == 'edit') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete campaign page type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('redhen_campaign_page_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function redhen_campaign_page_type_form_submit(&$form, &$form_state) {
  $redhen_campaign_page_type = entity_ui_form_submit_build_entity($form, $form_state);
  $redhen_campaign_page_type->data = $form_state['input']['data'];
  $redhen_campaign_page_type->save();
  drupal_set_message(t('Campaign page type has been saved.'));
  menu_rebuild();
  $form_state['redirect'] = 'admin/structure/redhen/page_types';
}

/**
 * Form API submit callback for the delete button.
 */
function redhen_campaign_page_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/redhen/page_types/manage/' . $form_state['redhen_campaign_page_type']->name . '/delete';
}
