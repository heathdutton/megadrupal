<?php

/**
 * @file
 * Administration pages for OpenID URL.
 */ 

/**
 * Menu callback; Display the global OpenID URL settings.
 */
function openidurl_settings() {
  $form['openidurl_version'] = array(
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#title' => t('Version Compatibility'),
    '#default_value' => variable_get('openidurl_version', array(11, 20)),
    '#description' => t("Which versions of OpenID compatibility you would like to support."),
    '#options' => array(
      11 => t('1.1'),
      20 => t('2.0'),
    ),
  );
  $form['openidurl_server'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenID Server'),
    '#default_value' => variable_get('openidurl_server'),
    '#description' => t("This is the OpenID server (e.g. http://www.myopenid.com/server)."),
  );
  $form['openidurl_delegate'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenID Delegate'),
    '#default_value' => variable_get('openidurl_delegate'),
    '#description' => t("This is the OpenID delegate (e.g. http://yourname.myopenid.com/)."),
  );
  $form['openidurl_xrds'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenID XRDS Location'),
    '#default_value' => variable_get('openidurl_xrds'),
    '#description' => t("This is the OpenID XRDS location (e.g. http://yourname.myopenid.com/xrds)."),
  );
  return system_settings_form($form);
}
