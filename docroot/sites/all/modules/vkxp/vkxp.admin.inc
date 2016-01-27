<?php

/**
 * @file vkxp.admin.inc
 * Contains VKXP settings form.
 */

/**
 * Page callback.
 * Return form with VKXP main settings.
 */
function vkxp_admin_main_settings($form, &$form_state) {

  $form['vkxp_access_token'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access token'),
    '#collapsible' => TRUE,
  );

  $form['vkxp_access_token']['status'] = array(
    '#type' => 'markup',
    '#markup' => variable_get('vkxp_access_token') ?
      '<span style="color:green;">' . t('Recieved') . '</span>' :
      '<span style="color:red;">' . t('Not recieved') . '</span>'
  );

  // Application settings.
  $form['vkxp_application'] = array(
    '#type' => 'fieldset',
    '#title' => t('Application settings'),
    '#collapsible' => TRUE,
    '#description' => t('Note: You have to <a href="@url" target="_blank">create</a> a <strong>standalone</strong> application to make this module work.',
      array('@url' => 'http://vk.com/editapp?act=create')),
  );

  $form['vkxp_application']['vkxp_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#description' => t('ID of vk application.'),
    '#default_value' => variable_get('vkxp_app_id'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['vkxp_application']['vkxp_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Application secret code'),
    '#description' => t('Secret code of vk application.'),
    '#default_value' => variable_get('vkxp_app_secret'),
    '#required' => TRUE,
  );

  // Owner settings.
  $form['vkxp_owner'] = array(
    '#type' => 'fieldset',
    '#title' => t('Owner settings'),
    '#description' => t('Set user or group to which data will be transferred.'),
    '#collapsible' => TRUE,
  );

  $form['vkxp_owner']['vkxp_owner_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Owner ID'),
    '#description' => t('User ID or Group ID.'),
    '#default_value' => variable_get('vkxp_owner_id', ''),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['vkxp_owner']['vkxp_wall_owner'] = array(
    '#type' => 'select',
    '#title' => t('Select owner type'),
    '#description' => t('Who owns the ID above.'),
    '#options' => array(
      'group' => t('Group'),
      'user' => t('User'),
    ),
    '#default_value' => variable_get('vkxp_wall_owner', 'group'),
  );

  // Additional settings.
  $form['vkxp_additional'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional settings'),
    '#collapsible' => TRUE,
  );

  $form['vkxp_additional']['vkxp_official'] = array(
    '#type' => 'checkbox',
    '#title' => t('Write from group name'),
    '#description' => t('Check this if you want to post messages from group name.'),
    '#default_value' => variable_get('vkxp_official', TRUE),
  );

  $form['vkxp_additional']['vkxp_add_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add link on wall to posted page'),
    '#default_value' => variable_get('vkxp_add_link', FALSE),
    '#description' => t('Check this if you want to post node url on vk wall.'),
  );

  return system_settings_form($form);
}

/**
 * Validates main settings form.
 */
function vkxp_admin_main_settings_validate($form, &$form_state) {
  $values = &$form_state['values'];

  // Remove spaces. Just in case of small user mistake.
  $values['vkxp_app_id']     = trim($values['vkxp_app_id']);
  $values['vkxp_app_secret'] = trim($values['vkxp_app_secret']);
  $values['vkxp_app_secret'] = trim($values['vkxp_app_secret']);
}

/**
 * Form for recieving access token from VK.
 */
function vkxp_admin_access_token($form, &$form_state) {

  $html_id = drupal_html_id('access-token-form');
  $form['#prefix'] = '<div id="' . $html_id . '">';
  $form['#suffix'] = '</div>';

  // FIRST STEP.
  // Getting authorize code from VK.
  $params = array();
  $params['client_id']     = variable_get('vkxp_app_id');
  $params['scope']         = VKXP_AUTHORIZE_SCOPE;
  $params['redirect_uri']  = VKXP_AUTHORIZE_REDIRECT_URI;
  $params['response_type'] = VKXP_AUTHORIZE_RESPONSE_TYPE;

  $form['new_access_token'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recieve new access token'),
    '#collapsible' => TRUE,
    '#collapsed' => variable_get('vkxp_access_token') ? TRUE : FALSE,
  );

  $form['new_access_token']['link'] = array(
    '#theme' => 'link',
    '#text' => t('Get code'),
    '#path' => VKXP_AUTHORIZE_URI,
    '#options' => array(
      'query' => $params,
      'html' => FALSE,
      'attributes' => array(
        'target' => '_blank',
      ),
    ),
  );

  $form['new_access_token']['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Code'),
    '#description' => t('Copy #code param from the URL here.'),
  );

  $form['new_access_token']['actions'] = array(
    '#type' => 'action',
  );

  $form['new_access_token']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Get access token'),
  );

  return $form;
}

/**
 * Second step of getting access token.
 */
function vkxp_admin_access_token_submit($form, &$form_state) {

  // Collect params for authorization.
  // See http://vk.com/developers.php?oid=-1&p=%D0%90%D0%B2%D1%82%D0%BE%D1%80%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D1%8F_%D1%81%D0%B0%D0%B9%D1%82%D0%BE%D0%B2
  $params = array();
  $params['client_id']     = variable_get('vkxp_app_id');
  $params['client_secret'] = variable_get('vkxp_app_secret');
  $params['code']          = $form_state['values']['code'];
  $params['redirect_uri']  = VKXP_ACCESS_TOKEN_REDIRECT_URI;

  $data = vkxp_query('', $params, VKXP_ACCESS_TOKEN_URI);

  // Access token was recieved.
  if (!empty($data['access_token'])) {
    $message = array(
      'text'     => t('Access token was successfully recieved.'),
      'severity' => 'status',
    );
    _vkxp_watchdog($message);

    variable_set('vkxp_access_token', $data['access_token']);

    // Redirect user to the settings page.
    $form_state['redirect'] = 'admin/config/services/vkxp';
  }
  // Access token was not recieved.
  elseif (isset($data['error']) && isset($data['error_description'])) {
    $message = array(
      'text'     => t('Access token was not recieved. Reason: %error', array('%error' => check_plain($data['error_description']))),
      'severity' => 'error',
    );
    _vkxp_watchdog($message);

    // Remove variable if user not allowed to get a new access token.
    variable_del('vkxp_access_token');
  }

  return $form;
}
