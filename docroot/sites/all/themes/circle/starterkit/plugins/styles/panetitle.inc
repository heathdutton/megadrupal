<?php
/**
 * @file
 * Add custom class to pane title and pane content.
 */

// Plugin definition.
$plugin = array(
  'title' => '--> ' . t('Circle: Pane classes'),
  'description' => t('Add custom class to pane content and pane title'),
  'pane settings form' => 'circle_pane_title_class_settings_form',
  'render pane' => 'circle_pane_title_class',
);

/**
 * Render callback for a single pane.
 */
function theme_circle_pane_title_class($vars) {
  $content = $vars['content'];
  $pane = $vars['pane'];
  $display = $vars['display'];
  $title_class = isset($vars['settings']['title_class']) ? $vars['settings']['title_class'] : '';
  $content_class = isset($vars['settings']['content_class']) ? $vars['settings']['content_class'] : '';

  if (empty($content->content)) {
    return;
  }

  $output = theme('panels_pane', array(
    'content' => $content,
    'pane' => $pane,
    'display' => $display,
    'title_class' => $title_class,
    'content_class' => $content_class,
  ));
  return $output;
}

/**
 * Panetitle plugin settings.
 */
function circle_pane_title_class_settings_form($style_settings, $display, $pid, $type, $form_state) {
  $form['title_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Title class'),
    '#default_value' => (isset($style_settings['title_class'])) ? $style_settings['title_class'] : '',
    '#description' => t('Add an additional class to pane title'),
  );
  $form['content_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Content class'),
    '#default_value' => (isset($style_settings['content_class'])) ? $style_settings['content_class'] : '',
    '#description' => t('Add an additional class to pane content wrapper'),
  );
  return $form;
}
