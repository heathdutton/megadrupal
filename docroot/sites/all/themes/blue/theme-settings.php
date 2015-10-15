<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 */
function blue_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['mtt_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blue Theme Settings'),
    '#collapsible' => FALSE,
	'#collapsed' => FALSE,
  );

  $form['mtt_settings']['tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  
   $form['mtt_settings']['tabs']['blue_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blue Theme width setting'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  );

   $form['mtt_settings']['tabs']['blue_setting']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of theme.'),
  	'#description'   => t('Set width of theme in px for fix width like 800px, 960px, 1200px etc. You can aslo set theme liquid width by seting width into % like 100%, 90% etc.'),
	'#default_value' => theme_get_setting('width', 'blue'),
  );

   $form['mtt_settings']['tabs']['footer_text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer Copyright text'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  );  
   $form['mtt_settings']['tabs']['footer_text']['copyright'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright text'),
  	'#description'   => t('Text appear after &copy; symbol.'),
	'#default_value' => theme_get_setting('copyright', 'black'),
  );  
   
}