<?php
/**
 * @file
 * particle_emitter.config_particles.inc.
 */

 /**
  * Implements hook_form().
  */
function particle_emitter_particles_form_inc($delta) {

  $form['particle_emitter_group_particles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Particles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'particle_emitter',
    '#weight' => 2,
  );

  $particle_emitter_particle_count_default = array($delta => 10);
  $particle_emitter_particle_count = variable_get('particle_emitter_particle_count', $particle_emitter_particle_count_default);
  if (empty($particle_emitter_particle_count[$delta])) {
    $particle_emitter_particle_count[$delta] = $particle_emitter_particle_count_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_particle_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Particle count'),
    '#description' => t('Define the amount of particles.'),
    '#default_value' => $particle_emitter_particle_count[$delta],
    '#size' => 4,
    '#maxlength' => 4,
    '#required' => FALSE,
  );

  $particle_emitter_particle_composite_type_default = array($delta => 'darker');
  $particle_emitter_particle_composite_type = variable_get('particle_emitter_particle_composite_type', $particle_emitter_particle_composite_type_default);
  if (empty($particle_emitter_particle_composite_type[$delta])) {
    $particle_emitter_particle_composite_type[$delta] = $particle_emitter_particle_composite_type_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_particle_composite_type'] = array(
    '#type' => 'select',
    '#title' => t('Particle FX'),
    '#options' => _particle_emitter_get_composite_types(),
    '#default_value' => $particle_emitter_particle_composite_type[$delta],
    '#description' => t('Specify effect of particles depending on composite type per frame.'),
  );

  $form['particle_emitter_group_particles']['particle_emitter_group_particles_life'] = array(
    '#type' => 'fieldset',
    '#title' => t('Particle life'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_particle_life_default = array($delta => 'random');
  $particle_emitter_particle_life = variable_get('particle_emitter_particle_life', $particle_emitter_particle_life_default);
  if (empty($particle_emitter_particle_life[$delta])) {
    $particle_emitter_particle_life[$delta] = $particle_emitter_particle_life_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_life']['particle_emitter_particle_life'] = array(
    '#type' => 'select',
    '#title' => t('Life type'),
    '#options' => array(
      'random' => t('Scope'),
      'fixed' => t('Fixed')),
    '#default_value' => $particle_emitter_particle_life[$delta],
    '#description' => t('Select the life mode. "Fixed" lifetime will use "Lifetime fixed", "Scope" will use a random value between "Lifetime min" and "Lifetime max".'),
  );

  $particle_emitter_particle_life_fix_default = array($delta => 10);
  $particle_emitter_particle_life_fix = variable_get('particle_emitter_particle_life_fix', $particle_emitter_particle_life_fix_default);
  if (empty($particle_emitter_particle_life_fix[$delta])) {
    $particle_emitter_particle_life_fix[$delta] = $particle_emitter_particle_life_fix_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_life']['particle_emitter_particle_life_fix'] = array(
    '#type' => 'textfield',
    '#title' => t('Lifetime fixed'),
    '#description' => t('Define fixed lifetime.'),
    '#default_value' => $particle_emitter_particle_life_fix[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $particle_emitter_particle_life_min_default = array($delta => 10);
  $particle_emitter_particle_life_min = variable_get('particle_emitter_particle_life_min', $particle_emitter_particle_life_min_default);
  if (empty($particle_emitter_particle_life_min[$delta])) {
    $particle_emitter_particle_life_min[$delta] = $particle_emitter_particle_life_min_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_life']['particle_emitter_particle_life_min'] = array(
    '#type' => 'textfield',
    '#title' => t('Lifetime min'),
    '#description' => t('Define minimum lifetime.'),
    '#default_value' => $particle_emitter_particle_life_min[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $particle_emitter_particle_life_max_default = array($delta => 20);
  $particle_emitter_particle_life_max = variable_get('particle_emitter_particle_life_max', $particle_emitter_particle_life_max_default);
  if (empty($particle_emitter_particle_life_max[$delta])) {
    $particle_emitter_particle_life_max[$delta] = $particle_emitter_particle_life_max_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_life']['particle_emitter_particle_life_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Lifetime max'),
    '#description' => t('Define maximum lifetime.'),
    '#default_value' => $particle_emitter_particle_life_max[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Particle Types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_particle_type_default = array($delta => 'circle');
  $particle_emitter_particle_type = variable_get('particle_emitter_particle_type', $particle_emitter_particle_type_default);
  if (empty($particle_emitter_particle_type[$delta])) {
    $particle_emitter_particle_type[$delta] = $particle_emitter_particle_type_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_particle_type'] = array(
    '#type' => 'select',
    '#title' => t('Particle type'),
    '#options' => array(
      'circle' => t('Circle'),
      'rectangle' => t('Rectangle'),
      'image' => t('Image'),
      'text' => t('Text')),
    '#default_value' => $particle_emitter_particle_type[$delta],
    '#description' => t('Select the particle type.'),
  );

  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_circle'] = array(
    '#type' => 'fieldset',
    '#title' => t('Circles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_particle_radius_default = array($delta => 'random');
  $particle_emitter_particle_radius = variable_get('particle_emitter_particle_radius', $particle_emitter_particle_radius_default);
  if (empty($particle_emitter_particle_radius[$delta])) {
    $particle_emitter_particle_radius[$delta] = $particle_emitter_particle_radius_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_circle']['particle_emitter_particle_radius'] = array(
    '#type' => 'select',
    '#title' => t('Radius'),
    '#options' => array(
      'random' => t('Scope'),
      'fixed' => t('Fixed')),
    '#default_value' => variable_get('particle_emitter_particle_radius', $particle_emitter_particle_radius[$delta]),
    '#description' => t('Select the radius mode. "Fixed" radius will use "Radius fixed", "Scope" will use a random value between "Radius min" and "Radius max".'),
  );

  $particle_emitter_particle_radius_transform_default = array($delta => 'shrink');
  $particle_emitter_particle_radius_transform = variable_get('particle_emitter_particle_radius_transform', $particle_emitter_particle_radius_transform_default);
  if (empty($particle_emitter_particle_radius_transform[$delta])) {
    $particle_emitter_particle_radius_transform[$delta] = $particle_emitter_particle_radius_transform_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_circle']['particle_emitter_particle_radius_transform'] = array(
    '#type' => 'select',
    '#title' => t('Transformation'),
    '#options' => array(
      'off' => t('Off'),
      'shrink' => t('Shrink'),
      'grow' => t('Grow')),
    '#default_value' => $particle_emitter_particle_radius_transform[$delta],
    '#description' => t('Select the radius transformation mode. Shrinking will affect the lifetime and particle will die if radius becomes 0.'),
  );

  $particle_emitter_particle_radius_fix_default = array($delta => 10);
  $particle_emitter_particle_radius_fix = variable_get('particle_emitter_particle_radius_fix', $particle_emitter_particle_radius_fix_default);
  if (empty($particle_emitter_particle_radius_fix[$delta])) {
    $particle_emitter_particle_radius_fix[$delta] = $particle_emitter_particle_radius_fix_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_circle']['particle_emitter_particle_radius_fix'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius fixed'),
    '#description' => t('Define fixed radius.'),
    '#default_value' => $particle_emitter_particle_radius_fix[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $particle_emitter_particle_radius_min_default = array($delta => 10);
  $particle_emitter_particle_radius_min = variable_get('particle_emitter_particle_radius_min', $particle_emitter_particle_radius_min_default);
  if (empty($particle_emitter_particle_radius_min[$delta])) {
    $particle_emitter_particle_radius_min[$delta] = $particle_emitter_particle_radius_min_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_circle']['particle_emitter_particle_radius_min'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius min'),
    '#description' => t('Define minimum radius.'),
    '#default_value' => $particle_emitter_particle_radius_min[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $particle_emitter_particle_radius_max_default = array($delta => 30);
  $particle_emitter_particle_radius_max = variable_get('particle_emitter_particle_radius_max', $particle_emitter_particle_radius_max_default);
  if (empty($particle_emitter_particle_radius_max[$delta])) {
    $particle_emitter_particle_radius_max[$delta] = $particle_emitter_particle_radius_max_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_circle']['particle_emitter_particle_radius_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius max'),
    '#description' => t('Define maximum radius.'),
    '#default_value' => $particle_emitter_particle_radius_max[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_rectangle'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rectangles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_particle_rect_transform_default = array($delta => 'shrink');
  $particle_emitter_particle_rect_transform = variable_get('particle_emitter_particle_rect_transform', $particle_emitter_particle_rect_transform_default);
  if (empty($particle_emitter_particle_rect_transform[$delta])) {
    $particle_emitter_particle_rect_transform[$delta] = $particle_emitter_particle_rect_transform_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_rectangle']['particle_emitter_particle_rect_transform'] = array(
    '#type' => 'select',
    '#title' => t('Transformation'),
    '#options' => array(
      'off' => t('Off'),
      'shrink' => t('Shrink'),
      'grow' => t('Grow')),
    '#default_value' => $particle_emitter_particle_rect_transform[$delta],
    '#description' => t('Select the rectangle transformation mode. Shrinking will affect the lifetime and particle will die if a side-length becomes 0.'),
  );

  $particle_emitter_particle_rect_width_default = array($delta => 10);
  $particle_emitter_particle_rect_width = variable_get('particle_emitter_particle_rect_width', $particle_emitter_particle_rect_width_default);
  if (empty($particle_emitter_particle_rect_width[$delta])) {
    $particle_emitter_particle_rect_width[$delta] = $particle_emitter_particle_rect_width_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_rectangle']['particle_emitter_particle_rect_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Rectangle width'),
    '#description' => t('Define rectangle width.'),
    '#default_value' => $particle_emitter_particle_rect_width[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $particle_emitter_particle_rect_height_default = array($delta => 10);
  $particle_emitter_particle_rect_height = variable_get('particle_emitter_particle_rect_height', $particle_emitter_particle_rect_height_default);
  if (empty($particle_emitter_particle_rect_height[$delta])) {
    $particle_emitter_particle_rect_height[$delta] = $particle_emitter_particle_rect_height_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_rectangle']['particle_emitter_particle_rect_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Rectangle height'),
    '#description' => t('Define rectangle height.'),
    '#default_value' => $particle_emitter_particle_rect_height[$delta],
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
  );

  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_image'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_particle_image_transform_default = array($delta => 'off');
  $particle_emitter_particle_image_transform = variable_get('particle_emitter_particle_image_transform', $particle_emitter_particle_image_transform_default);
  if (empty($particle_emitter_particle_image_transform[$delta])) {
    $particle_emitter_particle_image_transform[$delta] = $particle_emitter_particle_image_transform_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_image']['particle_emitter_particle_image_transform'] = array(
    '#type' => 'select',
    '#title' => t('Transformation'),
    '#options' => array(
      'off' => t('Off'),
      'shrink' => t('Shrink'),
      'grow' => t('Grow')),
    '#default_value' => $particle_emitter_particle_image_transform[$delta],
    '#description' => t('Select the image transformation mode. Shrinking will affect the lifetime and particle will die if a width or height becomes 0.'),
  );

  $images = variable_get("particle_emitter_images");
  $image_data = array();
  for ($i = 0; $i < count($images); $i++) {
    $img = $images[$i]['file']['file_managed_container']['file_managed_element'];
    if($img != 0) {
      $image_file = file_load($img);
      if (isset($image_file->fid) && isset($image_file->filename)) {
        $image_data[$image_file->fid] = $image_file->filename;
      }
    }
  }
  $particle_emitter_particle_image_set_default = array($delta => array());
  $particle_emitter_particle_image_set = variable_get('particle_emitter_particle_image_set', $particle_emitter_particle_image_set_default);
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_image']['particle_emitter_particle_image_set'] = array(
    '#title' => t('Images'),
    '#type' => 'checkboxes',
    '#options' => $image_data,
    '#default_value' => $particle_emitter_particle_image_set[$delta],
    '#description' => t('Select the images to use.'),
  );

  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $particle_emitter_particle_text_string_default = array($delta => 'Drupal');
  $particle_emitter_particle_text_string = variable_get('particle_emitter_particle_text_string', $particle_emitter_particle_text_string_default);
  if (empty($particle_emitter_particle_text_string[$delta])) {
    $particle_emitter_particle_text_string[$delta] = $particle_emitter_particle_text_string_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_text']['particle_emitter_particle_text_string'] = array(
    '#type' => 'textfield',
    '#title' => t('Text string'),
    '#description' => t('Define a text string.'),
    '#default_value' => $particle_emitter_particle_text_string[$delta],
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => FALSE,
  );

  $particle_emitter_particle_text_style_default = array($delta => 'bold 36px Arial');
  $particle_emitter_particle_text_style = variable_get('particle_emitter_particle_text_style', $particle_emitter_particle_text_style_default);
  if (empty($particle_emitter_particle_text_style[$delta])) {
    $particle_emitter_particle_text_style[$delta] = $particle_emitter_particle_text_style_default;
  }
  $form['particle_emitter_group_particles']['particle_emitter_group_particles_types']['particle_emitter_group_particles_text']['particle_emitter_particle_text_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Text style'),
    '#description' => t('Define the font style.'),
    '#default_value' => $particle_emitter_particle_text_style[$delta],
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => FALSE,
  );
  
  return $form;
}
