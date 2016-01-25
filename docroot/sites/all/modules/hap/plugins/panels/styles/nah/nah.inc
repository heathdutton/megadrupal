<?php

$plugin = array(
  'title' => t('★ Nah..'),
  'description' => t('This style can ommit all wrapping markup or have a single wrapping div with a configurable class.'),
  'render region' => 'hap_nah_style_render_region',
  'render pane' => 'hap_nah_style_render_pane',
  'pane settings form' => 'hap_nah_style_settings_form',
  'settings form' => 'hap_nah_style_settings_form',
  'defaults' => array(
    'no_wrapper' => '',
    'class' => '',
  ),
);

/**
 * Form callback.
 */
function hap_nah_style_settings_form($conf) {
  $form['class'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#title' => t('Class'),
    '#default_value' => $conf['class'],
    '#states' => array(
      'invisible' => array(
        'input[name="settings[no_wrapper]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['no_wrapper'] = array(
    '#type' => 'checkbox',
    '#title' => t('Seriously bra, I don\'t want ANY wrapping markup..'),
    '#default_value' => $conf['no_wrapper']
  );

  return $form;
}

/**
 * Common preprocessor.
 */
function hap_nah_style_preprocessor(&$vars) {
  $vars['attributes_array'] = array();
  if ($vars['settings']['class']) {
    $vars['attributes_array']['class'][] = $vars['settings']['class'];
  }
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_hap_nah_style_render_region(&$vars) {
  hap_nah_style_preprocessor($vars);
}

/**
 * Implements template_preprocess_HOOK().
 */
function template_preprocess_hap_nah_style_render_pane(&$vars) {
  hap_nah_style_preprocessor($vars);
}


/**
 * Theme callback for regions.
 */
function theme_hap_nah_style_render_region($vars) {
  $output = '';
  foreach ($vars['panes'] as $item) {
    $output .= $item;
  }

  if ($vars['settings']['no_wrapper']) {
    $attributes = drupal_attributes($vars['attributes_array']);
    $output = '<div ' . $attributes . '>' . $output . '</div>';
  }

  return $output;
}

/**
 * Theme callback for panes.
 */
function theme_hap_nah_style_render_pane($vars) {
  if ($vars['settings']['no_wrapper']) {
    $output = render($vars['content']->content);
  }
  else {
    $output  = '<div' . drupal_attributes($vars['attributes_array']) .'>';
    $output .= render($vars['content']->content);
    $output .= '</div>';
  }

  return $output;
}
