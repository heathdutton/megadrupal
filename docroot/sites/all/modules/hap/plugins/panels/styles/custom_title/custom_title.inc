<?php

$plugin = array(
  'title' => t('★ Custom Header'),
  'description' => t('Choose a heading level for your region or pane title.'),
  'render region' => 'hap_custom_header_style_render_region',
  'render pane' => 'hap_custom_header_style_render_pane',
  'pane settings form' => 'hap_custom_header_style_settings_form',
  'settings form' => 'hap_custom_header_region_style_settings_form',
  'defaults' => array(
    'heading' => 'h2',
    'class' => '',
    'title' => '',
  ),
);

/**
 * Settings form.
 */
function hap_custom_header_style_settings_form($conf) {
  $form['heading'] = array(
    '#type' => 'select',
    '#title' => t('Choose heading'),
    '#description' => t('Select the desired header-level for the title.'),
    '#options' => drupal_map_assoc(array('h1', 'h2', 'h3', 'h4', 'h5', 'h6')),
    '#default_value' => $conf['heading'],
  );

  $form['class'] = array(
    '#type' => 'textfield',
    '#title' => t('Class'),
    '#description' => t('Optionally add a class to the heading element'),
    '#default_value' => $conf['class'],
  );

  return $form;
}

/**
 * Region settings form.
 */
function hap_custom_header_region_style_settings_form($conf) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom Title'),
    '#description' => t('Enter the custom title for your region.'),
    '#required' => TRUE,
    '#default_value' => $conf['title'],
    );

  // Combine with common settings form.
  $form += hap_custom_header_style_settings_form($conf);

  return $form;
}

/**
 * Pane render callback.
 */
function theme_hap_custom_header_style_render_pane($vars) {
  $settings = $vars['settings'];
  $content = $vars['content'];
  $output = '';
  $attributes = '';

  if (!empty($settings['class'])) {
    $attributes = ' class = "' . $settings['class'] . '"';
  }

  if (isset($content->title) && !empty($content->title)) {
    $output .= '<' . $settings['heading'] . $attributes . '>' . $content->title . '</' . $settings['heading'] . '>';
  }

  $output .= render($content->content);

  return $output;
}

/**
 * Region render callback.
 */
function theme_hap_custom_header_style_render_region($vars) {
  $settings = $vars['settings'];
  $output = '';
  $attributes = '';

  if (!empty($settings['class'])) {
    $attributes = ' class = "' . $settings['class'] . '"';
  }

  if (!empty($settings['title'])) {
    $output .= '<' . $settings['heading'] . $attributes . '>' . $settings['title'] . '</' . $settings['heading'] . '>';
  }

  foreach ($vars['panes'] as $pane) {
    $output .= $pane;
  }

  return $output;
}
