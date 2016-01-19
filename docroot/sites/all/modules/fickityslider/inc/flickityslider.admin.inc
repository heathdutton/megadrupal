<?php
/**
 * @file
 * Administrative page callbacks for the flickityslider module.
 */


/**
 * Form builder; Form for advanced module settings.
 */
function flickityslider_form_settings() {
  $form = array();

  $form['flickity_basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Settings'),
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );

  // Slider Autoplay.
  $form['flickity_basic']['flickity_autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay Slider'),
    '#description' => t('Check this box to autoplay the slider.'),
    '#default_value' => variable_get('flickity_autoplay', FALSE),
    '#access' => user_access('administer flickityslider'),
  );
  
  //Autoplay Time.
  $form['flickity_basic']['autoplaytime'] = array(
    '#type' => 'textfield', 
    '#title' => t('Time'),
    '#description' => t('Number of milliseconds.'),
    '#default_value' => variable_get('autoplaytime', 5000),
    '#size' => 20,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#states' => array(
      // Hide the textfield when the autoplay checkbox is disabled.
      'invisible' => array(
        ':input[name="flickity_autoplay"]' => array('checked' => FALSE),
      ),
    ),
  );
  
  //Prev/Next Arrows.
  $form['flickity_basic']['flickity_prevnext'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Prev/Next Arrows'),
    '#description' => t('Check this box to display prev/next arrows on the slider.'),
    '#default_value' => variable_get('flickity_prevnext', TRUE),
    '#access' => user_access('administer flickityslider'),
  );
  
  return system_settings_form($form);
}
