<?php

/**
 * @file
 * Whiteboard module forms
 */

/**
 * Implements hook_form().
 */
function whiteboard_forms($form_id, $args) {
  $forms['whiteboard_form'] = array(
    'callback' => 'whiteboard_form',
  );
  $forms['whiteboard_board_form'] = array(
    'callback' => 'whiteboard_board_form',
  );
  $forms['whiteboard_top_form'] = array(
    'callback' => 'whiteboard_top_form',
  );
  $forms['whiteboard_color_form'] = array(
    'callback' => 'whiteboard_color_form',
  );
  $forms['whiteboard_submit_form'] = array(
    'callback' => 'whiteboard_submit_form',
  );
  $forms['whiteboard_meta_form'] = array(
    'callback' => 'whiteboard_meta_form',
  );
  $forms['whiteboard_status_form'] = array(
    'callback' => 'whiteboard_status_form',
  );
  $forms['whiteboard_side_form'] = array(
    'callback' => 'whiteboard_side_form',
  );
  $forms['whiteboard_bottom_form'] = array(
    'callback' => 'whiteboard_side_form',
  );
  $forms['whiteboard_ellipse_form'] = array(
    'callback' => 'whiteboard_ellipse_form',
  );
  $forms['whiteboard_rectangle_form'] = array(
    'callback' => 'whiteboard_rectangle_form',
  );
  $forms['whiteboard_circle_form'] = array(
    'callback' => 'whiteboard_circle_form',
  );
  $forms['whiteboard_save_form'] = array(
    'callback' => 'whiteboard_save_form',
  );
  return $forms;
}

/**
 * Implements hook_form() to create buttons and coordinates to be placed on top of the whiteboard().
 *
 */
function whiteboard_top_form($node, $form_state) {
  $form['mousieX'] = array(
    '#type' => 'textfield',
    '#description' => t('X'),
    '#default_value' => '0',
    '#id' => 'mousieX',
    '#maxlength' => 3,
  );
  $form['mousieY'] = array(
    '#type' => 'textfield',
    '#description' => t('Y'),
    '#default_value' => '0',
    '#id' => 'mousieY',
    '#maxlength' => 3,
  );
  $form['whiteboard-clear-points'] = array(
    '#type' => 'button',
    '#value' => t('Clear Points'),
  );

  $form['whiteboard-clear-board'] = array(
    '#type' => 'button',
    '#value' => t('Clear Board'),
  );
  $form['whiteboard-penwidth-fine'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/fine.png',
  );
  $form['whiteboard-penwidth-thick'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/thick.png',
  );
  $form['whiteboard-penwidth-blunt'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/blunt.png',
  );
  $form['whiteboard-black'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/black.png',
  );
  $form['whiteboard-blue'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/blue.png',
  );
  $form['whiteboard-green'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/green.png',
  );
  $form['whiteboard-red'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/red.png',
  );
  $form['whiteboard-hex'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/color.png',
  );
  $form['whiteboard-eraser'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/eraser.png',
  );
  return $form;
}

/**
 * Implements hook_form() to create grid buttons().
 *
 */
function whiteboard_side_form($node, $form_state) {

  $form['whiteboard-cartecian'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/cartecian.png',
  );
  $form['whiteboard-default'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/default.png',
  );
  $form['whiteboard-show-grid'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/grid.png',
  );
  $form['whiteboard-hide-grid'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/hide-grid.png',
  );
  $form['whiteboard-show-range'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/show-range.png',
  );
  $form['whiteboard-hide-range'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/hide-range.png',
  );


  return $form;
}

/**
 * Implements hook_form() to create bottom row of drawing buttons().
 *
 */
function whiteboard_bottom_form($node, $form_state) {
  $form['whiteboard-line'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/line.png',
  );
  $form['whiteboard-polyline'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/polyline.png',
  );
  $form['whiteboard-polygon'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/polygon.png',
  );
  $form['whiteboard-arc'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/arc.png',
  );
  $form['whiteboard-curve'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/curve.png',
  );
  $form['whiteboard-closed-curve'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/closed-curve.png',
  );
  $form['whiteboard-bezier'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/bezier.png',
  );
  $form['whiteboard-circle'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/circle.png',
  );
  $form['whiteboard-ellipse'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/ellipse.png',
  );
  $form['whiteboard-rectangle'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'whiteboard') . '/img/rectangle.png',
  );
  return $form;
}

/**
 * Implements hook_form() for arc form().
 */
