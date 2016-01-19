<?php
/**
 * @file
 * SlideRoom module admin settings.
 */

/**
 * Return the SlideRoom global settings form.
 */
function slideroom_admin_settings() {
  $form['slideroom_oauth_token'] = array(
    '#type' => 'textfield',
    '#title' => t('SlideRoom API OAuth token'),
    '#required' => FALSE,
    '#default_value' => variable_get('slideroom_oauth_token', ''),
    '#description' => t('An OAuth token is required to access the API. To generate a token, visit the !apilink page, which can be found under Settings in the Review app.', array('!apilink' => l(t('Developer API'), 'https://review.slideroom.com/#/account/index/api'))),
  );

  $form['slideroom_request_throttle'] = array(
    '#type' => 'textfield',
    '#title' => t('SlideRoom API Request Throttle (hours)'),
    '#required' => FALSE,
    '#default_value' => variable_get('slideroom_request_throttle', ''),
    '#description' => t('Minimum number of hours between new export requests to the SlideRoom API (e.g. 24 = one export per day). By default, new export requests are triggered every time cron runs. Set this to throttle API requests, or leave blank for no limit.'),
  );

  return system_settings_form($form);
}