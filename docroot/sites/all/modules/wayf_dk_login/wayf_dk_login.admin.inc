<?php
/**
 * @file
 * Admin form functions.
 */

/**
 * Configuration form.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function wayf_dk_login__settings_form() {

  $idp_metadata_default = wayf_dk_login__get_ipd_metadata('test');

  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['idp'] = array(
    '#type' => 'fieldset',
    '#title' => t('WAYF bridge'),
    '#group' => 'tabs',
  );

  $form['idp']['wayf_dk_login_mode'] = array(
    '#type' => 'select',
    '#title' => t('Service mode'),
    '#options' => array(
      'test' => t('Test'),
      'qa' => t('Quality assurance'),
      'production' => t('Production'),
    ),
    '#default_value' => variable_get('wayf_dk_login_mode', 'test'),
    '#ajax' => array(
      'callback' => 'wayf_dk_login__ajax_callback',
      'wrapper'  => 'idp-wrapper',
      'method'   => 'replace',
      'effect'   => 'fade',
    ),
  );

  $form['idp']['container'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="idp-wrapper">',
    '#suffix' => '</div>',
  );

  $form['idp']['container']['wayf_dk_login_idp_sso'] = array(
    '#type' => 'hidden',
    '#default_value' => variable_get('wayf_dk_login_idp_sso', $idp_metadata_default->sso),
  );

  $form['idp']['container']['wayf_dk_login_idp_sso_markup'] = array(
    '#type' => 'markup',
    '#markup' => '<p><strong>' . t('Single SignOn URL') . '</strong> : ' . variable_get('wayf_dk_login_idp_sso', $idp_metadata_default->sso) . '</p>',
  );

  $form['idp']['container']['wayf_dk_login_idp_slo'] = array(
    '#type' => 'hidden',
    '#default_value' => variable_get('wayf_dk_login_idp_slo', $idp_metadata_default->slo),
  );

  $form['idp']['container']['wayf_dk_login_idp_slo_markup'] = array(
    '#type' => 'markup',
    '#markup' => '<p><strong>' . t('Single Logout URL') . '</strong> : ' . variable_get('wayf_dk_login_idp_slo', $idp_metadata_default->slo) . '</p>',
  );

  $form['idp']['container']['wayf_dk_login_idp_certificate'] = array(
    '#type' => 'hidden',
    '#default_value' => variable_get('wayf_dk_login_idp_certificate', $idp_metadata_default->cert),
  );

  $form['idp']['container']['wayf_dk_login_idp_certificate_markup'] = array(
    '#type' => 'markup',
    '#prefix' => '<p></p><b>' . t('Certificate') . '</b></br><div style="width:700px;word-break:break-all;font-size:0.8em;">',
    '#markup' => variable_get('wayf_dk_login_idp_certificate', $idp_metadata_default->cert),
    '#suffix' => '</div></p>',
  );

  $form['sp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service provider'),
    '#group' => 'tabs',
  );

  $form['sp']['wayf_dk_login_sp_entityid'] = array(
    '#type' => 'textfield',
    '#title' => t('Connection ID'),
    '#default_value' => variable_get('wayf_dk_login_sp_entityid', ''),
    '#description' => t('EntityID used for the service.'),
  );

  $form['sp']['wayf_dk_login_sp_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('AssertionConsumerService:0:Location'),
    '#default_value' => variable_get('wayf_dk_login_sp_endpoint', ''),
    '#description' => t('Endpoint URL for the service.'),
  );

  $form['sp']['wayf_dk_login_sp_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Private key'),
    '#default_value' => variable_get('wayf_dk_login_sp_key', ''),
    '#description' => t('Private key, base64 PEM formatted. The private key should start with -----BEGIN RSA PRIVATE KEY----- and end with -----END RSA PRIVATE KEY-----'),
  );

  $form['sp']['wayf_dk_login_organizations_list_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Organizations feed URL'),
    '#default_value' => variable_get('wayf_dk_login_organizations_list_url', WAYF_DK_LOGIN_ORGANIZATIONS_LIST_URL),
    '#description' => t('The URL of the JSON feed with organizations.'),
  );

  // Get languages from the feed.
  $list  = variable_get('wayf_dk_login_organizations_list', array());
  $items = array_keys(reset($list));
  $options = array();
  foreach ($items as $item) {
    if ($item == 'schacHomeOrganization') {
      continue;
    }
    $options[$item] = $item;
  }

  $form['sp']['wayf_dk_login_organizations_name_language'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Organizations name language version'),
    '#default_value' => variable_get('wayf_dk_login_organizations_name_language', 0),
    '#description' => t('The language code of the orginazation names to use. If empty, run cron and make sure the feed URL is correct.'),
  );

  $form['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field mappings'),
    '#group' => 'tabs',
    '#description' => '<p>' . t('Notice: only textfields can be assigned attributes
      released from WAYF and currently only singular attributes are supported.
      The email address attribute are mapped to user->mail by default.') . '</p>',
  );

  $form['fields']['wayf_dk_login_field_mapping'] = array('#tree' => TRUE);

  $fields_info = field_info_instances('user', 'user');
  $wayf_dk_login_field_mapping = variable_get('wayf_dk_login_field_mapping', array());
  foreach ($fields_info as $field_name => $field) {
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'text') {
      $form['fields']['wayf_dk_login_field_mapping'][$field_name] = array(
        '#type' => 'select',
        '#options' => wayf_dk_login__wayf_attributes(),
        '#title' => check_plain($field['label']),
        '#default_value' => empty($wayf_dk_login_field_mapping[$field_name]) ? '' : $wayf_dk_login_field_mapping[$field_name],
      );
    }
  }

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#group' => 'tabs',
  );

  $form['display']['wayf_dk_login_loginform'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add WAYF login button to the standard login form'),
    '#default_value' => variable_get('wayf_dk_login_loginform', FALSE),
  );

  $icon_path = drupal_get_path('module', 'wayf_dk_login') . '/icons/';

  $icons = array();
  foreach (wayf_dk_login__icons() as $icon) {
    $icons[$icon] = theme('image', array('path' => $icon_path . $icon, 'attributes' => array('class' => 'wayf_dk_login-be-img')));
  }

  $form['display']['wayf_dk_login_icon'] = array(
    '#type' => 'radios',
    '#title' => t('Icon'),
    '#description' => t('Select icon.'),
    '#options' => $icons,
    '#default_value' => variable_get('wayf_dk_login_icon', 'UK_01.png'),
  );

  $form['organizations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Organizations'),
    '#group' => 'tabs',
  );

  // Retrieve relevant data for the organization list form.
  $list = variable_get('wayf_dk_login_organizations_list', array());
  $language = variable_get('wayf_dk_login_organizations_name_language', 0);
  $options = array();
  foreach ($list as $key => $value) {
    $options[$value['schacHomeOrganization']] = $value[$language];
  }
  asort($options);

  $active_list = variable_get('wayf_dk_login_organizations_active', array());

  $form['organizations']['wayf_dk_login_organizations_active'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed organizations'),
    '#options' => $options,
    '#default_value' => $active_list,
    '#description' => t('Select the organizations whose users should be able to log in. If none are checked, users from everywhere can log in.'),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#group' => 'tabs',
  );

  $form['settings']['development'] = array(
    '#type' => 'fieldset',
    '#title' => t('Development'),
  );

  $form['settings']['development']['wayf_dk_login_log_auth_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log authentication requests'),
    '#default_value' => variable_get('wayf_dk_login_log_auth_data', FALSE),
    '#description' => t('Log authentication data including attributes. This can be useful for debugging.'),
  );

  $form['#attached']['css'] = array(drupal_get_path('module', 'wayf_dk_login') . '/admin.css');

  return system_settings_form($form);
}

/**
 * Function wayf_dk_login__wayf_attributes returns list of WAYF attributes.
 *
 * @return [array]
 *   list of singular attributes
 */
