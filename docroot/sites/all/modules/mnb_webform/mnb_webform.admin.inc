<?php
/**
 * @file
 * Provides admin settings page to adjust API key and client ID.
 */

function mnb_webform_admin_settings() {
  $form = array();

  $form['mnbapi'] = array(
    '#title' => t('MyNewsletterBuilder API Settings'),
    '#type' => 'fieldset',
  );

  $form['mnbapi']['mnb_webform_apikey'] = array(
    '#title' => t('MyNewsletterBuilder API Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('mnb_webform_apikey', ''),
    '#description' => t('This can be found in MyNewsletterBuilder under "Account Settings".'),
  );

  $form['mnbui'] = array(
    '#title' => t('MyNewsletterBuilder User Interface Settings'),
    '#type' => 'fieldset',
  );

  $form['mnbui']['mnb_webform_showmsg'] = array(
    '#title' => t('Show successful subscription message'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mnb_webform_showmsg', 0),
    '#description' => t('Show users a "You have been successfully subscribed" message when they subscribe. Disable if you have a custom landing page. Unsubscriptions and subscription edits will still result in a message.')
  );

  return system_settings_form($form);
}
