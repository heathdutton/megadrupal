<?php

/**
 * @file
 * administration functions for ezmlm
 */

/**
 * Menu callback; presents the settings form for ezmlm
 */
function ezmlm_admin_settings() {
  // Add mailing list
  $form['ezmlm_add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add mailing lists'),
    '#description' => t('Add new lists'),
    '#collapsible' => TRUE,
  );
  $form['ezmlm_add']['ezmlm_add_name'] = array(
    '#type' => 'textfield',
    '#title' => t('New list name'),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => FALSE,
  );
  $form['ezmlm_add']['ezmlm_add_address'] = array(
    '#type' => 'textfield',
    '#title' => t('New list address'),
    '#size' => 30,
    '#maxlength' => 50,
    '#required' => FALSE,
  );

  $lists = _ezmlm_get_lists();

  if (count($lists)) {
    // List deletion fieldset
    $form['ezmlm_delete_group'] = array(
      '#type' => 'fieldset',
      '#title' => t('Remove mailing lists'),
      '#collapsible' => TRUE,
    );
    $options = array();
    foreach ($lists as $name => $email) {
      $options[$email] = "$name ($email)";
    }
    $form['ezmlm_delete_group']['ezmlm_delete'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#description' => 'Check the lists you want to remove',
    );

    // Registration form options
    $form['ezmlm_reg_group'] = array(
      '#type' => 'fieldset',
      '#title' => t('Registration settings'),
      '#description' => t('Allow new users to select mailing lists on registration'),
      '#collapsible' => TRUE,
    );
    $form['ezmlm_reg_group']['ezmlm_register'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display lists in registration form'),
      '#default_value' => variable_get('ezmlm_register', 1),
    );

    $reglist = _ezmlm_get_lists('REG');
    $checked = array();
    foreach ($reglist as $name => $email) {
      $checked[$email] = $email;
    }
    $form['ezmlm_reg_group']['ezmlm_reg_lists'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $checked,
      '#title' => 'Which lists should be displayed on the registration page?',
    );

    $form['ezmlm_reg_group']['ezmlm_reg_display'] = array(
      '#type' => 'radios',
      '#title' => t('List display type'),
      '#default_value' => variable_get('ezmlm_reg_display', 'email'),
      '#options' => array('email' => 'List address', 'name' => 'List name'),
      '#description' => t('Display %listaddress or %listname on the registration page', array('%listaddress' => 'list address', '%listname' => 'list name')),
    );
  }

  // Various settings
  $form['ezmlm_misc'] = array(
      '#type' => 'fieldset',
      '#title' => t('Various settings'),
      '#collapsible' => TRUE,
  );
  $form['ezmlm_misc']['ezmlm_title'] = array(
    '#type' => 'textfield',
    '#title' => t('List title'),
    '#description' => t('Configure the title of the subscription list'),
    '#default_value' => variable_get('ezmlm_title', t('Mailing lists')),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => FALSE,
  );
  $form['ezmlm_misc']['ezmlm_display'] = array(
    '#type' => 'radios',
    '#title' => t('List display type'),
    '#default_value' => variable_get('ezmlm_display', 'email'),
    '#options' => array('email' => 'List address', 'name' => 'List name'),
    '#description' => t('Display %listaddress or %listname on the subscription page', array('%listaddress' => 'list address', '%listname' => 'list name')),
  );
  $form['ezmlm_misc']['ezmlm_block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Block title'),
    '#description' => t('Configure the title of the block subscription list'),
    '#default_value' => variable_get('ezmlm_block_title', t('Subscriptions')),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => FALSE,
  );
  $form['ezmlm_misc']['ezmlm_block_display'] = array(
    '#type' => 'radios',
    '#title' => t('Block display type'),
    '#default_value' => variable_get('ezmlm_block_display', 'email'),
    '#options' => array('email' => 'List address', 'name' => 'List name'),
    '#description' => t('Display %listaddress or %listname on the block subscription page', array('%listaddress' => 'list address', '%listname' => 'list name')),
  );

  $form['#validate'][] = 'ezmlm_admin_settings_validate';
  $form['#submit'][] = 'ezmlm_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate form
 */
function ezmlm_admin_settings_validate($form, &$form_state) {
  $form_state['values']['ezmlm_add_name']    = trim($form_state['values']['ezmlm_add_name']);
  $form_state['values']['ezmlm_add_address'] = trim($form_state['values']['ezmlm_add_address']);

  if (drupal_strlen($form_state['values']['ezmlm_add_address'])) {
    if ($error = user_validate_mail($form_state['values']['ezmlm_add_address'])) {
      form_set_error('ezmlm_add_address', $error);
    }
    if (!drupal_strlen($form_state['values']['ezmlm_add_name'])) {
      form_set_error('ezmlm_add_name', t('You must enter a name for the mailing list.'));
    }
  }
}

/**
 * Submit form
 */
function ezmlm_admin_settings_submit($form, &$form_state) {
  // Various settings
  if (isset($form_state['values']['ezmlm_title']) && $form_state['values']['ezmlm_title'] != "") {
    variable_set('ezmlm_title', $form_state['values']['ezmlm_title']);
  }
  if (isset($form_state['values']['ezmlm_block_title']) && $form_state['values']['ezmlm_block_title'] != "") {
    variable_set('ezmlm_block_title', $form_state['values']['ezmlm_block_title']);
  }

  // Remove deleted lists
  $lists = _ezmlm_get_lists();
  foreach ($lists as $name => $email) {
    if ($form_state['values']['ezmlm_delete'][$email]) {
      unset($lists[$name]);
      drupal_set_message(t('Removed list @name (@email).', array('@name' => $name, '@email' => $email)));
    }
  }
  variable_set('ezmlmmailinglists', $lists);

  // Add list if necessary
  $list_text = $form_state['values']['ezmlm_add_name'];
  $list_email = $form_state['values']['ezmlm_add_address'];
  if (drupal_strlen($list_email)) {
    $lists[$list_text] = $list_email;
    variable_set('ezmlmmailinglists', $lists);
    drupal_set_message(t('The mailing list @name (@email) has been added.', array('@name' => $list_text, '@email' => $list_email)));
  }

  // Display on registration form?
  if (isset($form_state['values']['ezmlm_register'])) {
    variable_set('ezmlm_register', 1);
  }
  else {
    variable_set('ezmlm_register', 0);
  }

  // Which lists to display on registration form
  $reg_lists = array();
  foreach ($lists as $name => $email) {
    if (!empty($form_state['values']['ezmlm_reg_lists'][$email])) {
      $reg_lists[$name] = $email;
      drupal_set_message(t('Added list @name (@email) to registration page.', array('@name' => $name, '@email' => $email)));
    }
  }
  variable_set('ezmlm_register_list', $reg_lists);

  // Display list name or email?
  if (isset($form_state['values']['ezmlm_display'])) {
    variable_set('ezmlm_display', $form_state['values']['ezmlm_display']);
  }
  if (isset($form_state['values']['ezmlm_block_display'])) {
    variable_set('ezmlm_block_display', $form_state['values']['ezmlm_block_display']);
  }
  if (isset($form_state['values']['ezmlm_reg_display'])) {
    variable_set('ezmlm_reg_display', $form_state['values']['ezmlm_reg_display']);
  }
}
