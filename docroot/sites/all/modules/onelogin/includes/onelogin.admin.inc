<?php

/**
 * @file
 * Admin hooks, functions, and handlers for the OneLogin SAML module.
 */


/**
 * Configuration form for the OneLogin SAML module.
 */
function onelogin_settings_form() {
  // Global configurations for OneLogin.
  $form['global'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Global configurations'),
  );
  $form['global']['onelogin_cert'] = array(
    '#type' => 'textarea',
    '#title' => t('x.509 OneLogin certificate'),
    '#default_value' => variable_get('onelogin_cert', ''),
    '#description' => t('Paste the x.509 certificate from OneLogin here.'),
    '#required' => TRUE,
  );
  $form['global']['onelogin_login_url'] = array(
    '#type' => 'textfield',
    '#title' => t('SAML Login URL'),
    '#default_value' => variable_get('onelogin_login_url', ''),
    '#description' => t('You can find this url by editing your Drupal Application in OneLogin.'),
    '#required' => TRUE,
  );
  $form['global']['onelogin_required_globally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require authentication via OneLogin globally'),
    '#default_value' => variable_get('onelogin_required_globally', FALSE),
    '#description' => t('Forces user authentication via OneLogin across every page on the site. Useful for internal sites and intranets.'),
  );

  // Configurations related to user auto-provisioning.
  $form['provisioning'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Automatic user provisioning'),
  );
  $form['provisioning']['onelogin_autoprovision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable automatic Drupal user provisioning'),
    '#default_value' => variable_get('onelogin_autoprovision', FALSE),
    '#description' => t('When a valid OneLogin authentication attempt is made but a Drupal user with the associated e-mail address does not exist, a new user will be automatically provisioned.'),
  );
  $form['provisioning']['onelogin_autoprovision_email_pregmatch'] = array(
    '#type' => 'textfield',
    '#title' => t('User auto-provision regex string'),
    '#default_value' => variable_get('onelogin_autoprovision_email_pregmatch', ''),
    '#description' => t('Only provision users whose e-mails match this regular expression string (for example: %example).', array(
      '%example' => '/.*?@example\.com/is',
    )),
  );

  return system_settings_form($form);
}
