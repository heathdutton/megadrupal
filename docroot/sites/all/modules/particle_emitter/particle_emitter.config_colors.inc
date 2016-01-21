<?php
/**
 * @file
 * particle_emitter.config_colors.inc.
 */

 /**
  * Implements hook_form().
  */
function particle_emitter_colors_form_inc($delta) {

  $particle_emitter_canvas_bgcolor_default = array($delta => '#FFFFFF');
  $particle_emitter_canvas_bgcolor = variable_get('particle_emitter_canvas_bgcolor', $particle_emitter_canvas_bgcolor_default);
  if (empty($particle_emitter_canvas_bgcolor[$delta])) {
    $particle_emitter_canvas_bgcolor[$delta] = $particle_emitter_canvas_bgcolor_default[$delta];
  }
  $particle_emitter_particles_color_hex_default = array($delta => '#000000');
  $particle_emitter_particles_color_hex = variable_get('particle_emitter_particles_color_hex', $particle_emitter_particles_color_hex_default);
  if (empty($particle_emitter_particles_color_hex[$delta])) {
    $particle_emitter_particles_color_hex[$delta] = $particle_emitter_particles_color_hex_default[$delta];
  }
  $js_settings = array(
    'canvas_bgcolor' => $particle_emitter_canvas_bgcolor[$delta],
    'particles_color_hex' => $particle_emitter_particles_color_hex[$delta],
  );
  drupal_add_js(array('particle_emitter' => $js_settings), 'setting');

  $form['particle_emitter_group_colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colors'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'particle_emitter',
    '#weight' => 4,
  );

  $form['particle_emitter_group_colors']['particle_emitter_group_colors_canvas'] = array(
    '#type' => 'fieldset',
    '#title' => t('Canvas Coloration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_canvas_fill_style_default = array($delta => 'full');
  $particle_emitter_canvas_fill_style = variable_get('particle_emitter_canvas_fill_style', $particle_emitter_canvas_fill_style_default);
  if (empty($particle_emitter_canvas_fill_style[$delta])) {
    $particle_emitter_canvas_fill_style[$delta] = $particle_emitter_canvas_fill_style_default;
  }
  $form['particle_emitter_group_colors']['particle_emitter_group_colors_canvas']['particle_emitter_canvas_fill_style'] = array(
    '#type' => 'select',
    '#title' => t('Fill Style'),
    '#options' => array(
      'full' => t('Full')),
    '#default_value' => $particle_emitter_canvas_fill_style[$delta],
    '#description' => t('Select the filling type for the canvas.'),
  );

  $particle_emitter_canvas_bgcolor_default = array($delta => '#ffffff');
  $particle_emitter_canvas_bgcolor = variable_get('particle_emitter_canvas_bgcolor', $particle_emitter_canvas_bgcolor_default);
  if (empty($particle_emitter_canvas_bgcolor[$delta])) {
    $particle_emitter_canvas_bgcolor[$delta] = $particle_emitter_canvas_bgcolor_default;
  }
  $form['particle_emitter_group_colors']['particle_emitter_group_colors_canvas']['particle_emitter_canvas_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Canvas bg color'),
    '#description' => '<div id="particle_emitter_color_picker_canvas"></div>',
    '#default_value' => $particle_emitter_canvas_bgcolor[$delta],
    '#size' => 7,
    '#maxlength' => 7,
    '#required' => FALSE,
    '#attached' => array(
      'library' => array(
        array('system', 'farbtastic'),
      ),
      'js' => array(drupal_get_path('module', 'particle_emitter') . '/js/particle_emitter.color_picker.js'),
    ),
  );

  $form['particle_emitter_group_colors']['particle_emitter_group_colors_particles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Particles Coloration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_particles_fill_style_default = array($delta => 'gradient');
  $particle_emitter_particles_fill_style = variable_get('particle_emitter_particles_fill_style', $particle_emitter_particles_fill_style_default);
  if (empty($particle_emitter_particles_fill_style[$delta])) {
    $particle_emitter_particles_fill_style[$delta] = $particle_emitter_particles_fill_style_default;
  }
  $form['particle_emitter_group_colors']['particle_emitter_group_colors_particles']['particle_emitter_particles_fill_style'] = array(
    '#type' => 'select',
    '#title' => t('Fill Style'),
    '#options' => array(
      'gradient' => t('Gradient'),
      'full' => t('Full')),
    '#default_value' => $particle_emitter_particles_fill_style[$delta],
    '#description' => t('Select the filling style for the particles.'),
  );

  $particle_emitter_particles_fill_type_default = array($delta => 'random');
  $particle_emitter_particles_fill_type = variable_get('particle_emitter_particles_fill_type', $particle_emitter_particles_fill_type_default);
  if (empty($particle_emitter_particles_fill_type[$delta])) {
    $particle_emitter_particles_fill_type[$delta] = $particle_emitter_particles_fill_type_default;
  }
  $form['particle_emitter_group_colors']['particle_emitter_group_colors_particles']['particle_emitter_particles_fill_type'] = array(
    '#type' => 'select',
    '#title' => t('Fill Type'),
    '#options' => array(
      'random' => t('Random'),
      'fixed' => t('Fixed')),
    '#default_value' => $particle_emitter_particles_fill_type[$delta],
    '#description' => t('Select the filling type for the particles. Colors will be applied randomly or by the defined rgb color below.'),
  );

  $particle_emitter_particles_color_hex_default = array($delta => '#000000');
  $particle_emitter_particles_color_hex = variable_get('particle_emitter_particles_color_hex', $particle_emitter_particles_color_hex_default);
  if (empty($particle_emitter_particles_color_hex[$delta])) {
    $particle_emitter_particles_color_hex[$delta] = $particle_emitter_particles_color_hex_default;
  }
  $form['particle_emitter_group_colors']['particle_emitter_group_colors_particles']['particle_emitter_particles_color_hex'] = array(
    '#type' => 'textfield',
    '#title' => t('Choose a color:'),
    '#description' => '<div id="particle_emitter_color_picker_particles"></div>',
    '#default_value' => $particle_emitter_particles_color_hex[$delta],
    '#size' => 7,
    '#maxlength' => 7,
    '#required' => FALSE,
    '#attached' => array(
      'library' => array(
        array('system', 'farbtastic'),
      ),
      'js' => array(drupal_get_path('module', 'particle_emitter') . '/js/particle_emitter.color_picker.js'),
    ),
  );

  return $form;
}
