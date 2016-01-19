<?php

/**
 * @file
 * Airtime Widgets backend/block configuration.
 *
 * This file contains the functions and settings for the
 * parameters of the Airtime Widget plugin.
 */

/**
 * Implements hook_form().
 */
function airtimewidgets_admin_form($form, &$form_state) {

  // Now Playing settings.
  $form['airtimewidgets_fieldset_nowplaying'] = array(
    '#type' => 'fieldset',
    '#title' => t('Airtime Widgets Nowplaying Config'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['airtimewidgets_fieldset_nowplaying']['airtimewidgets_nowplaying_serverurl'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Airtime Server'),
    '#description' => t('Please enter the url to the Airtime server'),
    '#default_value' => variable_get('airtimewidgets_nowplaying_serverurl', AIRTIMEWIDGETS_DEFAULT_NOWPLAYING_SERVER),
  );

  $form['airtimewidgets_fieldset_nowplaying']['airtimewidgets_nowplaying_claim'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Label Nowplaying Claim'),
    '#description' => t('Please enter the text for the label Nowplaying Claim'),
    '#default_value' => variable_get('airtimewidgets_nowplaying_claim', AIRTIMEWIDGETS_DEFAULT_NOWPLAYING_CLAIM),
  );

  $form['airtimewidgets_fieldset_nowplaying']['airtimewidgets_nowplaying_offlinetext'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Nowplaying Offline Text'),
    '#description' => t('Please enter the text for the oflline message.'),
    '#default_value' => variable_get('airtimewidgets_nowplaying_offlinetext', AIRTIMEWIDGETS_DEFAULT_NOWPLAYING_OFFLINETEXT),
  );

  $form['airtimewidgets_fieldset_nowplaying']['airtimewidgets_nowplaying_currentlabel'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Label Nowplaying Current Title'),
    '#description' => t('Please enter the text for the label Nowplaying Current Title.'),
    '#default_value' => variable_get('airtimewidgets_nowplaying_currentlabel', AIRTIMEWIDGETS_DEFAULT_NOWPLAYING_CURRENTLABEL),
  );

  $form['airtimewidgets_fieldset_nowplaying']['airtimewidgets_nowplaying_nextlabel'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Label Nowplaying Next Title'),
    '#description' => t('Please enter the text for the label Nowplaying Next Title.'),
    '#default_value' => variable_get('airtimewidgets_nowplaying_nextlabel', AIRTIMEWIDGETS_DEFAULT_NOWPLAYING_NEXTLABEL),
  );

  $form['airtimewidgets_fieldset_nowplaying']['airtimewidgets_nowplaying_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Nowplaying Interval'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Define Nowplaying Interval. (seconds)'),
    '#default_value' => variable_get('airtimewidgets_nowplaying_interval', AIRTIMEWIDGETS_DEFAULT_NOWPLAYING_INTERVAL),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $form['airtimewidgets_fieldset_nowplaying']['airtimewidgets_nowplaying_consolelog'] = array(
    '#type' => 'select',
    '#title' => t('console.log() ON/OFF'),
    '#options' => array(
      'TRUE' => 'ON',
      'FALSE' => 'OFF'),
    '#default_value' => variable_get('airtimewidgets_nowplaying_consolelog', AIRTIMEWIDGETS_DEFAULT_NOWPLAYING_CONSOLELOG),
    '#description' => t('Enables/disables console.log() to debug javascript variables.'),
  );

  // Show And Track settings.
  $form['airtimewidgets_fieldset_showandtrack'] = array(
    '#type' => 'fieldset',
    '#title' => t('Airtime Widgets Show And Track Config'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['airtimewidgets_fieldset_showandtrack']['airtimewidgets_showandtrack_serverurl'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Airtime Server'),
    '#description' => t('Please enter the url to the Airtime server'),
    '#default_value' => variable_get('airtimewidgets_showandtrack_serverurl', AIRTIMEWIDGETS_DEFAULT_SHOWANDTRACK_SERVER),
  );

  $form['airtimewidgets_fieldset_showandtrack']['airtimewidgets_showandtrack_claim'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Claim Text'),
    '#description' => t('Please enter the text for the label claim'),
    '#default_value' => variable_get('airtimewidgets_showandtrack_claim', AIRTIMEWIDGETS_DEFAULT_SHOWANDTRACK_CLAIM),
  );

  $form['airtimewidgets_fieldset_showandtrack']['airtimewidgets_showandtrack_offlinetext'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Offline Text'),
    '#description' => t('Please enter the text for the oflline message.'),
    '#default_value' => variable_get('airtimewidgets_showandtrack_offlinetext', AIRTIMEWIDGETS_DEFAULT_SHOWANDTRACK_OFFLINETEXT),
  );

  $form['airtimewidgets_fieldset_showandtrack']['airtimewidgets_showandtrack_currentlabel'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Label Current Title'),
    '#description' => t('Please enter the text for the label current title.'),
    '#default_value' => variable_get('airtimewidgets_showandtrack_currentlabel', AIRTIMEWIDGETS_DEFAULT_SHOWANDTRACK_CURRENTLABEL),
  );

  $form['airtimewidgets_fieldset_showandtrack']['airtimewidgets_showandtrack_nextlabel'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Label Next Title'),
    '#description' => t('Please enter the text for the label next title.'),
    '#default_value' => variable_get('airtimewidgets_showandtrack_nextlabel', AIRTIMEWIDGETS_DEFAULT_SHOWANDTRACK_NEXTLABEL),
  );

  $form['airtimewidgets_fieldset_showandtrack']['airtimewidgets_showandtrack_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Interval'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Define Nowplaying Interval. (seconds)'),
    '#default_value' => variable_get('airtimewidgets_showandtrack_interval', AIRTIMEWIDGETS_DEFAULT_SHOWANDTRACK_INTERVAL),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $form['airtimewidgets_fieldset_showandtrack']['airtimewidgets_showandtrack_consolelog'] = array(
    '#type' => 'select',
    '#title' => t('console.log() ON/OFF'),
    '#options' => array(
      'TRUE' => 'ON',
      'FALSE' => 'OFF'),
    '#default_value' => variable_get('airtimewidgets_showandtrack_consolelog', AIRTIMEWIDGETS_DEFAULT_SHOWANDTRACK_CONSOLELOG),
    '#description' => t('Enables/disables console.log() to debug javascript variables.'),
  );

  // Todays Program settings.
  $form['airtimewidgets_fieldset_todaysprogram'] = array(
    '#type' => 'fieldset',
    '#title' => t('Airtime Widgets Todays Program Config'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['airtimewidgets_fieldset_todaysprogram']['airtimewidgets_todaysprogram_serverurl'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Airtime Server'),
    '#description' => t('Please enter the url to the Airtime server'),
    '#default_value' => variable_get('airtimewidgets_todaysprogram_serverurl', AIRTIMEWIDGETS_DEFAULT_TODAYSPROGRAM_SERVER),
  );

  $form['airtimewidgets_fieldset_todaysprogram']['airtimewidgets_todaysprogram_claim'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Claim Text'),
    '#description' => t('Please enter the text for the label claim'),
    '#default_value' => variable_get('airtimewidgets_todaysprogram_claim', AIRTIMEWIDGETS_DEFAULT_TODAYSPROGRAM_CLAIM),
  );

  $form['airtimewidgets_fieldset_todaysprogram']['airtimewidgets_todaysprogram_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Interval'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Define Todays Program Interval. (seconds)'),
    '#default_value' => variable_get('airtimewidgets_todaysprogram_interval', AIRTIMEWIDGETS_DEFAULT_TODAYSPROGRAM_INTERVAL),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $form['airtimewidgets_fieldset_todaysprogram']['airtimewidgets_todaysprogram_showlimit'] = array(
    '#type' => 'textfield',
    '#title' => t('Show Limit'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Define Show Limit. (Items)'),
    '#default_value' => variable_get('airtimewidgets_todaysprogram_showlimit', AIRTIMEWIDGETS_DEFAULT_TODAYSPROGRAM_SHOWLIMIT),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $form['airtimewidgets_fieldset_todaysprogram']['airtimewidgets_todaysprogram_consolelog'] = array(
    '#type' => 'select',
    '#title' => t('console.log() ON/OFF'),
    '#options' => array(
      'TRUE' => 'ON',
      'FALSE' => 'OFF'),
    '#default_value' => variable_get('airtimewidgets_todaysprogram_consolelog', AIRTIMEWIDGETS_DEFAULT_TODAYSPROGRAM_CONSOLELOG),
    '#description' => t('Enables/disables console.log() to debug javascript variables.'),
  );

  // Weekly Program settings.
  $form['airtimewidgets_fieldset_weeklyprogram'] = array(
    '#type' => 'fieldset',
    '#title' => t('Airtime Widgets Weekly Program Config'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['airtimewidgets_fieldset_weeklyprogram']['airtimewidgets_weeklyprogram_serverurl'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 60,
    '#title' => t('Airtime Server'),
    '#description' => t('Please enter the url to the Airtime server'),
    '#default_value' => variable_get('airtimewidgets_weeklyprogram_serverurl', AIRTIMEWIDGETS_DEFAULT_WEEKLYPROGRAM_SERVER),
  );

  $form['airtimewidgets_fieldset_weeklyprogram']['airtimewidgets_weeklyprogram_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Interval'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Define Weekly Program Interval. (seconds)'),
    '#default_value' => variable_get('airtimewidgets_weeklyprogram_interval', AIRTIMEWIDGETS_DEFAULT_WEEKLYPROGRAM_INTERVAL),
    '#size' => 60,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $form['airtimewidgets_fieldset_weeklyprogram']['airtimewidgets_weeklyprogram_consolelog'] = array(
    '#type' => 'select',
    '#title' => t('console.log() ON/OFF'),
    '#options' => array(
      'TRUE' => 'ON',
      'FALSE' => 'OFF'),
    '#default_value' => variable_get('airtimewidgets_weeklyprogram_consolelog', AIRTIMEWIDGETS_DEFAULT_WEEKLYPROGRAM_CONSOLELOG),
    '#description' => t('Enables/disables console.log() to debug javascript variables.'),
  );

  return system_settings_form($form);

}
