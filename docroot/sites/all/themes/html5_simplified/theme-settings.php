<?php

/**
 * @file
 * Theme setting callbacks for the html5_simplified theme.   
 * 
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function html5_simplified_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['dem0'] = array(
    '#type' => 'fieldset',
    '#title' => t('Demo content'),
  );
  $form['dem0']['demo_content'] = array(
    '#type' => 'select',
    '#title' => t('Do you wish to enable demo reference content'),
    '#default_value' => theme_get_setting('demo_content', 'html5_simplified'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
  );
  $form['dem0']['top_social_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social links in header'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['dem0']['top_social_link']['social_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show social icons (Facebook, Twitter and RSS) in header'),
    '#default_value' => theme_get_setting('social_links', 'html5_simplified'),
    '#description' => t("Check this option to show twitter, facebook, rss icons in header. Uncheck to hide."),
  );
  $form['dem0']['top_social_link']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username', 'html5_simplified'),
    '#description' => t("Enter your Twitter username."),
  );
  $form['dem0']['top_social_link']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username', 'html5_simplified'),
    '#description' => t("Enter your Facebook username."),
  );
}
