<?php
// $Id:

function shallowgrunge_form_system_theme_settings_alter(&$form, $form_state) {

drupal_add_js(drupal_get_path('theme', 'shallowgrunge') . '/color.js');

  // Create the form widgets using Forms API
  $form['themecolor'] = array(
      '#type' => 'fieldset',
      '#title' => t('Theme Color'),
      '#description' => t('Insert hex code. Include the # sign.<br />
                           Darker colors tend to work better for this theme.
                           <br />The HTML 5 color picker will only show if your
                           browser supports it.<br />
                           <a onclick=\'ShallowGrungeResetColors();\'>Reset Colors</a>'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
  $form['themecolor']['headcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Header color'),
    '#default_value' => theme_get_setting('headcolor'),
    '#size' => 7,
    '#maxlength' => 7,   
    '#attributes' => array('onfocus' => 'type="color"') 
  );
  $form['themecolor']['navcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Navigation color'),
    '#default_value' => theme_get_setting('navcolor'),
    '#size' => 7,
    '#maxlength' => 7, 
    '#attributes' => array('onfocus' => 'type="color"')
  );
  $form['themecolor']['headingscolor'] = array(
      '#type' => 'textfield',
      '#title' => t('Headings color'),
      '#default_value' => theme_get_setting('headingscolor'),
      '#size' => 7,
      '#maxlength' => 7, 
      '#attributes' => array('onfocus' => 'type="color"')
    );
  $form['themecolor']['linkcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Link color'),
    '#default_value' => theme_get_setting('linkcolor'),
    '#size' => 7,
    '#maxlength' => 7, 
    '#attributes' => array('onfocus' => 'type="color"')
  );
  $form['grunge'] = array(
    '#type' => 'radios',
    '#title' => t('Grunge'),
    '#default_value' => theme_get_setting('grunge'),
    '#options' => array( 'grunge1' => t('Grunge 1'), 
                         'grunge2' => t('Grunge 2'), 
                         'grunge3' => t('Grunge 3'), 
                         'grunge4' => t('Grunge 4')
                        ),
  );
  
  // Return the additional form widgets
  return $form;
}