function whiteboard_arc_form($node, $form_state) {
  $form['whiteboard-arc-width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => '200',
    '#maxlength' => 4,
  );
  $form['whiteboard-arc-height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => '100',
    '#maxlength' => 4,
  );
  $form['whiteboard-arc-angle'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Angle'),
    '#default_value' => '0',
    '#maxlength' => 4,
  );
  $form['whiteboard-arc-length'] = array(
    '#type' => 'textfield',
    '#title' => t('Arc length'),
    '#default_value' => '200',
    '#maxlength' => 4,
  );
  $form['whiteboard-arc-cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );
  $form['whiteboard-arc-submit'] = array(
    '#type' => 'button',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Implements hook_form() for rectangle().
 */
function whiteboard_rectangle_form($node, $form_state) {
  $form['whiteboard-rectangle-width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => '200',
    '#maxlength' => 4,
  );
  $form['whiteboard-rectangle-height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => '100',
    '#maxlength' => 4,
  );
  $form['whiteboard-rectangle-cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );
  $form['whiteboard-rectangle-submit'] = array(
    '#type' => 'button',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Implements hook_form() for ellipse form().
 */
function whiteboard_ellipse_form($node, $form_state) {
  $form['whiteboard-ellipse-width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => '200',
    '#maxlength' => 4,
  );
  $form['whiteboard-ellipse-height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => '100',
    '#maxlength' => 4,
  );
  $form['whiteboard-ellipse-cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );
  $form['whiteboard-ellipse-submit'] = array(
    '#type' => 'button',
    '#value' => t('Submit'),
  );
  return $form;
}


/**
 * Implements hook_form() for circle().
 */
function whiteboard_circle_form($node, $form_state) {
  $form['whiteboard-circle-radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => '50',
    '#maxlength' => 4,
  );
  $form['whiteboard-circle-cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );
  $form['whiteboard-circle-submit'] = array(
    '#type' => 'button',
    '#value' => t('Submit'),
  );
  return $form;
}


/**
 * Implements hook_form().
 */
function whiteboard_color_form($node, $form_state) {
  $form['whiteboard-color-code'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML Color Code'),
    '#default_value' => '#777777',
    '#maxlength' => 7,
  );
  $form['whiteboard-color-cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );
  $form['whiteboard-color-submit'] = array(
    '#type' => 'button',
    '#value' => t('Choose HTML color'),
  );
  return $form;
}

/**
 * Implements hook_form().
 */
function whiteboard_save_form($node, $form_state) {
  $form['whiteboard_submit'] = array(
    '#type' => 'button',
    '#value' => t('Save Whiteboard'),
  );
  return $form;
}

/**
 * Implements hook_form() to create input's related to color, tension, and penwidth().
 */
function whiteboard_meta_form() {
  $form['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#default_value' => '#99BBFF',
    '#required' => TRUE,
    '#description' => t('Set the HTML hexadecimal color'),
    '#size' => 7,
    '#id' => 'color',
  );

  $form['penwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Pen width'),
    '#default_value' => '3',
    '#description' => t('Set the thickness of marker'),
    '#required' => TRUE,
    '#size' => 3,
    '#id' => 'penwidth',
  );
  $form['tension'] = array(
    '#type' => 'textfield',
    '#title' => t('Tension'),
    '#default_value' => '0',
    '#description' => t('Set tension for curves'),
    '#required' => TRUE,
    '#size' => 5,
    '#id' => 'tension',
  );

  return $form;
}

/**
 * Implements hook_form() to be used on the admininstration page().
 */
function whiteboard_form($node, &$form_state) {
  global $user;
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => check_plain($node->title),
    '#required' => TRUE,
  );

  $body_element = array(
    '#type' => 'textarea',
    '#base_type' => 'textarea',
    '#title' => t('Body'),
    '#parents' => array('body', 'body_filter'),
    '#default_value' => '',
    '#rows' => 3,
    '#description' => t('Notes for the whiteboard'),
  );
  $body_element = filter_process_format($body_element);
  $form['body_filter']['format'] = $body_element['format'];

  if(isset($node->nid))
  {
    $result = db_select('field_data_body','db')
               ->fields('db')
               ->condition('entity_id', $node->nid, '=') 
               ->execute()
               ->fetchAssoc();
    $form['body_filter']['format']['format']['#default_value'] = $result['body_format'];
  }
  return $form;
}

