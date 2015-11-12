<?php

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */

function readability_admin_settings() {
  $form['top'] = array(
    '#value' => t('Readablity uses five test to automate grading of your content.'),
  );
  
  $form['readability_advanced'] = array(
          '#type' => 'fieldset',
          '#title' => t('Target grade thresholds'),
          '#description' => t('Use the below fields to input a minium and maximum grades content should be between in order to be considered pass. Content that grades outside these thresholds will produce warning messages.'),
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
          '#tree' => TRUE,
  );
  
  $form['readability_advanced']['readability_targetgrade_min'] = array(
    '#type' => 'textfield',
    '#title' => t('Target Grade Floor'), 
    '#default_value' => variable_get('readability_targetgrade_min', READABILITY_DEFAULT_TARGETGRADE_MIN), 
  );
  
  $form['readability_advanced']['readability_targetgrade_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Target Grade Ceiling'), 
    '#default_value' => variable_get('readability_targetgrade_max', READABILITY_DEFAULT_TARGETGRADE_MAX), 
  );  

  return system_settings_form($form);
}

