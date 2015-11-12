<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function biography_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['mtt_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Biography Theme Settings'),
    '#collapsible' => FALSE,
	'#collapsed' => FALSE,
  );

  $form['mtt_settings']['tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  
   $form['mtt_settings']['tabs']['bio_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Biography info'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  );
   $form['mtt_settings']['tabs']['bio_info']['bio_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of Biography person.'),
  	'#description'   => t('Name of Biography person.'),
	'#default_value' => theme_get_setting('bio_name', 'biography'),
  );
  
    $form['mtt_settings']['tabs']['footer_text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer Text'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  ); 
   $form['mtt_settings']['tabs']['footer_text']['copyright'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright text'),
  	'#description'   => t('Text appear after &copy; symbol.'),
	'#default_value' => theme_get_setting('copyright', 'biography'),
  );
  
    $form['mtt_settings']['tabs']['theme_attribute'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme developer credit'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  );  
    $form['mtt_settings']['tabs']['theme_attribute']['biography_attribute'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show theme developer credit'),
    '#description' => t('if unchecked then theme developer credit in the footer will disappear <br>so you don\'t need touch the code'),
    '#default_value' => theme_get_setting('biography_attribute', 'biography'),
  );
   
}