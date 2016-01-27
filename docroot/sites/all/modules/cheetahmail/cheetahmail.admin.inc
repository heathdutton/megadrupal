<?php
/**
 * @file
 * Cheetahmail admin forms
 */

require_once 'cheetahmail.api.inc';

/**
 * Provide Cheetahmail API settings form
 */
function cheetahmail_admin_menu_api_form() {
  $form = array();

  $activated = variable_get('experian/cheetahmail/isactivated', FALSE);

  $form['experian/cheetahmail/api/idmlist'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Base Number'),
    '#description'   => t('Enter IdMlist for Experian Cheetahmail.'),
    '#default_value' => variable_get('experian/cheetahmail/api/idmlist', ''),
    '#required'      => TRUE,
    '#disabled'      => $activated,
  );

  $form['experian/cheetahmail/api/username'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Username Api'),
    '#description'   => t('Enter Username for Experian Cheetahmail.'),
    '#default_value' => variable_get('experian/cheetahmail/api/username', ''),
    '#required'      => TRUE,
    '#disabled'      => $activated,
  );

  $form['experian/cheetahmail/api/password'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Api Key'),
    '#description'   => t('Enter Api Key for Experian Cheetahmail.'),
    '#default_value' => variable_get('experian/cheetahmail/api/password', ''),
    '#required'      => TRUE,
  );

  if ($activated) {
    $form['actions']['cheetahmail_uninstall'] = array(
      '#type'          => 'submit',
      '#value'         => t('Uninstall'),
      '#submit'        => array('cheetahmail_api_settings_form_uninstall_submit'),
    );

    $form['actions']['cheetahmail_selftest'] = array(
      '#type'          => 'submit',
      '#value'         => t('Self Test'),
      '#submit'        => array('cheetahmail_api_settings_form_selftest_submit'),
    );
  }

  $form = system_settings_form($form);
  $form['#validate'] = array('cheetahmail_api_settings_form_activate_validate');

  return $form;
}

/**
 * Validation callback for `cheetahmail_admin_menu_api_form` form.
 * Test connection with EMS API using specified $configs.
 */
function cheetahmail_api_settings_form_activate_validate(&$form, &$form_state) {
  if (!empty($form_state['clicked_button']['#id']) &&  ('edit-cheetahmail-uninstall' == $form_state['clicked_button']['#id'])) {
    return;
  }

  $values = $form_state['values'];

  $configs = array(
    'IdMlist'   => $values['experian/cheetahmail/api/idmlist'],
    'UserName'  => $values['experian/cheetahmail/api/username'],
    'Password'  => $values['experian/cheetahmail/api/password'],
  );

  $client = new EMSWebService('subscribers', $configs);
  $result = $client->GetFieldsDefinition();

  if ($result) {
    drupal_set_message(t('Test Connection to EMS API succedded.'), $type = 'status');
    if (!variable_get('experian/cheetahmail/isactivated', FALSE)) {
      variable_set('experian/cheetahmail/isactivated', TRUE);
      variable_set('experian/cheetahmail/api/idmlist', $values['experian/cheetahmail/api/idmlist']);
      variable_set('experian/cheetahmail/api/username', $values['experian/cheetahmail/api/username']);
      variable_set('experian/cheetahmail/api/password', $values['experian/cheetahmail/api/password']);
      module_invoke_all('cheetahmail_activated');
      menu_rebuild();
    }
    else {
      variable_set('experian/cheetahmail/api/idmlist', $values['experian/cheetahmail/api/idmlist']);
      variable_set('experian/cheetahmail/api/username', $values['experian/cheetahmail/api/username']);
      variable_set('experian/cheetahmail/api/password', $values['experian/cheetahmail/api/password']);
    }

    cheetahmail_api_get_helper()->self_test();
    return;
  }
  else {
    form_set_error('experian/cheetahmail/api/password', t('Test Connection to EMS API failed.'));
  }
}

/**
 * Submit callback for Uninstall button
 * of `cheetahmail_admin_menu_api_form` form.
 */
function cheetahmail_api_settings_form_uninstall_submit(&$form, &$form_state) {
  cheetahmail_api_delete_all_configs();
  menu_rebuild();
  drupal_set_message(t('Experian Cheetahmail configuration is deleted.'), $type = 'status');
}

/**
 * Submit callback for `Self Test` button
 * of `cheetahmail_admin_menu_api_form` form.
 */
