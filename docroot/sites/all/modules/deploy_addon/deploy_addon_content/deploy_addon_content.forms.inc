<?php

/**
 * @file
 * Form functions for the Deploy Add-on Content module.
 */

/**
 * Form function callback that adds extra functionality as user tab.
 */
function deploy_addon_content_user_adhoc_plan_form($form, &$form_state, $account) {

  // During initial form build, add the entity to the form state for use during
  // form building and processing. During a rebuild, use what is in the form
  // state.
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }
  else {
    $account = $form_state['user'];
  }

  $form = array();

  $plan = deploy_plan_load($account->name);

  if (!$plan) {
    drupal_set_message(t('You must <a href="@url">create a user deploy plan</a> first.', array('@url' => url('admin/config/content/deploy_addon/deploy_adhoc_plan'))), 'warning');
  }

  $form['adhoc'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Deploy Plan'),
  );

  $form['adhoc']['use_adhoc_plan_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set user deploy plan automatically on login'),
    '#description' => t('The user must have a user deploy plan created.'),
    '#default_value' => (!empty($account->data['deploy_adhoc_plan_auto_login']) ? 1 : 0),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler to save user's deploy preferences.
 */
function deploy_addon_content_user_adhoc_plan_form_submit($form, &$form_state) {

  $account = $form_state['user'];
  $edit = array();

  $edit['data']['deploy_adhoc_plan_auto_login'] = $form_state['values']['use_adhoc_plan_login'];

  user_save($account, $edit);
  drupal_set_message(t('Settings were saved.'));
}
