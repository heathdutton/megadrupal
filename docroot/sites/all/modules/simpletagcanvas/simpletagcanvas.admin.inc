<?php

/**
 * @file
 * SimpleTagCanvas backend behavior configuration
 *
 * This file contains the functions and settings for the
 * parameters of the TagCavas plugin.
 */

define('SIMPLETAGCANVAS_DEFAULT_MOUSEWHEELZOOM', 'false');
define('SIMPLETAGCANVAS_DEFAULT_SHAPE', 'sphere');
define('SIMPLETAGCANVAS_DEFAULT_LINKSELECTION', 'true');
define('SIMPLETAGCANVAS_DEFAULT_WIDTH', '300');
define('SIMPLETAGCANVAS_DEFAULT_HEIGHT', '300');
define('SIMPLETAGCANVAS_DEFAULT_LOCK', 'null');
define('SIMPLETAGCANVAS_DEFAULT_MINSPEED', '0.05');
define('SIMPLETAGCANVAS_DEFAULT_MAXSPEED', '0.05');

/**
 * Implements hook_form().
 */
function simpletagcanvas_admin_form($form, &$form_state) {

  $form['description'] = array(
    '#markup' => '<div>' . t('Select the relevant shape of your simple TagCanvas here.') . '</div>',
  );

  $form['simpletagcanvas_appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simple TagCanvas Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['simpletagcanvas_appearance']['simpletagcanvas_shape'] = array(
    '#type' => 'select',
    '#title' => t('Shape'),
    '#options' => array(
      'sphere' => t('The sphere shape'),
      'vcylinder' => t('The vertical cylinder'),
      'hcylinder' => t('The horizontal cylinder'),
      'hring' => t('The horizontal ring'),
      'vring' => t('The vertical ring'),
    ),
    '#default_value' => variable_get('simpletagcanvas_shape', SIMPLETAGCANVAS_DEFAULT_SHAPE),
    '#description' => t('Available TagCanvas Shapes'),
  );

  $form['simpletagcanvas_appearance']['simpletagcanvas_width'] = array(
    '#type' => 'textfield',
    '#title' => t('SimpleTagCanvas Width'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Define canvas width in px.'),
    '#default_value' => variable_get('simpletagcanvas_width', SIMPLETAGCANVAS_DEFAULT_WIDTH),
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  $form['simpletagcanvas_appearance']['simpletagcanvas_height'] = array(
    '#type' => 'textfield',
    '#title' => t('SimpleTagCanvas Height'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Define canvas height.'),
    '#default_value' => variable_get('simpletagcanvas_height', SIMPLETAGCANVAS_DEFAULT_HEIGHT),
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  $form['simpletagcanvas_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simple TagCanvas Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['simpletagcanvas_options']['simpletagcanvas_mousewheelzoom'] = array(
    '#type' => 'select',
    '#title' => t('Mouse Wheel Zoom'),
    '#options' => array(
      '1' => 'True',
      '0' => 'False'),
    '#default_value' => variable_get('simpletagcanvas_mousewheelzoom', SIMPLETAGCANVAS_DEFAULT_MOUSEWHEELZOOM),
    '#description' => t('Enables zooming the cloud in and out using the mouse wheel or scroll gesture.'),
  );

  $form['simpletagcanvas_options']['simpletagcanvas_lock'] = array(
    '#type' => 'select',
    '#title' => t('Lock'),
    '#options' => array(
      'null' => 'null',
      'x' => 'x',
      'y' => 'y',
      'xy' => 'xy',
    ),
    '#default_value' => variable_get('simpletagcanvas_lock', SIMPLETAGCANVAS_DEFAULT_LOCK),
    '#description' => t('Limits rotation of the cloud using the mouse.'),
  );

  $form['simpletagcanvas_options']['simpletagcanvas_minspeed'] = array(
    '#type' => 'textfield',
    '#title' => t('minSpeed'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => variable_get('simpletagcanvas_minspeed', SIMPLETAGCANVAS_DEFAULT_MINSPEED),
    '#description' => t('Minimum speed of rotation when mouse leaves canvas.'),
  );

  $form['simpletagcanvas_options']['simpletagcanvas_maxspeed'] = array(
    '#type' => 'textfield',
    '#title' => t('maxSpeed'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => variable_get('simpletagcanvas_maxspeed', SIMPLETAGCANVAS_DEFAULT_MAXSPEED),
    '#description' => t('Maximum speed of rotation.'),
  );

  $form['#validate'][] = 'simpletagcanvas_admin_form_validate';
  return system_settings_form($form);

}

/**
 * Implements hook_form_validate().
 */
function simpletagcanvas_admin_form_validate($form, &$form_state) {
  $string_minspeed = $form_state['values']['simpletagcanvas_minspeed'];
  if (simpletagcanvas_validate_float($string_minspeed, 0, 1)) {
    form_set_error('simpletagcanvas_minspeed', t('minSpeed invalid characters. Only float are allowed! (max: 1.0 | min: 0)'));
  }
  $string_maxspeed = $form_state['values']['simpletagcanvas_maxspeed'];
  if (simpletagcanvas_validate_float($string_maxspeed, 0, 1)) {
    form_set_error('simpletagcanvas_maxspeed', t('maxSpeed invalid characters. Only float are allowed! (max: 1.0 | min: 0)'));
  }
}
