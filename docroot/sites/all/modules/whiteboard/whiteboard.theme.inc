<?php

/**
 * @file
 * Whiteboard theme functions
 *
 */

/**
 * Implements hook_theme().
 */
/**
 * Implementation of hook_theme().
 */
function whiteboard_theme($existing, $type, $theme, $path) {
  return array(
    'whiteboard_form' => array(
      'arguments' => array(),
    ),
    'whiteboard_bottom' => array(
      'arguments' => array(),
    ),
    'whiteboard_side' => array(
      'arguments' => array(),
    ),
    'whiteboard_top' => array(
      'arguments' => array(),
    ),
    'whiteboard_status' => array(
      'arguments' => array(),
    ),
    'whiteboard_color' => array(
      'arguments' => array(),
    ),
    'whiteboard_meta' => array(
      'arguments' => array(),
    ),
    'whiteboard_save' => array(
      'arguments' => array(),
    ),
    'whiteboard_board' => array(
      'arguments' => array(),
    ),
    'whiteboard_ellipse' => array(
      'arguments' => array(),
    ),
    'whiteboard_arc' => array(
      'arguments' => array(),
    ),
    'whiteboard_rectangle' => array(
      'arguments' => array(),
    ),
    'whiteboard_circle' => array(
      'arguements' => array(),
    ),
  );
}

/**
 * Theme a whiteboard.
 *
 * @ingroup themeable
 */
function theme_whiteboard_board($variables) {
  $output = '<div id="whiteboard-container">';
  $output .= theme('whiteboard_meta');
  $output .= theme('whiteboard_top');
  $output .= '<div id="whiteboard"></div>';
  $output .= theme('whiteboard_side');
  $output .= '<div id="whiteboard-ellipse-container">' . theme('whiteboard_ellipse') . '</div>';
  $output .= '<div id="whiteboard-arc-container">' . theme('whiteboard_arc') . '</div>';
  $output .= '<div id="whiteboard-rectangle-container">' . theme('whiteboard_rectangle') . '</div>';
  $output .= '<div id="whiteboard-circle-container">' . theme('whiteboard_circle') . '</div>';
  $output .= '<div id="whiteboard-color-container">' . theme('whiteboard_color') . '</div>';
  $output .= '<div id="whiteboard-bottom-container">' . theme('whiteboard_bottom') . '</div>';
  $output .= '<div id="whiteboard-status-container"><span id="whiteboard-status"></span></div>';
  $output .= theme('whiteboard_save');
  $output .= '</div>'; 
  return $output;
}

/**
 * Theme whiteboard meta inputs
 *
 * @ingroup themeable
 */
function theme_whiteboard_meta() {
  $output = drupal_get_form('whiteboard_meta_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-meta">' . $output . '</div>';
}

/**
 * Theme whiteboard side inputs
 *
 * @ingroup themeable
 */
function theme_whiteboard_side() {
  $output = drupal_get_form('whiteboard_side_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-side">' . $output . '</div>';
}


/**
 * Theme whiteboard top row
 *
 * @ingroup themeable
 */
function theme_whiteboard_top() {
  $output = drupal_get_form('whiteboard_top_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-top">' . $output . '</div>';
}

/**
 * Theme whiteboard color  buttons.
 *
 * @ingroup themeable
 */
function theme_whiteboard_color() {
  $output = drupal_get_form('whiteboard_color_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-color">' . $output . '</div>';
}

/**
 * Theme chat buttons.
 *
 * @ingroup themeable
 */
function theme_whiteboard_save() {
  $output = drupal_get_form('whiteboard_save_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-submit-button">' . $output . '</div>';
}

/**
 * Theme whiteboard ellipse inputs
 *
 * @ingroup themeable
 */
function theme_whiteboard_ellipse() {
  $output = drupal_get_form('whiteboard_ellipse_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-ellipse">' . $output . '</div>';
}

/**
 * Theme whiteboard rectangle inputs
 *
 * @ingroup themeable
 */
function theme_whiteboard_rectangle() {
  $output = drupal_get_form('whiteboard_rectangle_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-rectangle">' . $output . '</div>';
}

/**
 * Theme whiteboard circle inputs
 *
 * @ingroup themeable
 */
function theme_whiteboard_circle() {
  $output = drupal_get_form('whiteboard_circle_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-circle">' . $output . '</div>';
}

/**
 * Theme whiteboard arc inputs
 *
 * @ingroup themeable
 */
function theme_whiteboard_arc() {
  $output = drupal_get_form('whiteboard_arc_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-arc">' . $output . '</div>';
}

/**
 * Theme whiteboard bottom
 *
 * @ingroup themeable
 */
function theme_whiteboard_bottom() {
  $output = drupal_get_form('whiteboard_bottom_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-bottom">' . $output . '</div>';
}

/**
 * Theme whiteboard bottom
 *
 * @ingroup themeable
 */
function theme_whiteboard() {
  $output = drupal_get_form('whiteboard_form');
  $output = drupal_render($output);
  return '<div id="whiteboard-body">' . $output . '</div>';
}

