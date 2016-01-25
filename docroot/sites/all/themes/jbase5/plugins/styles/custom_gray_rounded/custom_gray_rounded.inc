<?php

/**
 * @file
 * Definition of the 'rounded_corners' panel style.
 */
// Plugin definition
$plugin = array(
  'title' => t('Custom - Gray background with rounded corners'),
  'description' => t('Presents the panes or panels with a gray background and rounded corners'),
  'render region' => 'custom_theme_gray_rounded_style_render_region',
  'render pane' => 'custom_theme_gray_rounded_style_render_pane',
  'settings form' => 'custom_theme_gray_rounded_style_settings_form',
  'hook theme' => array(
    'custom_theme_gray_rounded' => array(
      'variables' => array('content' => NULL),
      'path' => panels_get_path('plugins/styles/custom_theme_gray_rounded'),
      'template' => 'custom-theme-gray-rounded',
    ),
  ),
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_custom_theme_gray_rounded_style_render_region($vars) {
  $display = $vars['display'];
  $region_id = $vars['region_id'];
  $panes = $vars['panes'];
  $settings = $vars['settings'];
  $output = '';
  // Determine where to put the box. If empty or 'pane' around each pane. If
  // 'panel' then just around the whole panel.
  $where = empty($settings['corner_location']) ? 'pane' : $settings['corner_location'];
  $print_separator = FALSE;
  foreach ($panes as $pane_id => $pane) {
    // Add the separator if we've already displayed a pane.
    if ($print_separator) {
      $output .= '<div class="panel-separator">&nbsp;</div>';
    }
    if ($where == 'pane') {
      $output .= theme('custom_theme_gray_rounded', array('content' => $pane));
    }
    else {
      $output .= $pane;
      $print_separator = TRUE;
    }
  }
  if ($where == 'panel') {
    $output = theme('custom_theme_gray_rounded', array('content' => $output));
  }
  return $output;
}

/**
 * Render callback for a single pane.
 */
function theme_custom_theme_gray_rounded_style_render_pane($vars) {
  $content = $vars['content'];
  $pane = $vars['pane'];
  $display = $vars['display'];
  if (empty($content->content)) {
    return;
  }
  $output = theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));
  // Just stick a box around the standard theme_panels_pane.
  $output = theme('custom_theme_gray_rounded', array('content' => $output));
  return $output;
}

/**
 * Settings form callback.
 */
function custom_theme_gray_rounded_style_settings_form($style_settings) {
  $form['corner_location'] = array(
    '#type' => 'select',
    '#title' => t('Box around'),
    '#options' => array(
      'pane' => t('Each pane'),
      'panel' => t('Each panel'),
    ),
    '#default_value' => (isset($style_settings['corner_location'])) ? $style_settings['corner_location'] : 'ul',
    '#description' => t('Choose whether to include the box around each pane (piece of content) or region (each column or region)'),
  );
  return $form;
}