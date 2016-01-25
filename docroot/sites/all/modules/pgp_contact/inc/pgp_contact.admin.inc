<?php
/**
 * @file
 * Admin settings form for the PGP Contact Forms module
 */

/**
 * Settings form for admin section.
 */
function pgp_contact_settings() {
  module_load_include('inc', 'pgp_contact', 'inc/pgp_contact.forms');

  _pgp_contact_warnings();

  $form = array();

  $form['pgp_contact_library_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to library'),
    '#default_value' => variable_get('pgp_contact_library_path', 'sites/all/libraries/openpgpjs'),
    '#size' => 128,
    '#maxlength' => 128,
    '#description' => t("Path to OpenPGP.js library downloaded from !link.", array('!link' => l(t('GitHub'), 'https://github.com/openpgpjs/openpgpjs/releases/latest'))),
    '#required' => TRUE,
  );

  // Adjust the library path element if libraries API is being used.
  if (module_exists('libraries')) {
    $form['pgp_contact_library_path']['#disabled'] = TRUE;
    $form['pgp_contact_library_path']['#description'] = t('This library is being managed by !link', array('!link' => l(t('Libraries API'), 'http://drupal.org/project/libraries')));
  }

  $form['pgp_contact_force_ssl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force SSL on all contact pages and key upload pages.'),
    '#default_value' => variable_get('pgp_contact_force_ssl', 0),
    '#description' => t('This will prevent users from using unencrypted pages to upload their keys or send encrypted messages'),
    '#required' => FALSE,
  );

  $form['pgp_contact_policy_wrapper'] = _pgp_contact_policy_field('pgp_contact_sitewide_policy');

  $form['pgp_contact_policy_wrapper']['pgp_contact_sitewide_policy']['#options'] = array(
    'user' => t('Allow individual policies per form.'),
    'always' => t('Force encryption on this site for all forms with public keys assigned to them.'),

  );

  $form['tradeoff_features'] = array(
    '#tree' => FALSE,
    '#type' => 'fieldset',
    '#title' => t('Trade-Offs'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['tradeoff_features']['intro_text'] = array(
    '#value' => '<p>' . t('These features can compromise the security of encrypted data sent via forms, but can be convenient for certain use cases.') . '</p>',
    '#type' => 'markup',
  );

  $form['tradeoff_features']['pgp_contact_disable_ssl_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the unsecured connection warning'),
    '#default_value' => variable_get('pgp_contact_disable_ssl_check', 0),
    '#description' => t("This will hide the unsecured connection warning on contact forms. A better way of dealing with this problem is to use SSL on your site."),
    '#required' => FALSE,
  );

  $form['#submit'][] = 'pgp_contact_set_policy';

  return system_settings_form($form);
}

/**
 * Set policies for all keys on a submission of always for the sitewide policy
 */
function pgp_contact_set_policy ($form, &$form_state) {
  if ($form_state['values']['pgp_contact_sitewide_policy'] == 'always') {
    db_update('pgp_contact_pub_keys')
      ->fields(array('policy' => 'always'))
      ->execute();
    drupal_set_message(t('All public keys set to force encryption.'), 'warning');
  }
}

/**
 * Validate that the settings path allows us access to the right libraries
 */
function pgp_contact_settings_validate($form, &$form_state) {
  global $conf;
  $lib = _pgp_contact_lib_info(array(), $form_state['values']['pgp_contact_library_path']);
  if ($form_state['values']['pgp_contact_force_ssl'] == 1 && (!isset($conf['https']) || !$conf['https'])) {
    form_set_error('pgp_contact_force_ssl', t('You  must add !code to your settings.php file in order to force SSL connections.', array('!code' => '<code>$conf[\'https\'] = TRUE;</code>'))); 
  }
  if (!$lib['installed']) { 
    form_set_error('pgp_contact_library_path', t('The OpenPGP.js library was not found.')); 
  }
}
