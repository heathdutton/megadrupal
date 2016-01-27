<?php

/**
 * @file livestream.inc
 * 
 * Main (ctools) plugin file for "livestream" plugin type
 */

$plugin = array(
    'title' => t('Verify Element Position & Dimensions'), 
    'description' => t('Verify Element Position & Dimensions'), 
    'html tag name' => 'live-stream', 
    
    // hooks 
    'hook_preprocess_fb_social_plugin' => '_drulenium_livestream_preprocess_fb_social_plugin' 
);

function drulenium_livestream_defaults() {
  return array(
      'event_app_id' => variable_get('fb_social_appid', ''), 
      'width' => '', 
      'height' => '', 
      'posx' => '', 
      'posy' => '', 
      'pages' => '',
  	  'locator' => '',
  );
}

function drulenium_livestream_fb_settings($options) {
  
  $form = array();
  $form['locator'] = array(
      '#type' => 'textfield', 
      '#title' => t('Element Locator'), 
      '#description' => t('The Locator of the element you want to Verify.') 
  );
  $form['width'] = array(
      '#type' => 'textfield', 
      '#title' => t('Width'), 
      '#description' => t('The Width of the element you want to Verify.') 
  );
  
  $form['height'] = array(
      '#type' => 'textfield', 
      '#title' => t('Height'), 
      '#description' => t('The Height of the element you want to Verify.') 
  );
  
  $form['posx'] = array(
      '#type' => 'textfield', 
      '#title' => t('Left Position'), 
      '#description' => t('The Left Position of the element you want to Verify.') 
  );   
  
  $form['posy'] = array(
      '#type' => 'textfield', 
      '#title' => t('Top Position'), 
      '#description' => t('The Top Position of the element you want to Verify.') 
  );  
   /*
  $form['pages'] = array(
      '#type' => 'textarea', 
      '#title' => t('Pages'), 
      '#description' => t('The pages you want to Test') 
  );
  */
  $defaults = drulenium_livestream_defaults();
  
  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }
  
  return $form;
}

function drulenium_livestream_drupal_settings($options) {
  return array();
}

function _drulenium_livestream_preprocess_fb_social_plugin(&$variables) {
  $options = &$variables['options'];
  $options['always_post_to_friends'] = $options['always_post_to_friends'] ? "true" : "false";
}

