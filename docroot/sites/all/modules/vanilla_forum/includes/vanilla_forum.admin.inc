<?php

/**
 * @file
 * Administrative page callbacks for the vanilla_forum module.
 */

/**
 * Form builder for the vanilla_forum admin config form.
 */
function vanilla_forum_configuration_form($form, &$form_state) {
  // Vanilla Cookie ID fieldset description text.
  $cookie_id_description = '';
  $cookie_id_description .= t('<p>Please enter the unique Vanilla Forum cookie IDs for your site\'s Forum implementation.&nbsp;');
  $cookie_id_description .= t('The cookies are set on the Vanilla side<br/>and their IDs can be located by logging into Vanilla forum and inspecting the cookie information in your browser.</p>');

  // Vanilla Forum URL textfield description text.
  $vanilla_url_description = '';
  $vanilla_url_description .= t('Please enter your forum URL.&nbsp;');
  $vanilla_url_description .= t('Your forum and site must share the same domain in order for simultaneous login/logout&nbsp;');
  $vanilla_url_description .= t('between Drupal and Vanilla to work.');

  // Forum Settings fieldset.
  $form['vanilla_forum_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vanilla Forum Settings'),
    '#description' => t('Settings specific to your Vanilla Forum implementation.'),
  );
  // Vanilla Forum Client ID textfield.
  $form['vanilla_forum_settings']['vanilla_forum_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client id'),
    '#default_value' =>  variable_get('vanilla_forum_client_id', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#description' => t('The JSConnect client ID. This is generated on the Vanilla side and can be found under the jsConnect section of your Vanilla Forum Dashboard.'),
    '#required' => TRUE,
  );
  // Vanilla Forum Shared Secret textfield.
  $form['vanilla_forum_settings']['vanilla_forum_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shared secret'),
    '#default_value' =>  variable_get('vanilla_forum_secret', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#description' => t('The JSConnect shared secret between your Drupal Installation and Vanilla forum.'),
    '#required' => TRUE,
  );
  // Vanilla Forum URL textfield.
  $form['vanilla_forum_settings']['vanilla_forum_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Vanilla Forum URL'),
    '#description' => filter_xss($vanilla_url_description, array('br', 'b')),
    '#maxlength' => 250,
    '#size' => 50,
    '#default_value' => variable_get('vanilla_forum_url', ''),
    '#required' => TRUE,
  );
  // Forum Privacy Settings checkbox.
  $form['vanilla_forum_settings']['vanilla_forum_privacy_settings'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Private forum</strong>'),
    '#description' => t('Check this box if your forum is set to private. Private forums are only accessible by registered logged in users.'),
    '#default_value' => variable_get('vanilla_forum_privacy_settings', 0),
  );

  // Vanilla Cookie ID fieldset.
  $form['vanilla_forum_cookie_ids'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vanilla Forum Cookie IDs'),
    '#description' => filter_xss($cookie_id_description, array('p', 'br')),
  );
  // Main Cookie ID textfield.
  $form['vanilla_forum_cookie_ids']['vanilla_forum_main_cookie_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Main Vanilla cookie ID'),
    '#description' => t('Format: <b>vf_forumname_XXXXX</b> (<i>Used to simultaneously log users out of Vanilla/Drupal</i>).'),
    '#maxlength' => 100,
    '#size' => 30,
    '#default_value' => variable_get('vanilla_forum_main_cookie_id', ''),
    '#required' => TRUE,
  );
  // Volatile Cookie ID textfield.
  $form['vanilla_forum_cookie_ids']['vanilla_forum_volatile_cookie_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Vanilla Volatile cookie ID'),
    '#description' => t('Format: <b>vf_forumname_XXXXX-Volatile</b> (<i>Used by Vanilla to record preferences and session login</i>).'),
    '#maxlength' => 100,
    '#size' => 30,
    '#default_value' => variable_get('vanilla_forum_volatile_cookie_id', ''),
    '#required' => TRUE,
  );

  // User Profile image settings. Only display if user profile pictures are
  // enabled on the site.
  if (variable_get('user_pictures', 0)) {
    $form['vanilla_forum_profile_image_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('User profile image synchronization'),
      '#collapsible' => TRUE,
      '#collapsed' => variable_get('vanilla_forum_profile_image_sync', '') ? FALSE : TRUE,
    );
    $form['vanilla_forum_profile_image_settings']['vanilla_forum_profile_image_sync'] = array(
      '#type' => 'checkbox',
      '#title' => t('Synchronize user profile images'),
      '#default_value' =>  variable_get('vanilla_forum_profile_image_sync', ''),
      '#description' => t('Sync Drupal user profile images over to Vanilla.'),
    );
    $form['vanilla_forum_profile_image_settings']['vanilla_forum_profile_image_style'] = array(
      '#type' => 'select',
      '#title' => t('Profile image size'),
      '#options' => image_style_options(),
      '#default_value' =>  variable_get('vanilla_forum_profile_image_style', ''),
      '#description' => t("Choose an image style for the profile images. 180x180 is optimal."),
    );
  }
  return system_settings_form($form);
}
