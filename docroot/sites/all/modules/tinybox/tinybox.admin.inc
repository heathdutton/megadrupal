<?php
/**
 * @file
 * Menu builder functions for TinyBox menu.
 */

function tinybox_settings() {
  $tinybox_lib_path = variable_get('tinybox_lib_path', 'sites/all/libraries/tinybox');
  
  if (!file_exists($tinybox_lib_path . '/tinybox.js')) {
    drupal_set_message('Can not find: ' . $tinybox_lib_path . '/tinybox.js. You must upload TinyBox library to: ' . $tinybox_lib_path, 'error');
  }
  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#weight' => 0,
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['general']['tinybox_lib_path'] = array(
    '#type' => 'textfield',
    '#title' => t('TinyBox Library Path'),
    '#default_value' => variable_get('tinybox_lib_path', 'sites/all/libraries/tinybox'),
    '#description' => t('Download TinyBox library from: <a href="http://www.scriptiny.com/2011/03/javascript-modal-windows/" target="_blank">TinyBox Library download</a> then upload to this path.'),
  );

  $form['tinybox_splash'] = array(
    '#type' => 'fieldset',
    '#weight' => 1,
    '#title' => t('TinyBox Splash'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['tinybox_splash']['tinybox_splash_source_type'] = array(
    '#type' => 'select',
    '#title' => t('Splash Source Type'),
    '#options' => array(
      'nid' => 'NID',
      'content_type' => 'Content Type',
      'views' => 'Views',
      ),
    '#default_value' => variable_get('tinybox_splash_source_type', 'nid')
  );
  
  $form['tinybox_splash']['tinybox_splash_source_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Splash Source Value'),
    '#default_value' => variable_get('tinybox_splash_source_value', ''),
    '#description' => t('Leave blank to disable Splash. Enter NID, Content Type or Views name.'),
  );

  $form['tinybox_splash']['tinybox_splash_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#default_value' => variable_get('tinybox_splash_delay', 0),
    '#description' => t('Set 0 to disable. Delay in second before next display. If you set 3600 then Splash Screen will be show again after 3600 seconds.'),
  );
  
  $form['tinybox_splash']['tinybox_splash_php'] = array(
    '#type' => 'textarea',
    '#title' => t('Show on which this PHP code returns TRUE (experts only)'),
    '#default_value' => variable_get('tinybox_splash_php', ''),
    '#description' => t('Enter PHP code between <?php ?>. Note that executing incorrect PHP code can break your Drupal site. <em>PHP Filter</em> module must enable to use this feature'),
  );
  
  $form['tinybox_setting'] = array(
    '#type' => 'fieldset',
    '#weight' => 2,
    '#title' => t('TinyBox Setting'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['tinybox_setting']['tinybox_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => variable_get('tinybox_width', 0),
    '#description' => t('Set 0 for for auto width.'),
  );
  
  $form['tinybox_setting']['tinybox_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => variable_get('tinybox_height', 0),
    '#description' => t('Set 0 for for auto height.'),
  );
  
  $form['tinybox_setting']['tinybox_autohide'] = array(
    '#type' => 'textfield',
    '#title' => t('Autohide'),
    '#default_value' => variable_get('tinybox_autohide', 0),
    '#description' => t('Set 0 to disable. Autohide Tinybox after certain seconds.'),
  );
  
  $form['tinybox_setting']['tinybox_css'] = array(
    '#type' => 'textfield',
    '#title' => t('Tinybox CSS'),
    '#default_value' => variable_get('tinybox_css', ''),
    '#description' => t('Leave blank for default: sites/all/modules/tinybox'),
  );
  
  return system_settings_form($form);
}

