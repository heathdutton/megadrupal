<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 */
function white_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['mtt_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('white Theme Settings'),
    '#collapsible' => FALSE,
	'#collapsed' => FALSE,
  );

  $form['mtt_settings']['tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  
   $form['mtt_settings']['tabs']['white_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('white setting'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  );

   $form['mtt_settings']['tabs']['white_setting']['photo_1'] = array(
    '#type' => 'textfield',
    '#title' => t('First photo.'),
  	'#description'   => t('First white of slide-show.'),
	'#default_value' => theme_get_setting('photo_1', 'white'),
  );  
  $form['mtt_settings']['tabs']['white_setting']['photo_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Second photo.'),
  	'#description'   => t('Second photo of slide-show.'),
	'#default_value' => theme_get_setting('photo', 'white'),
  ); 
  $form['mtt_settings']['tabs']['white_setting']['photo_3'] = array(
    '#type' => 'textfield',
    '#title' => t('Third photo.'),
  	'#description'   => t('Third photo of slide-show.'),
	'#default_value' => theme_get_setting('photo_3', 'white'),
  ); 
  $form['mtt_settings']['tabs']['white_setting']['photo_4'] = array(
    '#type' => 'textfield',
    '#title' => t('Fourth photo.'),
  	'#description'   => t('Fourth photo of slide-show.'),
	'#default_value' => theme_get_setting('photo_4', 'white'),
  ); 
  $form['mtt_settings']['tabs']['white_setting']['photo_5'] = array(
    '#type' => 'textfield',
    '#title' => t('Fifth photo.'),
  	'#description'   => t('Fifth photo of slide-show.'),
	'#default_value' => theme_get_setting('photo_5', 'white'),
  ); 
   
}