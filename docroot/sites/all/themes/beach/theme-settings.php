<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 */
function beach_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['mtt_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('beach Theme Settings'),
    '#collapsible' => FALSE,
	'#collapsed' => FALSE,
  );

  $form['mtt_settings']['tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  
   $form['mtt_settings']['tabs']['beach_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('beach setting'),
    '#collapsible' => TRUE,
	'#collapsed' => TRUE,
  );

   $form['mtt_settings']['tabs']['beach_setting']['beach_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Beach width.'),
  	'#description'   => t('Set width of Beach theme like 800px, 960px, 1150px. You can alsos set width in % to set liquid width like 100%, 90% etc. .'),
	'#default_value' => theme_get_setting('beach_width', 'beach'),
  );  
   
}