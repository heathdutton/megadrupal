<?php

/**
 * @file
 * Administration form for this module.
 */

/**
 * Menu callback: AI Mobile module configuration.
 */
function ai_mobile_settings_form($form, &$form_state) {

  $form['ai_mobile_domain_desktop'] = array(
    '#type' => 'textfield',
    '#title' => t('Regular domain'),
    '#default_value' => variable_get('ai_mobile_domain_desktop', AI_MOBILE_DOMAIN_DESKTOP),
    '#description' => ai_mobile_settings_txts('domain_desktop'),
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['ai_mobile_domain_mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile domain'),
    '#default_value' => variable_get('ai_mobile_domain_mobile', AI_MOBILE_DOMAIN_MOBILE),
    '#description' => ai_mobile_settings_txts('domain_mobile'),
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['ai_mobile_detect'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Detect php library path'),
    '#default_value' => variable_get('ai_mobile_detect', AI_MOBILE_DETECT),
    '#description' => ai_mobile_settings_txts('mobile_detect'),
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  global $theme_key;
  $form['ai_mobile_theme'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name of mobile theme'),
    '#default_value' => variable_get('ai_mobile_theme', $theme_key),
    '#description' => ai_mobile_settings_txts('mobile_theme'),
    '#required' => TRUE,
    '#maxlength' => 255,
  );
  $form['ai_mobile_phone_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable phones support'),
    '#default_value' => variable_get('ai_mobile_phone_enabled', AI_MOBILE_PHONE_ENABLED),
    '#description' => ai_mobile_settings_txts('phone_enabled'),
  );
  $form['ai_mobile_tablet_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable tablets support'),
    '#default_value' => variable_get('ai_mobile_tablet_enabled', AI_MOBILE_TABLET_ENABLED),
    '#description' => ai_mobile_settings_txts('tablet_enabled'),
  );
  $form['ai_mobile_support_auth'] = array(
    '#type' => 'checkbox',
    '#title' => t('Support authenticated users'),
    '#default_value' => variable_get('ai_mobile_support_auth', AI_MOBILE_SUPPORT_AUTH),
    '#description' => ai_mobile_settings_txts('support_auth'),
  );
  $form['ai_mobile_no_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable mobile domain redirect'),
    '#default_value' => variable_get('ai_mobile_no_redirect', AI_MOBILE_NO_REDIRECT),
    '#description' => ai_mobile_settings_txts('no_redirect'),
  );

  return system_settings_form($form);
}


/**
 * Form items descriptions.
 */
function ai_mobile_settings_txts($key) {
  $descriptions = array(
    'domain_desktop' => 'This is the domain that this module will treat as desktop/tablet domain.<br />Usually it\'s \'www.example.com\'.',
    'domain_mobile' => 'This is the domain that this module will treat as mobile domain.<br />Usually it\'s \'m.example.com\'.',
    'mobile_detect' => 'Path to the Mobile_Detect.php library, relative to Drupal root directory (no trailing slash).',
    'mobile_theme' => 'Theme specified here must be enabled on the site for this to work',
    'phone_enabled' => 'Defines if mobile site is enabled. If no, mobile theme will never show and mobile subdomain won\'t be used.',
    'tablet_enabled' => 'Defines if tablet CSS styles are enabled. If enabled, &lt;body&gt; tag will have \'tablet\' class applied when user agent is tablet device.',
    'support_auth' => 'Allow logged in users to access mobile site?<br />If no, logged in users will always see desktop version. This is useful if you have only<br />site managers and you want them to manage site through regular theme.',
    'no_redirect' => 'Development option: switch theme without domain redirect.<br />If enabled will cause the theme to change based on device type without redirecting to \'m\' subdomain.<br />This is useful while in development environment because you can use same url<br />to test both mobile and desktop versions but should be turned off on production<br />sites because switching theme without changing subdomain will not work correctly when<br />Drupal page cache is enabled.',
  );

  if (isset($descriptions[$key])) {
    return t($descriptions[$key]);
  }
  else {
    return '';
  }
}
