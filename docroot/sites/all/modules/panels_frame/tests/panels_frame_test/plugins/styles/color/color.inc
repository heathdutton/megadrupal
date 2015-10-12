<?php

$plugin = array(
  'title' => t('(PF Test) Color'),
  'description' => t('A blob of color.'),
  // 'hidden' => TRUE,
  'weight' => -100,
  'hook theme' => array(
    'panels_frame_test_style_box' => array(
      'variables' => array('content' => NULL, 'class' => ''),
    ),
  ),

  // Pane-specific callbacks.
  'render pane' => 'panels_frame_test_color_style_render_pane',
  'pane settings form' => 'panels_frame_test_color_style_pane_settings_form',
  'pane settings form validate' => 'panels_frame_test_color_style_pane_settings_validate',
  'pane settings form submit' => 'panels_frame_test_color_style_pane_settings_submit',

  // Region-specific callbacks.
  'render region' => 'panels_frame_test_color_style_render_region',
  'settings form' => 'panels_frame_test_color_style_settings_form',
  'settings validate' => 'panels_frame_test_color_style_settings_validate',
  'settings form submit' => 'panels_frame_test_color_style_settings_submit',
);


/**
 * Theme callback for panes.
 */
function theme_panels_frame_test_color_style_render_pane($variables) {
  dpm($variables);
  return theme('panels_frame_test_style_box', array('content' => $variables['content']->content, 'class' => $variables['settings']['color']));
}

/**
 * Pane settings form.
 */
function panels_frame_test_color_style_pane_settings_form($conf, $display, $pid, $type, &$form_state) {
  $colors = _panels_frame_test_colors();

  $form['color'] = array(
    '#type' => 'select',
    '#title' => t('Color'),
    '#description' => t('You may only choose a REAL color'),
    '#options' => $colors + drupal_map_assoc(array('smurple', '50 shades of grey')),
    '#default_value' => isset($conf['color']) ? $conf['color'] : '',
  );

  return $form;
}

/**
 * Pane settings form validation callback.
 */
function panels_frame_test_color_style_pane_settings_validate($form, $conf, &$form_state) {
  if (!in_array($conf['color'], _panels_frame_test_colors())) {
    form_error($form['settings']['color'], t('You must choose a REAL color!'));
  }
}

/**
 * Pane settings form submit handler.
 */
function panels_frame_test_color_style_pane_settings_submit($form, $conf, &$form_state) {
  // dpm('stuff');
}

/**
 * Do the same as above for regions.
 */
/**
 * Theme callback for regions.
 */
function theme_panels_frame_test_color_style_render_region($variables) {
  return 'REGION';
}

/**
 * Theme callback.
 */
function theme_panels_frame_test_style_box($variables) {
  return '<div class="panels-frame-test-box color-' . $variables['class'] . '">' . $variables['content'] . '</div>';
}

