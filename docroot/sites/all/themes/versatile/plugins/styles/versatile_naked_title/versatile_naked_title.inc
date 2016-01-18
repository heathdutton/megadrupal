<?php

/**
 * @file
 * Definition of the Versatile Naked with title panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Versatile Naked with title'),
  'description' => t('Display the pane with no markup, except the title.'),
  'render pane' => 'versatile_naked_title_style_render_pane',
  'pane settings form' => 'versatile_naked_title_style_settings_form',
);

/**
 * Render callback.
 */
function theme_versatile_naked_title_style_render_pane($vars) {
  $content = $vars['content'];
  $settings = $vars['settings'];
  $output = '';

  if (!empty($content->title)) {
    if (empty($settings['tag'])) {
      $settings['tag'] = 'h1';
    }

    if (empty($settings['class'])) {
      $settings['class'] = '';
    }

    $output .= '<' . $settings['tag'];
    $output .= !empty($settings['class']) ? ' class="' . $settings['class'] . '">' : '>';
    $output .= $content->title . '</' . $settings['tag'] . '>';
  }
  
  $output .= render($vars['content']->content);

  return $output;
}

/**
 * Settings form callback.
 */
function versatile_naked_title_style_settings_form($style_settings) {
  $form['tag'] = array(
    '#type' => 'select',
    '#title' => t('HTML element'),
    '#description' => t('Element to wrap the title with.'),
    '#options' => array(
      'div' => 'DIV',
      'span' => 'SPAN',
      'h1' => 'H1',
      'h2' => 'H2',
      'h3' => 'H3',
      'h4' => 'H4',
      'h5' => 'H5',
      'h6' => 'H6',
      'p' => 'P',
      'pre' => 'PRE',
      'address' => 'ADDRESS',
    ),
    '#default_value' => (isset($style_settings['tag'])) ? $style_settings['tag'] : 'div',
  );
  $form['class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class'),
    '#default_value' => (isset($style_settings['class'])) ? $style_settings['class'] : '',
  );

  return $form;
}