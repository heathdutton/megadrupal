<?php

/**
 * @file
 * Admin page.
 */

/**
 * Implements hook_form().
 */
function sharerich_admin_form() {
  $form['social'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social networks'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['social']['sharerich_facebook_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook App ID'),
    '#description' => t('You need to have an App ID, which you can get from Facebook.'),
    '#default_value' => variable_get('sharerich_facebook_app_id'),
  );
  $form['social']['sharerich_facebook_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Site URL'),
    '#description' => t('You need to have an App ID, which you can get from Facebook.'),
    '#default_value' => variable_get('sharerich_facebook_site_url'),
  );
  $form['social']['sharerich_youtube_username'] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube Username'),
    '#description' => t('Enter your YouTube username in order for the social button to link to your YouTube channel.'),
    '#default_value' => variable_get('sharerich_youtube_username'),
  );
  $form['social']['sharerich_github_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Github Username'),
    '#description' => t('Enter your Github username in order for the social button to link to your Github profile.'),
    '#default_value' => variable_get('sharerich_github_username'),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['sharerich_library_variant'] = array(
    '#type' => 'radios',
    '#title' => t('Library variant'),
    '#options' => array(
      'minified' => t('Minified'),
      'source' => t('Source'),
    ),
    '#description' => t('Selecting source may help with JavaScript debugging during development.'),
    '#default_value' => variable_get('sharerich_library_variant', 'minified'),
  );
  $form['advanced']['sharerich_skip_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not load the rrssb library JavaScript'),
    '#description' => t('Enable this if the library is loaded via a non-standard Drupal method and should not be added by the Sharerich module.'),
    '#default_value' => variable_get('sharerich_skip_js', FALSE),
  );

  return system_settings_form($form);
}
