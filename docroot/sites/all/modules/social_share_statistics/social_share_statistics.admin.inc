<?php

/**
 * @file
 * Admin settings handling file for social share statistics.
 */

/**
 * Configuration form for the general settings for Social Share Statistics.
 */
function social_share_statistics_engine_settings_form() {
  $form['settings'] = array();

  $form['settings']['social_share_statistics_fbapi'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook API'),
    '#default_value' => variable_get('social_share_statistics_fbapi'),
    '#description' => t("The Facebook AppID."),
    '#required' => TRUE,
  );

  $form['settings']['social_share_statistics_twtname'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => variable_get('social_share_statistics_twtname'),
    '#description' => t("The Twitter Username to add to the tweet."),
    '#required' => FALSE,
  );

  $show_shares = array('No', 'Yes');
  drupal_map_assoc($show_shares);

  $form['settings']['social_share_statistics_shares'] = array(
    '#type' => 'radios',
    '#title' => t('Show Total Shares'),
    '#default_value' => variable_get('social_share_statistics_shares'),
    '#options' => $show_shares,
    '#description' => t("Do you want to show total shares?"),

  );

  $form['settings']['social_share_statistics_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Total Share Count Text'),
    '#default_value' => variable_get('social_share_statistics_text'),
    '#description' => t("The Total Shares Count Text you want to display before the count"),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
