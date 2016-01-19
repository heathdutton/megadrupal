<?php
/**
 * @file
 * particle_emitter.config_global.inc.
 */

 /**
  * Implements hook_form().
  */
function particle_emitter_global_form_inc($delta) {

  $form['particle_emitter_group_global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Particle Emitter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'particle_emitter',
    '#weight' => 0,
  );

  $particle_emitter_container_default = array(
    $delta => '#' . str_replace('_', '-', $delta),
  );
  $particle_emitter_container = variable_get('particle_emitter_container', $particle_emitter_container_default);
  if (empty($particle_emitter_container[$delta])) {
    $particle_emitter_container[$delta] = $particle_emitter_container_default;
  }
  $form['particle_emitter_group_global']['particle_emitter_container'] = array(
    '#type' => 'textfield',
    '#title' => t('Canvas Container'),
    '#description' => t('Selector for the html-element displaying the particle_emitter canvas. The default container is "#particle_emitter", alternatively any other selector can be used ("#page", ".myclass", "#page .myclass", etc).'),
    '#default_value' => $particle_emitter_container[$delta],
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => FALSE,
    '#disabled' => TRUE,
  );

  // $form['particle_emitter_group_global']['particle_emitter_factory_reset'] = array(
  // '#type' => 'checkbox',
  // '#title' => t('Reset configuration to default values.'),
  // );
  return $form;
}

 /**
  * Implements hook_form_submit().
  */
// function particle_emitter_global_form_submit($form, &$form_state) {
//
//  if (intval($form_state['values']['particle_emitter_factory_reset']) == 1) {
//    particle_emitter_factory_reset();
//  }
//  else {
//    variable_set('particle_emitter_container', $form_state['values']['particle_emitter_container']);
//    variable_set('particle_emitter_frame_rate', $form_state['values']['particle_emitter_frame_rate']);
//  }
//  drupal_set_message(t('The settings have been saved'));
// }

 /**
  * Resets all default values.
  */
// function particle_emitter_factory_reset() {
//
//  variable_set('particle_emitter_container', '#particle-emitter');
//  variable_set('particle_emitter_frame_rate', 30);
//  variable_set('particle_emitter_particle_canvas_width', 0);
//  variable_set('particle_emitter_particle_canvas_height', 0);
//  variable_set('particle_emitter_canvas_composite_type', 'source-over');
//  variable_set('particle_emitter_canvas_fill_style', 'full');
//  variable_set('particle_emitter_canvas_bgcolor', '#FFFFFF');
//  variable_set('particle_emitter_particle_count', 10);
//  variable_set('particle_emitter_particle_composite_type', 'darker');
//  variable_set('particle_emitter_particle_life', 'random');
//  variable_set('particle_emitter_particle_life_fix', 10);
//  variable_set('particle_emitter_particle_life_min', 10);
//  variable_set('particle_emitter_particle_life_max', 20);
//  variable_set('particle_emitter_particle_type', 'circle');
//  variable_set('particle_emitter_particle_radius', 'random');
//  variable_set('particle_emitter_particle_radius_transform', 'shrink');
//  variable_set('particle_emitter_particle_radius_fix', 10);
//  variable_set('particle_emitter_particle_radius_min', 10);
//  variable_set('particle_emitter_particle_radius_max', 30);
//  variable_set('particle_emitter_particles_fill_style', 'gradient');
//  variable_set('particle_emitter_particles_fill_type', 'random');
//  variable_set('particle_emitter_particles_color_rgb_r', 0);
//  variable_set('particle_emitter_particles_color_rgb_g', 0);
//  variable_set('particle_emitter_particles_color_rgb_b', 0);
//  variable_set('particle_emitter_mouse_tracking', 'off');
// }
