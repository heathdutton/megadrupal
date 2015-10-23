<?php

/**
 * @file
 * TODO.
 */

/**
 * Form builder function for module settings.
 */
function rotating_slogan_settings() {

  $form = array();

  $form['rotating_slogan_list'] = array(
  
    '#type' => 'textarea',
    '#title' => t('Text list'),
    '#default_value' => variable_get('rotating_slogan_list', ''),
    '#description' => t('Enter one text per line.'),
  );
  
  $form['rotating_slogan_type'] = array(
  
    '#type' => 'select',
    '#title' => t('Item to change'),
    '#default_value' => variable_get('rotating_slogan_type', 'site_slogan'),
    '#options' => array(
    
      'site_slogan' => t('Slogan'),
      'site_name' => t('Site name'),
    ),
  );
  
  $form['rotating_slogan_cycle'] = array(
  
    '#type' => 'select',
    '#title' => t('Cycling methode'),
    '#default_value' => variable_get('rotating_slogan_cycle', 'random'),
    '#options' => array(
    
      'random' => t('Random'),
      'cycling' => t('Cycle'),
    ),
  );

  return system_settings_form($form);
}
