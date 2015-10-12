<?php
/**
 * @file loginza.admin.inc
 *    Administration routines.
 */

function loginza_config_settings() {
  $providers_def = loginza_providers_list(TRUE);
  $providers = loginza_providers_list();
  $images = loginza_widgets_images();

  $form = array();

  $form['loginza_providers_set'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Providers Set'),
    '#options' => $providers_def,
    '#default_value' => variable_get('loginza_providers_set', array()),
    '#description' => t('If none selected all will be used.'),
  );
  $form['loginza_default_provider'] = array(
    '#type' => 'select',
    '#title' => t('Default Provider'),
    '#options' => array('' => '') + $providers_def,
    '#default_value' => variable_get('loginza_default_provider', ''),
    '#required' => FALSE,
  );
  $form['loginza_caption_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption text'),
    '#required' => TRUE,
    '#default_value' => variable_get('loginza_caption_text', t('Log in with Loginza')),
    '#description' => t('Used for Button and Icons widgets.'),
  );
  $form['loginza_default_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default user name'),
    '#required' => TRUE,
    '#default_value' => variable_get('loginza_default_name', 'user@loginza'),
  );
  $form['loginza_allow_email_bypass'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow email bypass'),
    '#default_value' => variable_get('loginza_allow_email_bypass', FALSE),
    '#description' => t('Allow silent user registration with fake email if not provided by Loginza.'),
  );

  $form['advanced_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced_set']['loginza_amnesia_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Don\'t remember return paths'),
    '#description' => t('Paths that user should not be terurned to after Loginza authentication.'),
    '#default_value' => variable_get('loginza_amnesia_paths', "user/register\r\nuser"),
  );
  $form['advanced_set']['loginza_providers_icons'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Providers icons'),
    '#options' => $providers_def,
    '#default_value' => variable_get('loginza_providers_icons', array()),
    '#description' => t('Providers icons to show for Loginza Icons widget. Used only when Providers Set is empty.'),
  );

  $form['loginza_providers_titles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Providers titles'),
    '#description' => t('Override default provider titles.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  foreach ($providers as $provider => $title) {
    $form['loginza_providers_titles'][$provider] = array(
      '#type' => 'textfield',
      '#title' => $providers_def[$provider],
      '#default_value' => $title,
      '#required' => TRUE,
    );
  }

  $form['loginza_widgets_images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Widgets images'),
    '#description' => t('Override default widget images.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['loginza_widgets_images']['button_widget'] = array(
    '#type' => 'textfield',
    '#title' => t('Button Widget'),
    '#default_value' => $images['button_widget'],
    '#required' => TRUE,
  );
  foreach ($providers as $provider => $title) {
    $form['loginza_widgets_images'][$provider] = array(
      '#type' => 'textfield',
      '#title' => $providers_def[$provider],
      '#default_value' => $images[$provider],
      '#required' => TRUE,
    );
  }

  $form['integration_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Integration'),
    '#description' => t('The only widget can be integrated into login block. If both selected - String Widget will be ignored.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['integration_set']['loginza_integrate_login_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Loginza Button Widget to login block'),
    '#default_value' => variable_get('loginza_integrate_login_button', TRUE),
  );
  $form['integration_set']['loginza_integrate_login_string'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Loginza String Widget to login block'),
    '#default_value' => variable_get('loginza_integrate_login_string', FALSE),
  );
  $form['integration_set']['loginza_integrate_login_iframe'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Loginza Iframe Widget to login page'),
    '#default_value' => variable_get('loginza_integrate_login_iframe', TRUE),
  );
  $form['integration_set']['loginza_integrate_register_iframe'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Loginza Iframe Widget to register page'),
    '#default_value' => variable_get('loginza_integrate_register_iframe', TRUE),
  );
  $form['integration_set']['loginza_integrate_comment_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Loginza Icons Widget to comment form'),
    '#default_value' => variable_get('loginza_integrate_comment_icons', TRUE),
  );

  return system_settings_form($form);
}
