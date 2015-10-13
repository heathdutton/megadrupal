<?php

/**
 * Implementation of themehook_settings().
 */
function ninesixtyrobots_omega_form_system_theme_settings_alter(&$form, $form_state) {  
  $form['ninesixtyrobots'] = array(
    '#type' => 'fieldset',
    '#title' => t('960 Robots Settings'),
  );
  $form['ninesixtyrobots']['breadcrumb_delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('Breadcrumb delimiter'),
    '#size' => 4,
    '#default_value' => theme_get_setting('breadcrumb_delimiter'),
    '#description' => t("Don't forget spaces at either end... if you're into that sort of thing."),
  );
  $form['ninesixtyrobots']['use_twitter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Twitter for site slogan'),
    '#default_value' => theme_get_setting('use_twitter'),
    '#description' => t("Choose a random status update using the search term below."),
  );
  $form['ninesixtyrobots']['twitter_search_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter search term'),
    '#default_value' => theme_get_setting('twitter_search_term'),
    '#description' => t("Pull a random tweet from search.twitter.com using this search query."),
  );
}