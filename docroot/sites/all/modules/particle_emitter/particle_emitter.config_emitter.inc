<?php
/**
 * @file
 * particle_emitter.config_emitter.inc.
 */

 /**
  * Implements hook_form().
  */
function particle_emitter_emitter_form_inc($delta) {

  $form['particle_emitter_group_emitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Emitter'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'particle_emitter',
    '#weight' => 1,
  );

  $particle_emitter_emitter_position_type_default = array($delta => 'center');
  $particle_emitter_emitter_position_type = variable_get('particle_emitter_emitter_position_type', $particle_emitter_emitter_position_type_default);
  if (empty($particle_emitter_emitter_position_type[$delta])) {
    $particle_emitter_emitter_position_type[$delta] = $particle_emitter_emitter_position_type_default;
  }
  $form['particle_emitter_group_emitter']['particle_emitter_emitter_position_type'] = array(
    '#type' => 'select',
    '#title' => t('Emitter location in canvas'),
    '#options' => array(
      'center' => t('Center'),
      'mouse' => t('Mouse'),
      'offset' => t('Offset')),
    '#default_value' => $particle_emitter_emitter_position_type[$delta],
    '#description' => t('Select emitter location in canvas.'),
  );

  $particle_emitter_emitter_offset_x_default = array($delta => 0);
  $particle_emitter_emitter_offset_x = variable_get('particle_emitter_emitter_offset_x', $particle_emitter_emitter_offset_x_default);
  if (empty($particle_emitter_emitter_offset_x[$delta])) {
    $particle_emitter_emitter_offset_x[$delta] = $particle_emitter_emitter_offset_x_default;
  }
  $form['particle_emitter_group_emitter']['particle_emitter_emitter_offset_x'] = array(
    '#type' => 'textfield',
    '#title' => t('Emitter X-offset'),
    '#description' => t('Define the x-offset.'),
    '#default_value' => $particle_emitter_emitter_offset_x[$delta],
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => FALSE,
  );

  $particle_emitter_emitter_offset_y_default = array($delta => 0);
  $particle_emitter_emitter_offset_y = variable_get('particle_emitter_emitter_offset_y', $particle_emitter_emitter_offset_y_default);
  if (empty($particle_emitter_emitter_offset_y[$delta])) {
    $particle_emitter_emitter_offset_y[$delta] = $particle_emitter_emitter_offset_y_default;
  }
  $form['particle_emitter_group_emitter']['particle_emitter_emitter_offset_y'] = array(
    '#type' => 'textfield',
    '#title' => t('Emitter Y-offset'),
    '#description' => t('Define the y-offset.'),
    '#default_value' => $particle_emitter_emitter_offset_y[$delta],
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => FALSE,
  );

  return $form;
}
