<?php
/**
 * @file
 * Theme settings for Breadcrump and Social Block. 
 */

/**
 * Implements of hook_form_system_theme_settings_alter().
 */
function azhagu_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['azhagu_settings']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['azhagu_settings']['breadcrumb']['breadcrumb_delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('Breadcrumb delimiter'),
    '#size' => 4,
    '#default_value' => theme_get_setting('breadcrumb_delimiter'),
    '#description' => t("Don't forget spaces at either end... if you're into that sort of thing."),
  );
  $form['azhagu_settings']['socialblock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['azhagu_settings']['socialblock']['social_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icons'),
    '#default_value' => theme_get_setting('social_block', 'azhagu'),
    '#description'   => t("Check this option to show Social Icons."),
  );
  $form['azhagu_settings']['socialblock']['social_block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Social Block Title'),
    '#default_value' => theme_get_setting('social_block_title', 'azhagu'),
    '#description'   => t("Enter your Social Block title here. Leave blank to hide."),
  );
  $form['azhagu_settings']['socialblock']['twitter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Profile URL'),
    '#default_value' => theme_get_setting('twitter_url', 'azhagu'),
    '#description'   => t("Enter your Twitter Profile URL. Leave blank to hide."),
  );
  $form['azhagu_settings']['socialblock']['facebook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Profile URL'),
    '#default_value' => theme_get_setting('facebook_url', 'azhagu'),
    '#description'   => t("Enter your Facebook Profile URL. Leave blank to hide."),
  );
  $form['azhagu_settings']['socialblock']['gplus_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Plus Address'),
    '#default_value' => theme_get_setting('gplus_url', 'azhagu'),
    '#description'   => t("Enter your Google Plus URL. Leave blank to hide."),
  );
  $form['azhagu_settings']['socialblock']['pinterest_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Pinterest Address'),
    '#default_value' => theme_get_setting('pinterest_url', 'azhagu'),
    '#description'   => t("Enter your Pinterest URL. Leave blank to hide."),
  );
}
