<?php

/**
 * @file
 * Admin page callback file for the xing connect module.
 */

/**
 * Admin setting form.
 */
function xing_connect_admin_api_key_setting($form, &$form_state) {
  $form['xing_connect_ckey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Xing Consumer key'),
    '#default_value' => variable_get('xing_connect_ckey', NULL),
    '#description' => t('Also called the <em>OAuth Consumer key</em> value on Xing App settings pages. <a href="https://dev.xing.com/applications/dashboard">Xing Apps must first be created</a> before they can be added here.'),
  );

  $form['xing_connect_skey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Xing Consumer secret'),
    '#default_value' => variable_get('xing_connect_skey', NULL),
    '#description' => t('Also called the <em>OAuth Consumer secret</em> value on Xing App settings pages.'),
  );

  $form['xing_connect_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Login using xing'),
    '#description' => t('Allow user to register and login with xing.'),
    '#default_value' => variable_get('xing_connect_login', 0),
  );

  $form['xing_connect_post_login_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Post Login url'),
    '#description' => t('Drupal URL to which the user should be redirected to after successful login.'),
    '#default_value' => variable_get('xing_connect_post_login_url', ''),
  );
  return system_settings_form($form);
}
