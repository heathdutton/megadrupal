<?php

/**
 * @file
 * Theme setting callbacks for the Modern Theme.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function corporate_corp_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['corporate_corp_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('corporate_corp Theme Settings'),
    '#weight' => -1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['corporate_corp_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs', 'corporate_corp'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
  $form['corporate_corp_settings']['footer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['corporate_corp_settings']['footer']['footer_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show copyright text in footer'),
    '#default_value' => theme_get_setting('footer_copyright', 'corporate_corp'),
    '#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );
  $form['corporate_corp_settings']['footer']['footer_credits'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show theme credits in footer'),
    '#default_value' => theme_get_setting('footer_credits', 'corporate_corp'),
    '#description'   => t("Check this option to show site credits in footer. Uncheck to hide."),
  );
  $form['corporate_corp_settings']['top_social_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social links in header'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['corporate_corp_settings']['top_social_link']['social_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show social icons (Facebook, Twitter and RSS) in header'),
    '#default_value' => theme_get_setting('social_links', 'corporate_corp'),
    '#description'   => t("Check this option to show twitter, facebook, rss icons in header. Uncheck to hide."),
  );
  $form['corporate_corp_settings']['top_social_link']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username', 'corporate_corp'),
    '#description' => t("Enter your Twitter username."),
  );
  $form['corporate_corp_settings']['top_social_link']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username', 'corporate_corp'),
    '#description' => t("Enter your Facebook username."),
  );

}