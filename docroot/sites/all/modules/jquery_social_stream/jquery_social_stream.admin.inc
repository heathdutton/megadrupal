<?php

/**
 * @file
 * Admin functions for Campaign Social Media module.
 */

/**
 * Admin settings form.
 */
function jquery_social_stream_admin_form($form, &$form_state) {
  $form = array();

  // Twitter.
  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Twitter settings'),
  );
  $form['twitter']['jquery_social_stream_twitter_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter API Key'),
    '#default_value' => variable_get('jquery_social_stream_twitter_api_key', ''),
  );
  $form['twitter']['jquery_social_stream_twitter_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter API Secret'),
    '#default_value' => variable_get('jquery_social_stream_twitter_api_secret', ''),
  );
  $form['twitter']['jquery_social_stream_twitter_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Access Token'),
    '#default_value' => variable_get('jquery_social_stream_twitter_access_token', ''),
  );
  $form['twitter']['jquery_social_stream_twitter_access_token_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Access Token Secret'),
    '#default_value' => variable_get('jquery_social_stream_twitter_access_token_secret', ''),
  );

  // Facebook.
  $form['facebook'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Facebook settings'),
  );
  $form['facebook']['jquery_social_stream_facebook_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook App ID'),
    '#default_value' => variable_get('jquery_social_stream_facebook_app_id', ''),
  );
  $form['facebook']['jquery_social_stream_facebook_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook App Secret'),
    '#default_value' => variable_get('jquery_social_stream_facebook_app_secret', ''),
  );

  // Google +.
  $form['google'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Google +1 settings'),
  );
  $form['google']['jquery_social_stream_google_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google +1 API Key'),
    '#default_value' => variable_get('jquery_social_stream_google_key', ''),
  );

  // YouTube.
  $form['youtube'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('YouTube settings'),
  );
  $form['youtube']['jquery_social_stream_youtube_key'] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube Google API Key'),
    '#default_value' => variable_get('jquery_social_stream_youtube_key', ''),
  );

  // Instagram.
  $form['instagram'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Instagram settings'),
  );

  $form['instagram']['jquery_social_stream_instagram_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Access Token'),
    '#description' => t('Access token created from the authorisation of your OAuth Client'),
    '#default_value' => variable_get('jquery_social_stream_instagram_access_token', ''),
  );
  $form['instagram']['jquery_social_stream_instagram_redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Redirect URL'),
    '#description' => t('The URL entered as the redirect URL when registering your new OAuth Client in the Instagram API setup'),
    '#default_value' => variable_get('jquery_social_stream_instagram_redirect_url', ''),
  );
  $form['instagram']['jquery_social_stream_instagram_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Client ID'),
    '#description' => t('Client ID for API created after registering your new OAuth Client in the instagram API setup'),
    '#default_value' => variable_get('jquery_social_stream_instagram_client_id', ''),
  );

  $section_names = array(
    'google' => 'Creating Your Own Google API Key',
    'instagram' => 'Creating Your Own Instagram API Client ID & Access Token',
    'facebook' => 'Facebook API Credentials',
    'twitter' => 'Twitter API Credentials',
    'youtube' => 'Creating Your Own Youtube API Key',
  );
  $doc_file = libraries_get_path('jquery-social-stream') . '/index.html';
  $doc_file_exists = file_exists(DRUPAL_ROOT . '/' . $doc_file);
  foreach ($section_names as $provider => $section) {
    if ($doc_file_exists) {
      $form[$provider]['#description'] = t('See section %section in <a href="/!path">jQuery Social Stream plugin documentation</a> for details.', array(
        '!path' => $doc_file,
        '%section' => $section
      ));
    }
    else {
      $form[$provider]['#description'] = t('See section %section in jQuery Social Stream plugin documentation for details (file <em>index.html</em> in plugin root directory).', array('%section' => $section));
    }
  }

  return system_settings_form($form);
}
