<?php
/**
 * @file
 * particle_emitter.config_canvas.inc.
 */

 /**
  * Implements hook_form().
  */
function particle_emitter_canvas_form_inc($delta) {

  $form['particle_emitter_group_canvas'] = array(
    '#type' => 'fieldset',
    '#title' => t('Canvas'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'particle_emitter',
    '#weight' => 3,
  );

  $particle_emitter_canvas_width_default = array($delta => 0);
  $particle_emitter_canvas_width = variable_get('particle_emitter_canvas_width', $particle_emitter_canvas_width_default);
  if (empty($particle_emitter_canvas_width[$delta])) {
    $particle_emitter_canvas_width[$delta] = $particle_emitter_canvas_width_default;
  }
  $form['particle_emitter_group_canvas']['particle_emitter_canvas_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Canvas width'),
    '#description' => t('Canvas width (px).'),
    '#default_value' => $particle_emitter_canvas_width[$delta],
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => FALSE,
  );

  $particle_emitter_canvas_height_default = array($delta => 0);
  $particle_emitter_canvas_height = variable_get('particle_emitter_canvas_height', $particle_emitter_canvas_height_default);
  if (empty($particle_emitter_canvas_height[$delta])) {
    $particle_emitter_canvas_height[$delta] = $particle_emitter_canvas_height_default;
  }
  $form['particle_emitter_group_canvas']['particle_emitter_canvas_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Canvas height'),
    '#description' => t('Canvas height (px).'),
    '#default_value' => $particle_emitter_canvas_height[$delta],
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => FALSE,
  );

  $particle_emitter_canvas_composite_type_default = array($delta => 'source-over');
  $particle_emitter_canvas_composite_type = variable_get('particle_emitter_canvas_composite_type', $particle_emitter_canvas_composite_type_default);
  if (empty($particle_emitter_canvas_composite_type[$delta])) {
    $particle_emitter_canvas_composite_type[$delta] = $particle_emitter_canvas_composite_type_default;
  }
  $form['particle_emitter_group_canvas']['particle_emitter_canvas_composite_type'] = array(
    '#type' => 'select',
    '#title' => t('Canvas FX'),
    '#options' => _particle_emitter_get_composite_types(),
    '#default_value' => $particle_emitter_canvas_composite_type[$delta],
    '#description' => t('Specify effect of canvas depending on composite type per frame.'),
  );

  return $form;
}
