<?php

/**
 * @file
 * Colorize your theme Admin settings form.
 */

/**
 * Admin settings form for radix_colorizer.
 */
function radix_colorizer_admin_settings($form, &$form_state, $theme) {
  module_load_include('admin.inc', 'colorizer');
  $form = colorizer_admin_form(FALSE, TRUE);
  $base = drupal_get_path('module', 'colorizer');
  $radix_base = drupal_get_path('module', 'radix_colorizer');

  // Some changes based on current theme.
  $theme_path = drupal_get_path('theme', $theme);
  $instance = $theme;
  $css_file = $theme_path . '/colorizer/.colorizer.css';
  $inc_file = $theme_path . '/colorizer/.colorizer.inc';

  // Most of this copied from Color module.
  $info = radix_colorizer_get_info($theme, TRUE);
  if (!empty($info['schemes'])) {
    $info['schemes'][''] = array('title' => t('Custom'), 'colors' => array());

    $color_sets = array();
    $schemes = array();
    foreach ($info['schemes'] as $key => $scheme) {
      $color_sets[$key] = $scheme['title'];
      $schemes[$key] = $scheme['colors'];
      $schemes[$key] += $info['schemes']['default']['colors'];
    }

    // See if we're using a predefined scheme.
    // Note: we use the original theme when the default scheme is chosen.
    $current_scheme = variable_get('colorizer_' . $instance . '_palette', array());
    foreach ($schemes as $key => $scheme) {
      if ($current_scheme == $scheme) {
        $scheme_name = $key;
        break;
      }
    }
    if (empty($scheme_name)) {
      if (empty($current_scheme)) {
        $scheme_name = 'default';
      }
      else {
        $scheme_name = '';
      }
    }

    $file = drupal_get_path('theme', $theme) . '/' . $css_file;
    $style = '';

    if (file_exists($file)) {
      // Aggregate @imports recursively for each configured top level CSS file
      // without optimization. Aggregation and optimization will be
      // handled by drupal_build_css_cache() only.
      $style = drupal_load_stylesheet($file, FALSE);
    }

    // Add scheme selector.
    $form['colorizer_colors']['scheme'] = array(
      '#type' => 'select',
      '#title' => t('Color scheme'),
      '#options' => $color_sets,
      '#default_value' => $scheme_name,
      '#prefix' => '<div class="colorizer-form clearfix">',
      '#attached' => array(
        // Add Farbtastic color picker.
        'library' => array(
          array('system', 'farbtastic'),
        ),
        // Add custom CSS.
        'css' => array(
          $base . '/css/colorizer_admin.css' => array(),
          $radix_base . '/css/radix_colorizer.css' => array(),
        ),
        // Add custom JavaScript.
        'js' => array(
          $base . '/js/colorizer.js',
          array(
            'data' => array(
              'color' => array(
                'reference' => colorizer_get_palette($theme, $instance, TRUE),
                'schemes' => $schemes,
              ),
              'gradients' => $info['gradients'],
              'css' => $style,
            ),
            'type' => 'setting',
          ),
        ),
      ),
    );

    // Add palette fields.
    $palette = radix_colorizer_get_palette($theme, $instance);
    $names = $info['fields'];
    $form['colorizer_colors']['palette']['#tree'] = TRUE;
    $form['colorizer_colors']['palette']['#prefix'] = '<div id="palette" class="clearfix">';

    // $default_info = t('When default scheme is selected, style from theme is used.');
    // $form['colorizer_colors']['palette']['#prefix'] .= '<div class="messages status">' . $default_info . '</div>';

    $form['colorizer_colors']['palette']['#suffix'] = '</div></div>';
    foreach ($palette as $name => $value) {
      if (isset($names[$name])) {
        $form['colorizer_colors']['palette'][$name] = array(
          '#type' => 'textfield',
          '#title' => check_plain($names[$name]),
          '#default_value' => $value,
          '#size' => 8,
          '#states' => array(
            'invisible' => array(
              ':input[name="scheme"]' => array('value' => 'default'),
            ),
          ),
        );
      }
    }

    $form['theme'] = array('#type' => 'value', '#value' => $theme);
    $form['scheme'] = array('#type' => 'value', '#value' => $scheme_name);
    $form['instance'] = array('#type' => 'value', '#value' => $instance);
    $form['info'] = array('#type' => 'value', '#value' => $info);
  }

  // Hide the colorizer help.
  $form['colorizer_help']['#access'] = FALSE;

  return $form;
}

/**
 * Form validation handler for radix_colorizer_admin_settings().
 */
function radix_colorizer_admin_settings_validate($form, &$form_state) {
  module_load_include('admin.inc', 'colorizer');
  return colorizer_admin_form_validate($form, $form_state);
}

/**
 * Form submission handler for radix_colorizer_admin_settings().
 */
function radix_colorizer_admin_settings_submit($form, &$form_state) {
  // Save the file locations.
  if (!empty($form_state['values']['op']) && ($form_state['values']['op'] == $form_state['values']['colorizer_clearcache'])) {
    // Clear the colorizer css cache.
    colorizer_clearcache();
  }

  if (!empty($form_state['values']['colorizer_reset'])) {
    $instance = $form_state['values']['instance'];
    variable_del('colorizer_' . $instance . '_palette');
    // Delete old files.
    foreach (variable_get('colorizer_' . $instance . '_files', array()) as $file) {
      @drupal_unlink($file);
    }
    if (isset($file) && $file = dirname($file)) {
      @drupal_rmdir($file);
    }
    variable_del('colorizer_' . $instance . '_files');
    return;
  }

  // The rest of this is from Color.
  // Get theme coloring info.
  if (!isset($form_state['values']['info'])) {
    return;
  }
  $theme = $form_state['values']['theme'];
  $instance = $form_state['values']['instance'];
  $info = $form_state['values']['info'];
  $scheme = $form_state['values']['scheme'];

  // Resolve palette.
  $palette = $form_state['values']['palette'];
  if ($form_state['values']['scheme'] != '') {
    foreach ($palette as $key => $color) {
      if (isset($info['schemes'][$form_state['values']['scheme']]['colors'][$key])) {
        $palette[$key] = $info['schemes'][$form_state['values']['scheme']]['colors'][$key];
      }
    }
    $palette += $info['schemes']['default']['colors'];
  }

  // If default scheme, reset palette.
  if ($scheme == 'default') {
    $palette = array();
    $palette += $info['schemes']['default']['colors'];
  }

  variable_set('colorizer_' . $instance . '_palette', $palette);
  radix_colorizer_update_stylesheet($theme, $instance, $palette);

  // Save the current scheme.
  variable_set('colorizer_' . $instance . '_current_scheme', $scheme);

  drupal_set_message(t('The configuration options have been saved.'));
}