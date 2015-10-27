<?php
/**
 * @file
 * Contains form handling for the gridlet's administration settings.
 */

/**
 * Admin settings form.
 */
function golden_gridlet_admin_form() {
  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grid Appearance'),
    '#description' => t('Please provide colors in hexidecimal ("#FFFFFF") and opacity in decimal format (0.618).'),
    '#collapsible' => FALSE,
    '#weight' => 50,
  );
  $form['appearance']['guide_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Guide Color'),
    '#default_value' => variable_get('golden_gridlet_guide_color', '#FFC300'),
    '#size' => 8,
    '#maxlength' => 7,
    '#required' => TRUE,
  );
  $form['appearance']['guide_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Guide Opacity'),
    '#default_value' => variable_get('golden_gridlet_guide_opacity', '0.618'),
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['appearance']['guide_inner_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Guide Inner Color'),
    '#default_value' => variable_get('golden_gridlet_guide_inner_color', '#FFFFFF'),
    '#size' => 8,
    '#maxlength' => 7,
    '#required' => TRUE,
  );
  $form['appearance']['switch_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Grid Toggle Color'),
    '#default_value' => variable_get('golden_gridlet_switch_color', '#000000'),
    '#size' => 8,
    '#maxlength' => 7,
    '#required' => TRUE,
  );
  $form['appearance']['switch_opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Grid Toggle Opacity'),
    '#default_value' => variable_get('golden_gridlet_switch_opacity', '0.618'),
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['grid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grid Settings'),
    '#collapsible' => FALSE,
    '#weight' => 0,
  );
  $form['grid']['base_font_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Base Font Size'),
    '#default_value' => variable_get('golden_gridlet_switch_opacity', '16'),
    '#field_suffix' => 'px',
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['grid']['baseline_grid_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Baseline Grid Height'),
    '#default_value' => variable_get('golden_gridlet_baseline_grid_height', '1.5'),
    '#field_suffix' => 'em',
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['grid']['eight_col_breakpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Eight Column Breakpoint Width'),
    '#default_value' => variable_get('golden_gridlet_eight_col_breakpoint', '45'),
    '#field_suffix' => 'em',
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['grid']['sixteen_col_breakpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Sixteen Column Breakpoint Width'),
    '#default_value' => variable_get('golden_gridlet_sixteen_col_breakpoint', '117'),
    '#field_suffix' => 'em',
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['grid']['gutter_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Gutter Size'),
    '#default_value' => variable_get('golden_gridlet_gutter_size', '1.5'),
    '#field_suffix' => 'em',
    '#size' => 6,
    '#maxlength' => 5,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Helper function for color validation.
 */
function _golden_gridlet_valid_color($color) {
  return preg_match('/^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$/', $color);
}

/**
 * Admin form validation.
 */
function golden_gridlet_admin_form_validate($form, &$form_state) {
  $guide_color = $form_state['values']['guide_color'];
  $guide_inner_color = $form_state['values']['guide_inner_color'];
  $guide_opacity = $form_state['values']['guide_opacity'];
  $switch_color = $form_state['values']['switch_color'];
  $switch_opacity = $form_state['values']['switch_opacity'];
  $base_font_size = $form_state['values']['base_font_size'];
  $baseline_grid_height = $form_state['values']['baseline_grid_height'];
  $eight_col_breakpoint  = $form_state['values']['eight_col_breakpoint'];
  $sixteen_col_breakpoint  = $form_state['values']['sixteen_col_breakpoint'];
  $gutter_size = $form_state['values']['gutter_size'];

  // Normalize hex color formatting.
  if (strpos($guide_color, '#') !== 0) {
    $guide_color = $form_state['values']['guide_color'] = '#' . $guide_color;
  }
  if (strpos($guide_inner_color, '#') !== 0) {
    $guide_inner_color = $form_state['values']['guide_inner_color'] = '#' . $guide_inner_color;
  }
  if (strpos($switch_color, '#') !== 0) {
    $switch_color = $form_state['values']['switch_color'] = '#' . $switch_color;
  }

  // Perform validation.
  if (!_golden_gridlet_valid_color($guide_color)) {
    form_set_error('guide_color', t('Guide Color must be a valid hex color.'));
  }
  if (!_golden_gridlet_valid_color($guide_inner_color)) {
    form_set_error('guide_inner_color', t('Guide Inner Color must be a valid hex color.'));
  }
  if (!_golden_gridlet_valid_color($switch_color)) {
    form_set_error('switch_color', t('Grid Toggle Color must be a valid hex color.'));
  }
  if (!is_numeric($guide_opacity) || $guide_opacity > 1 || $guide_opacity <= 0) {
    form_set_error('guide_opacity', t('Guide Opacity must be a decimal value between 0 and 1.'));
  }
  if (!is_numeric($switch_opacity) || $switch_opacity > 1 || $switch_opacity <= 0) {
    form_set_error('switch_opacity', t('Grid Toggle Opacity must be a decimal value between 0 and 1.'));
  }
  if (!is_numeric($base_font_size) || $base_font_size <= 0) {
    form_set_error('base_font_size', t('Base Font Size must be a decimal value greater than zero.'));
  }
  if (!is_numeric($baseline_grid_height) || $baseline_grid_height <= 0) {
    form_set_error('baseline_grid_height', t('Baseline Grid Height must be a decimal value greater than zero.'));
  }
  if (!is_numeric($eight_col_breakpoint) || $eight_col_breakpoint <= 0) {
    form_set_error('eight_col_breakpoint', t('Eight Column Breakpoint width must be a decimal value greater than zero.'));
  }
  if (!is_numeric($sixteen_col_breakpoint) || $sixteen_col_breakpoint <= 0) {
    form_set_error('sixteen_col_breakpoint', t('Sixteen Column Breakpoint width must be a decimal value greater than zero.'));
  }
  if (!is_numeric($gutter_size) || $gutter_size <= 0) {
    form_set_error('gutter_size', t('Gutter Size must be a decimal value greater than zero.'));
  }
  if ($eight_col_breakpoint > $sixteen_col_breakpoint) {
    form_set_error('eight_col_breakpoint', t('Eight Column Breakpoint Width must be less than Sixteen Column Breakpoint Width.'));
  }
}

/**
 * Admin form submission.
 */
function golden_gridlet_admin_form_submit($form, &$form_state) {
  variable_set('golden_gridlet_guide_color', $form_state['values']['guide_color']);
  variable_set('golden_gridlet_guide_inner_color', $form_state['values']['guide_inner_color']);
  variable_set('golden_gridlet_guide_opacity', $form_state['values']['guide_opacity']);
  variable_set('golden_gridlet_switch_color', $form_state['values']['switch_color']);
  variable_set('golden_gridlet_switch_opacity', $form_state['values']['switch_opacity']);
  variable_set('golden_gridlet_base_font_size', $form_state['values']['base_font_size']);
  variable_set('golden_gridlet_baseline_grid_height', $form_state['values']['baseline_grid_height']);
  variable_set('golden_gridlet_eight_col_breakpoint', $form_state['values']['eight_col_breakpoint']);
  variable_set('golden_gridlet_sixteen_col_breakpoint', $form_state['values']['sixteen_col_breakpoint']);
  variable_set('golden_gridlet_gutter_size', $form_state['values']['gutter_size']);

  drupal_set_message(t('Your settings have been saved.'));
}
