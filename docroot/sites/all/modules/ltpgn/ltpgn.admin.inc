<?php
/**
 * @file
 * Administration page callbacks for ltpgn module
 */

/**
 * Administration form for ltpgn module
 */
function ltpgn_admin() {

  // Check that the pgn viewer exists.
  global $base_path;
  $settings = ltpgn_settings();
  $root = $_SERVER['DOCUMENT_ROOT'];
  $viewer = $root . $base_path . $settings['viewer'];
  if (!file_exists($viewer)) {
    drupal_set_message(t('@viewer not found. You must install ltpgnviewer as per the installation instructions in the README file for this module to work', array('@viewer' => $viewer)), 'error');
  }

  $frame_width = $settings['frame_width'];
  $frame_height = $settings['frame_height'];
  $max_size = $settings['max_size'];
  $directory = $settings['directory'];

  // Field for frame width.
  $form['frame_width'] = array(
    '#title' => t('Frame Width'),
    '#type' => 'textfield',
    '#description' => t('Width of the IFrame which contains the pgn viewer in the node view'),
    '#size'  => 10,
    '#default_value'  => $frame_width,
    '#required' => TRUE,
    '#weight'  => 1,
  );

  // Fields for frame height.
  $form['frame_height'] = array(
    '#title' => t('Frame Height'),
    '#type' => 'textfield',
    '#description' => t('Height of the IFrame which contains the pgn viewer in the node view'),
    '#size'  => 10,
    '#default_value'  => $frame_height,
    '#required' => TRUE,
    '#weight'  => 2,
  );

  // Field for mximum file size.
  $form['max_size'] = array(
    '#title' => t('Maximum Pgn file size (Kb)'),
    '#type' => 'textfield',
    '#description' => t('This allows the site administrator to set a file size limit to prevent very large PGN files being loaded, which may slow down the viewer. Files exceeding this limit cannot be loaded'),
    '#size'  => 10,
    '#default_value'  => $max_size,
    '#required' => TRUE,
    '#weight'  => 3,
  );

  // Location for PGN Files.
  $form['directory'] = array(
    '#title' => t('Directory to store the PGN files'),
    '#type' => 'textfield',
    '#description' => t('Sub-directory within the files directory where PGN game files will be stored.'),
    '#size'  => 10,
    '#default_value'  => $directory,
    '#required' => TRUE,
    '#weight'  => 4,
  );

  // Submit Button.
  $form['submit'] = array(
    '#value' => t('Save'),
    '#type'  => 'submit',
    '#weight'  => 5,
  );

  return $form;
}

/**
 *  validate admin settings form.
 */
function ltpgn_admin_validate($form, &$form_state) {

  // Check that the frame width and hieght are integers and greater than 50.
  // The value 50 has no particular significance, but it wouldn't make any
  // sense to have it smaller than this. These values are the size of an
  // IFrame that includes the pgn viewer so values of 200 or more would
  // be normal.
  $frame_height = intval($form_state['values']['frame_height']);
  if (!isset($frame_height)  || $frame_height < 50) {
    form_set_error('frame_height', t('Frame height must an integer greater than 50'));
  }
  $frame_width = intval($form_state['values']['frame_width']);
  if (!isset($frame_width)  || $frame_width < 50) {
    form_set_error('frame_width', t('Frame width must be greater than 50'));
  }

  // Check that the max size of the pgn file is at least 10. This again is
  // an arbitrary value, but you won't get much of a chess game in anything
  // smaller.
  $max_size = intval($form_state['values']['max_size']);
  if (!isset($max_size) || $max_size < 10) {
    form_set_error('max_size', t('Maximum file size must be greater than 10'));
  }
}

/**
 *  Action after the admin settings form is submitted,
 *  with the submit button
 */
function ltpgn_admin_submit($form, &$form_state) {

  $settings = ltpgn_settings();
  $new_settings = array();
  // Loop round the defaults incase a new variable has been added.
  $defaults = ltpgn_default_settings();
  foreach ($defaults as $name => $setting) {
    $new_settings[$name] = $form_state['values'][$name];
  }

  variable_set('ltpgn_settings', $new_settings);
  drupal_set_message(t('Ltpgn Settings Saved'));
}
