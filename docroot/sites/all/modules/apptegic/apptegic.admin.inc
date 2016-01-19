<?php

/**
 * @file
 * Administration settings for the Apptegic Drupal module.
 */

/**
 * Menu callback; The administration settings for the Apptegic module.
 */
function apptegic_admin_settings() {
  $form['apptegic_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Account'),
    '#description' => t('If you log into Apptegic via http://example.apptegic.com , then provide <em>example</em>.'),
    '#default_value' => variable_get('apptegic_account', ''),
  );
  $form['apptegic_dataset'] = array(
    '#type' => 'textfield',
    '#title' => t('Dataset'),
    '#description' => t('The dataset to use with this site.'),
    '#default_value' => variable_get('apptegic_dataset', ''),
  );
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 50,
  );
  $form['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#description' => t('Choose which fields can be sent off to Apptegic.'),
    '#group' => 'settings',
  );
  $form['fields']['apptegic_parameters'] = array(
    '#type' => 'textarea',
    '#title' => t('Parameters'),
    '#description' => t('Available fields which can be passed to Apptegic that are exposed to !rules. Set them up as Name|Type.', array(
      '!rules' => l(t('Rules'), 'admin/config/workflow/rules/reaction'),
    )),
    '#default_value' => apptegic_parameters(FALSE),
    '#rows' => count(apptegic_parameters()),
  );
  $form['fields']['apptegic_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company field'),
    '#description' => t('The name of the field which will be interpretted as the company for the user.'),
    '#default_value' => variable_get('apptegic_company', ''),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#description' => t('Configure additional settings for Apptegic.'),
    '#group' => 'settings',
  );
  $form['advanced']['apptegic_onmessage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable OnMessage'),
    '#default_value' => variable_get('apptegic_onmessage', FALSE),
  );
  $form['advanced']['apptegic_setusesiteconfig'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Site Config'),
    '#default_value' => variable_get('apptegic_setusesiteconfig', TRUE),
  );
  $form['advanced']['apptegic_apitoken'] = array(
    '#type' => 'textfield',
    '#title' => t('API Token'),
    '#description' => t('Required only if you are using secure OnMessage access.'),
    '#default_value' => variable_get('apptegic_apitoken', ''),
  );
  $form['advanced']['apptegic_cookiedomain'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Domain'),
    '#description' => t('Allow the Apptegic user information to be held across the given cookie domain.'),
    '#default_value' => variable_get('apptegic_cookiedomain', ''),
  );
  return system_settings_form($form);
}
