<?php

$plugin = array(
  'title' => t('★ Arbitrary Wrapper'),
  'description' => t('This style wraps content with any arbitrary element.'),
  'render region' => 'hap_arbitrary_wrapper_style_render_region',
  'render pane' => 'hap_arbitrary_wrapper_style_render_pane',
  'pane settings form' => 'hap_arbitrary_wrapper_style_settings_form',
  'settings form' => 'hap_arbitrary_wrapper_style_settings_form',
  'defaults' => array(
    'element' => 'div',
    'class' => '',
  ),
);

/**
 * Form callback.
 */
function hap_arbitrary_wrapper_style_settings_form($conf) {
  $form['element'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#title' => t('Element'),
    '#required' => TRUE,
    '#default_value' => $conf['element'],
  );

  $form['class'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#title' => t('Class'),
    '#default_value' => $conf['class'],
  );

  return $form;
}

/**
 * Common preprocessor.
 */
function hap_arbitrary_wrapper_style_preprocessor(&$vars) {
  $vars['attributes_array'] = array();
  if ($vars['settings']['class']) {
    $vars['attributes_array']['class'][] = $vars['settings']['class'];
  }
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_hap_arbitrary_wrapper_style_render_region(&$vars) {
  hap_arbitrary_wrapper_style_preprocessor($vars);
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_hap_arbitrary_wrapper_style_render_pane(&$vars) {
  hap_arbitrary_wrapper_style_preprocessor($vars);
}


/**
 * Theme callback for regions.
 */
function theme_hap_arbitrary_wrapper_style_render_region($vars) {
  $elem = $vars['settings']['element'];
  $class = $vars['settings']['class'];

  $output = '<' . $elem;

  if (!empty($class)) {
    $output .= ' class="' . $class . '">';
  }
  else {
    $output .= '>';
  }

  foreach ($vars['panes'] as $item) {
    $output .= $item;
  }

  $output .= '</' . $elem . '>';

  return $output;
}

/**
 * Theme callback for panes.
 */
function theme_hap_arbitrary_wrapper_style_render_pane($vars) {
  $elem = $vars['settings']['element'];
  $class = $vars['settings']['class'];

  $output = '<' . $elem;

  if (!empty($class)) {
    $output .= ' class="' . $class . '">';
  }
  else {
    $output .= '>';
  }

  $output .= render($vars['content']->content);
  $output .= '</' . $elem . '>';

  return $output;
}