function cheetahmail_api_settings_form_selftest_submit(&$form, &$form_state) {
  cheetahmail_api_get_helper()->self_test($show_success_msg = TRUE, $recheck_in_EMS = TRUE);
}

/**
 * Campaign configuration form.
 * (Registration Campaign).
 */
function cheetahmail_admin_menu_campaignconfigs_form() {
  $configs = cheetahmail_api_get_helper()->_get_campaign_configs();

  $form = cheetahmail_api_get_campaign_configuration_form($configs);

  unset($form['vtabs']['messageParams']);

  // Subscribe/Unsubcribe emails.
  $form['vtabs']['subscribe_emails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscribe Email'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['vtabs']['subscribe_emails']['subscribeSubject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription Email Subject'),
    '#default_value' => $configs['subscribeSubject'],
    '#required' => TRUE,
  );

  $form['vtabs']['subscribe_emails']['subscribeEmailhtml'] = array(
    '#type' => 'textarea',
    '#title' => t('Subscription Email content HTML'),
    '#default_value' => $configs['subscribeEmailhtml'],
  );

  $form['vtabs']['subscribe_emails']['subscribeEmailtxt'] = array(
    '#type' => 'textarea',
    '#title' => t('Subscription Email content TXT'),
    '#default_value' => $configs['subscribeEmailtxt'],
  );

  $form['vtabs']['unsubscribe_emails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unsubscribe Email'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['vtabs']['unsubscribe_emails']['unsubscribeSubject'] = array(
    '#type' => 'textfield',
    '#title' => t('Unsubscription Email Subject'),
    '#default_value' => $configs['unsubscribeSubject'],
    '#required' => TRUE,
  );

  $form['vtabs']['unsubscribe_emails']['unsubscribeEmailhtml'] = array(
    '#type' => 'textarea',
    '#title' => t('Unsubscription Email content HTML'),
    '#default_value' => $configs['unsubscribeEmailhtml'],
  );

  $form['vtabs']['unsubscribe_emails']['unsubscribeEmailtxt'] = array(
    '#type' => 'textarea',
    '#title' => t('Unsubscription Email content TXT'),
    '#default_value' => $configs['unsubscribeEmailtxt'],
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validation callback for cheetahmail_admin_menu_campaignconfigs_form.
 */
function cheetahmail_admin_menu_campaignconfigs_form_validate(&$form, &$form_state) {
  if (!valid_email_address($form_state['values']['mailReply'])) {
    form_set_error('mailReply', 'Please, provide a valid email address.');
  }

  if (!valid_email_address($form_state['values']['mailRetNpai'])) {
    form_set_error('mailRetNpai', 'Please, provide a valid email address.');
  }
  $mail_from_addr = empty($form_state['values']['mailFromAddr']) ? '' : $form_state['values']['mailFromAddr'] . '@example.com';
  if (!valid_email_address($mail_from_addr)) {
    form_set_error('mailFromAddr', 'Please, provide a valid email address.');
  }
}

/**
 * Submit Callback for `cheetahmail_admin_menu_campaignconfigs_form` form
 */
function cheetahmail_admin_menu_campaignconfigs_form_submit(&$form, &$form_state) {
  $configs = array_merge(array(), $form_state['values']);

  unset($configs['op']);
  unset($configs['form_build_id']);
  unset($configs['form_token ']);
  unset($configs['form_id']);
  unset($configs['submit']);

  $configs['idConf'] = cheetahmail_api_get_helper()->_get_default_configId();

  if (cheetahmail_api_get_helper()->ems_configs_UpdateConfig($configs)) {
    $domains = cheetahmail_api_get_helper()->ems_configs_ListDomain();
    $config_id = cheetahmail_api_get_helper()->_get_default_configId();
    if (!empty($domains[$configs['domain']])) {
      $domain_id = cheetahmail_api_get_helper()->ems_configs_setDomain($config_id, $configs['domain']);
    }

    // Update Subscribe / Unsubscribe Email content.
    cheetahmail_api_get_helper()->_set_subscribing_emailTemplate($configs);
    cheetahmail_api_get_helper()->_set_unsubscribing_emailTemplate($configs);

    // Update ListUunsubscribe.
    $list_unsubscribe = cheetahmail_api_get_helper()->_get_default_listUnsubscribe($config_id);
    $list_unsubscribe = cheetahmail_api_get_helper()->ems_configs_SetListunsubscribe($list_unsubscribe);

    drupal_set_message(t('The settings are saved.'), $type = "status");
    return;
  }

  drupal_set_message(t('Failed to save the settings.'), $type = 'error');
}
