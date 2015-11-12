<?php

/**
 * @file
 * Provides setting for Mundus theme.
 *
 * @todo
 *
 *    -- Integration with color module.
 */


/**
 * Admin settings for theme using themename_form_system_theme_settings_alter().
 */
function mundus_form_system_theme_settings_alter(&$form, $form_state) {
  $form['google_font_choice'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google fonts'),
  );
  $form['google_font_choice']['google_font_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Google font headings (h1,h2,h3,h4,h5,h6)'),
    '#default_value' => theme_get_setting('google_font_name'),
    '#description' => t("Enter Google font name, default used is Merriweather Sans. Put in this field name of font without + sign, e.g Merriweather Sans, not Merriweather+Sans"),
  );
  $form['google_font_choice']['google_font_body'] = array(
    '#type' => 'textfield',
    '#title' => t('Google font body'),
    '#default_value' => theme_get_setting('google_font_body'),
    '#description' => t("Enter Google font name, default used is Ubuntu. Put in this field name of font without + sign, e.g Merriweather Sans, not Merriweather+Sans"),
  );
  $form['mundus_color'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mundus color style'),
  );
  $form['mundus_color']['mundus_style'] = array(
    '#type' => 'radios',
    '#title' => t('Select color style'),
    '#default_value' => theme_get_setting('mundus_style'),
    '#options' => array(
      'red' => t('Theme default (red)'),
      'aero' => t('Aero'),
      'blue' => t('Blue'),
      'green' => t('Green'),
      'dark_spring_green' => t('Dark spring green'),
      'fandango' => t('Fandango'),
      'rust' => t('Rust'),
      'amaranth' => t('Amaranth'),
    ),
  );
  $form['mundus_login'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mundus login'),
  );
  $form['mundus_login']['mundus_login_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Mundus login'),
    '#default_value' => theme_get_setting('mundus_login_top'),
    '#description' => t("Check this option to show Mundus login in topbar position"),
  );
  $form['mundus_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mundus search'),
  );
  $form['mundus_search']['mundus_search_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Mundus search'),
    '#default_value' => theme_get_setting('mundus_search_top'),
    '#description' => t("Check this option to show Mundus search in topbar position"),
  );
  $form['social_profiles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mundus social'),
  );
  $form['social_profiles']['social_profiles_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Mundus social'),
    '#default_value' => theme_get_setting('social_profiles_top'),
    '#description' => t("Check this option to show Mundus social in topbar position"),
  );
  $form['social_profiles']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username'),
    '#description' => t("Enter your Twitter username or leave empty to hide."),
  );
  $form['social_profiles']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username'),
    '#description' => t("Enter your Facebook username or leave empty to hide."),
  );
  $form['social_profiles']['linkedin_username'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn Username'),
    '#default_value' => theme_get_setting('linkedin_username'),
    '#description' => t("Enter your LinkedIn username or leave empty to hide"),
  );
  $form['social_profiles']['google_plus_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Plus Username'),
    '#default_value' => theme_get_setting('google_plus_username'),
    '#description' => t("Enter your Google Plus username or leave empty to hide."),
  );
  $form['social_profiles']['rss_url'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS'),
    '#default_value' => theme_get_setting('rss_url'),
    '#description' => t("Enter a custom RSS URL or leave empty to hide, default is rss.xml"),
  );
}
