<?php

/**
 * @file
 * Theme setting callbacks for the Prefessional Pro theme.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */

function professional_pro_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['professional_pro_settings']['top_social_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social links in header'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['professional_pro_settings']['top_social_link']['social_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show social icons (Facebook, Twitter and RSS) in header'),
    '#default_value' => theme_get_setting('social_links', 'professional_pro'),
    '#description'   => t("Check this option to show twitter, facebook, rss icons in header. Uncheck to hide."),
  );
  $form['professional_pro_settings']['top_social_link']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username', 'professional_pro'),
    '#description' => t("Enter your Twitter username."),
  );
  $form['professional_pro_settings']['top_social_link']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username', 'professional_pro'),
    '#description' => t("Enter your Facebook username."),
  );

}