function wayf_dk_login__wayf_attributes() {

  $attributes = array(
    '' => 'Not mapped',
    'urn:oid:2.5.4.4'  => t('Last name'),
    'urn:oid:2.5.4.42' => t('First name'),
    'urn:oid:2.5.4.3'  => t('Nickname'),
    'urn:oid:2.5.4.10' => t('Organisation nickname'),
    'urn:oid:1.3.6.1.4.1.5923.1.1.1.5' => t('Primary user affiliation'),
  );

  return $attributes;

}

/**
 * Function wayf_dk_login__ajax_callback.
 *
 * @param [array] $form
 *   FAPI form array
 * @param [array] $form_state
 *   current form state
 *
 * @return [array]
 *   the form element
 */
function wayf_dk_login__ajax_callback($form, &$form_state) {
  $idp_metadata = wayf_dk_login__get_ipd_metadata($form_state['values']['wayf_dk_login_mode']);

  $form['idp']['container']['wayf_dk_login_idp_sso']['#value'] = $idp_metadata->sso;
  $form['idp']['container']['wayf_dk_login_idp_sso_markup']['#markup'] = $idp_metadata->sso;

  $form['idp']['container']['wayf_dk_login_idp_slo']['#value'] = $idp_metadata->slo;
  $form['idp']['container']['wayf_dk_login_idp_slo_markup']['#markup'] = $idp_metadata->slo;

  $form['idp']['container']['wayf_dk_login_idp_certificate']['#value'] = $idp_metadata->cert;
  $form['idp']['container']['wayf_dk_login_idp_certificate_markup']['#markup'] = $idp_metadata->cert;

  return $form['idp']['container'];
}
