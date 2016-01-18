<?php
/**
 * @file
 * Definition of the Versatile tag panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Versatile Tag'),
  'description' => t('Wrap the content of a pane in a HTML element.'),
  'render pane' => 'versatile_tag_style_render_pane',
  'render region' => 'versatile_tag_style_render_region',
  'pane settings form' => 'versatile_tag_style_pane_settings_form',
  'settings form' => 'versatile_tag_style_settings_form',
);

/**
 * Render callback.
 */
function theme_versatile_tag_style_render_pane($vars) {
  $content = $vars['content'];
  $settings = $vars['settings'];
  $output = '';

  if (empty($settings['content_tag'])) {
    $settings['content_tag'] = 'div';
  }
  
  if (empty($settings['content_id'])) {
    $settings['content_id'] = '';
  }

  if (empty($settings['content_class'])) {
    $settings['content_class'] = '';
  }

  if (empty($settings['title_tag'])) {
    $settings['title_tag'] = 'h3';
  }

  if (empty($settings['title_class'])) {
    $settings['title_class'] = '';
  }

  $output = '<' . $settings['content_tag'];
  $output .= !empty($settings['content_id']) ? ' id="' . $settings['content_id'] . '"' : '';
  $output .= !empty($settings['content_class']) ? ' class="' . $settings['content_class'] . '">' : '>';

  if (!empty($content->title)) {
    $output .= '<' . $settings['title_tag'];
    $output .= !empty($settings['title_class']) ? ' class="' . $settings['title_class'] . '">' : '>';
    $output .= $content->title . '</' . $settings['title_tag'] . '>';
  }

  $output .= render($content->content) . '</' . $settings['content_tag'] . '>';
  
  return $output;
}

/**
 * Render callback.
 */
function theme_versatile_tag_style_render_region($vars) {
  $settings = $vars['settings'];
  $panes = $vars['panes'];
  
  if (empty($settings['content_tag'])) {
    $settings['content_tag'] = 'div';
  }
  
  if (empty($settings['content_id'])) {
    $settings['content_id'] = '';
  }

  if (empty($settings['content_class'])) {
    $settings['content_class'] = '';
  }

  $output = '<' . $settings['content_tag'];
  $output .= !empty($settings['content_id']) ? ' id="' . $settings['content_id'] . '"' : '';
  $output .= !empty($settings['content_class']) ? ' class="' . $settings['content_class'] . '">' : '>';
  $output .= implode("\n", $panes) . '</' . $settings['content_tag'] . '>';

  return $output;
}

/**
 * Settings form callback.
 */
function versatile_tag_style_settings_form($style_settings) {
  $form['content_tag'] = array(
    '#type' => 'select',
    '#title' => t('HTML element on content'),
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
      'section' => 'SECTION',
      'article' => 'ARTICLE',
      'aside' => 'ASIDE',
      'figure' => 'FIGURE',
      'header' => 'HEADER',
      'footer' => 'FOOTER',
      'nav' => 'NAV',
    ),
    '#default_value' => (isset($style_settings['content_tag'])) ? $style_settings['content_tag'] : 'div',
  );
  $form['content_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID on content'),
    '#default_value' => (isset($style_settings['content_id'])) ? $style_settings['content_id'] : '',
  );
  $form['content_class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class on content'),
    '#default_value' => (isset($style_settings['content_class'])) ? $style_settings['content_class'] : '',
  );

  return $form;
}

/**
 * Settings form callback.
 */
function versatile_tag_style_pane_settings_form($style_settings) {
  $form['content_tag'] = array(
    '#type' => 'select',
    '#title' => t('Wrapper'),
    '#description' => t('HTML element to use for wrapper.'),
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
      'section' => 'SECTION',
      'article' => 'ARTICLE',
      'aside' => 'ASIDE',
      'figure' => 'FIGURE',
      'header' => 'HEADER',
      'footer' => 'FOOTER',
      'nav' => 'NAV',
    ),
    '#default_value' => (isset($style_settings['content_tag'])) ? $style_settings['content_tag'] : 'div',
  );
  $form['content_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID on wrapper'),
    '#description' => t('ID to add to the wrapping tag.'),
    '#default_value' => (isset($style_settings['content_id'])) ? $style_settings['content_id'] : '',
  );
  $form['content_class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class on wrapper'),
    '#description' => t('CSS class to add to the wrapping tag.'),
    '#default_value' => (isset($style_settings['content_class'])) ? $style_settings['content_class'] : '',
  );

  $form['title_tag'] = array(
    '#type' => 'select',
    '#title' => t('Title'),
    '#description' => t('HTML element to use for title.'),
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
    '#default_value' => (isset($style_settings['title_tag'])) ? $style_settings['title_tag'] : 'div',
  );
  $form['title_class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class'),
    '#description' => t('CSS class to add to the title tag.'),
    '#default_value' => (isset($style_settings['title_class'])) ? $style_settings['title_class'] : '',
  );

  return $form;
}