<?php

/**
 * @file
 * Theme functions to support post module.
 */

/**
 * Preprocess function for post_button.
 *
 * @see post_theme()
 */
function template_preprocess_post_button(&$vars) {
  drupal_add_css($vars['directory'] . '/post.css');
  $settings = $vars['settings'];
  $vars['style'] = '';

  // Set initial classes.
  $vars['classes_container'][] = 'post-button';
  $vars['classes_array'] = array('pw_widget');
  $vars['classes_array'][] = $settings['copy_paste_tracking'] == 1 ? 'pw_copypaste_true' : 'pw_copypaste_false';

  // The pw_counter class will interfere with the vertical style.
  if (!isset($settings['vertical'])) {
    $vars['classes_array'][] = $settings['post_counter'] == 1 ? 'pw_counter_true' : 'pw_counter_false';
  }

  // Set icon size class.
  if (isset($settings['post']) || isset($settings['buttons'])) {
    $icon_size = isset($settings['post']['post_icon_size']) ? $settings['post']['post_icon_size'] : $settings['buttons']['buttons_icon_size'];
    if (is_numeric($icon_size)) {
      $vars['classes_array'][] = 'pw_size_' . $icon_size;
    }
  }

  // Po.st share button.
  if (isset($settings['post'])) {
    $vars['services'][] = 'pw_share';
  }
  // Expanded buttons with counters.
  elseif (isset($settings['expanded'])) {
    $vars['classes_array'][] = 'pw_counter_buttons';
    $vars['classes_container'][] = 'post-expanded';
  }
  // Vertical buttons with counters and floating sharebar.
  elseif (isset($settings['vertical'])) {
    drupal_add_js($vars['directory'] . '/post.js');

    $vars['classes_array'][] = 'pw_counter_buttons';
    $vars['classes_array'][] = 'pw_vertical';
    $vars['classes_container'][] = 'post-vertical';

    // Position and color settings for initial setup.
    $pos = check_plain($settings['vertical']['vertical_position']);
    $bg_color = check_plain($settings['vertical']['vertical_color']);
    $border_color = check_plain($settings['vertical']['vertical_border']);
    $vars['style'] = 'background-color:#' . $bg_color . ';border:1px solid #' . $border_color . ';';

    // Position setting is used for dynamic calculations in post.js.
    drupal_add_js(array('post_position' => $pos), 'setting');
  }

  // Only output additional services if Po.st style is not selected.
  if (!isset($settings['post'])) {
    $vars['services'] = $settings['enabled_services'];
    if (isset($settings['expanded']) || isset($settings['vertical'])) {
      foreach ($vars['services'] as &$service) {
        $service .= '_button';
      }
    }
  }
}
