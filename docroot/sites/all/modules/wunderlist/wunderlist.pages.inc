<?php
/**
 * @file
 * Page callbacks for Wunderlist module.
 */

/**
 * Wunderlist settings form.
 */
function wunderlist_admin_form() {

  if (!variable_get('wunderlist_token', FALSE)) {
    $state = mt_rand();
    $redirect_url = url('wunderlist/oauth2', array('absolute' => TRUE));
    $client_id = variable_get('wunderlist_client_id', '');
    if (!empty($client_id)) {
      $link = "https://www.wunderlist.com/oauth/authorize?client_id=$client_id&redirect_uri=$redirect_url&state=$state";
      drupal_set_message(t('You need to allow your application by following !link_auth', array('!link_auth' => l(t('this link'), $link))), 'warning', FALSE);
    }
  }
  else {
    drupal_set_message(t('Token acquired from Wunderlist'));
    drupal_set_message(t("Don't forget to run !cron to import your tasks!",
      array('!cron' => l(t('cron'), 'admin/config/system/cron'))), 'warning');
  }

  $form['wunderlist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Wunderlist settings'),
    '#description' => t('Wunderlist urls'),
  );

  $form['wunderlist']['wunderlist_url'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('wunderlist_url', 'https://www.wunderlist.com'),
    '#description' => t('The wunderlist server url.'),
  );

  $form['wunderlist']['wunderlist_api_url'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('wunderlist_api_url', 'https://a.wunderlist.com/api/v1'),
    '#description' => t('API url to fetch from'),
  );

  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('OAuth Settings'),
    '#access' => module_exists('oauth2_client'),
    '#description' => t('To enable OAuth based access for wunderlist, you must <a href="@url">register your application</a> with Wunderlist and add the provided keys here.', array('@url' => 'https://developer.wunderlist.com/apps/new')),
  );
  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#markup' => url('wunderlist/oauth2', array('absolute' => TRUE)),
  );
  $form['oauth']['wunderlist_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Client id'),
    '#default_value' => variable_get('wunderlist_client_id', NULL),
  );
  $form['oauth']['wunderlist_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Client secret'),
    '#default_value' => variable_get('wunderlist_client_secret', NULL),
  );

  return system_settings_form($form);
}
