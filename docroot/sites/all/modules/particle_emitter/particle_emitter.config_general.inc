<?php
/**
 * @file
 * particle_emitter.config_general.inc.
 */

 /**
  * Implements hook_form().
  */
function particle_emitter_general_form_inc() {

  $form['particle_emitter_group_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Particle Emitter'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['particle_emitter_group_general']['particle_emitter_frame_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Frame Rate'),
    '#description' => t('Frame rate definition (fps - frame per second). Default value is 30 fps = (1 / 30) * 1000 = 33 milliseconds per frame.'),
    '#default_value' => variable_get('particle_emitter_frame_rate', 30),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}
