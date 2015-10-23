<?php
/**
 * @file
 * Fancy and flexible style plugin.
 */

$plugin = array(
  'title' => t('Fancy and flexible'),
  'description' => t('Add background color, image, borders and more!'),
  'render pane' => 'mvpcreator_theme_fancy_render_pane',
  'render region' => 'mvpcreator_theme_fancy_render_region',
  'settings form' => 'mvpcreator_theme_fancy_settings_form',
  'settings form validate' => 'mvpcreator_theme_fancy_settings_validate',
  'pane settings form' => 'mvpcreator_theme_fancy_settings_form',
  'pane settings form validate' => 'mvpcreator_theme_fancy_settings_validate',

  // Signals to mvpcreator_theme that we can work as in 'full width' mode
  // if instructed.
  'full width' => TRUE,
);

/**
 * Settings form callback.
 */
function mvpcreator_theme_fancy_settings_form($settings) {
  $form = array();
  
  //
  // Settings connected with the background.
  //

  $form['background'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background'),
    '#parents' => array('settings'),
  );

  // Get the list of color options.
  $colors = mvpcreator_theme_get_color_options();
  reset($colors);

  $form['background']['background_color'] = array(
    '#type' => 'radios',
    '#title' => t('Background color'),
    '#options' => $colors,
    '#default_value' => isset($settings['background_color']) ? $settings['background_color'] : key($colors),
  );

  $form['background']['background_color_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML color code'),
    '#default_value' => isset($settings['background_color_custom']) ? $settings['background_color_custom'] : '#eee',
    '#states' => array(
      'visible' => array(
        ':input[name="settings[background_color]"]' => array('value' => 'custom'),
      ),
    ),
  );

  // Get the list of background image options.
  $images = mvpcreator_theme_get_background_image_options();
  reset($images);

  $form['background']['background_image'] = array(
    '#type' => 'radios',
    '#title' => t('Background image'),
    '#options' => $images,
    '#default_value' => isset($settings['background_image']) ? $settings['background_image'] : key($images),
  );

  $form['background']['background_image_custom'] = array(
    '#type' => 'managed_file',
    '#title' => t('Background image'),
    '#default_value' => isset($settings['background_image_custom']) ? $settings['background_image_custom'] : NULL,
    '#states' => array(
      'visible' => array(
        ':input[name="settings[background_image]"]' => array('value' => 'custom'),
      ),
    ),
  );

  $form['background']['background_image_type'] = array(
    '#type' => 'select',
    '#title' => t('Background image type'),
    '#options' => array(
      'hero' => t('Hero'),
      'tiled' => t('Tiled'),
      'stretch' => t('Stretch'),
    ),
    '#default_value' => isset($settings['background_image_type']) ? $settings['background_image_type'] : 'hero',
    '#states' => array(
      'visible' => array(
        ':input[name="settings[background_image]"]' => array('value' => 'custom'),
      ),
    ),
  );

  if (module_exists('media')) {
    $form['background']['background_image_custom']['#type'] = 'media';
    $form['background']['background_image_custom']['#media_options'] = array(
      'global' => array(
        'types' => array('image'),
        'file_extensions' => 'jpg jpeg gif png',
        'enabledPlugins' => array('upload', 'media_default--media_browser_1'),
        'schemes' => array('public'),
        'uri_scheme' => 'public',
      ),
    );
  }

  //
  // Settings connected with text.
  //

  $form['text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text'),
    '#parents' => array('settings'),
  );

  $form['text']['text_color'] = array(
    '#type' => 'select',
    '#title' => t('Text color'),
    '#options' => array(
      'dark' => t('Dark (for a light background)'),
      'light' => t('Light (for a dark background)'),
    ),
    '#default_value' => isset($settings['text_color']) ? $settings['text_color'] : 'dark',
  );

  //
  // Settings connected with size.
  //

  $form['size'] = array(
    '#type' => 'fieldset',
    '#title' => t('Size'),
    '#parents' => array('settings'),
  );

  $form['size']['min_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum height'),
    '#default_value' => isset($settings['min_height']) ? $settings['min_height'] : '0',
    '#field_suffix' => 'px',
  );

  $padding_types = array(
    'padding_top' => t('Padding top'),
    'padding_left' => t('Padding left'),
    'padding_bottom' => t('Padding bottom'),
    'padding_right' => t('Padding right'),
  );
  foreach ($padding_types as $padding_type => $padding_label) {
    $form['size'][$padding_type] = array(
      '#type' => 'textfield',
      '#title' => $padding_label,
      '#default_value' => isset($settings[$padding_type]) ? $settings[$padding_type] : '0',
      '#field_suffix' => 'px',
    );
  }

  $form['size']['full_width_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make content of region full width'),
    '#description' => t('This will only work if this layout supports it and it renders the full page.'),
    '#default_value' => isset($settings['full_width_content']) ? $settings['full_width_content'] : FALSE,
  );

  return $form;
}

/**
 * Settings validate callback.
 */
function mvpcreator_theme_fancy_settings_validate($form, $settings) {
  if ($settings['background_color'] == 'custom' && !mvpcreator_theme_is_valid_color($settings['background_color_custom'])) {
    form_set_error('settings][background_color_custom', t('Invalid HTML color code. Should look like <em>#EEE</em> or <em>#EEEEEE</em>.'));
  }

  // Check all the integer fields.
  $integer_fields = array(
    'padding_top',
    'padding_left',
    'padding_bottom',
    'padding_right',
  );
  foreach ($integer_fields as $integer_field) {
    if ((int)$settings[$integer_field] != $settings[$integer_field] || $settings[$integer_field] < 0) {
      form_set_error('settings][' . $integer_field, t('Must be a positive, whole number.'));
    }
  }
}

/**
 * Generate CSS classes or attributes for the given settings.
 */
function _mvpcreator_theme_fancy_build_attributes($settings, $type) {
  $class = array($type . '-mvpcreator-theme-fancy');
  $style = array();

  // Background color.
  if ($settings['background_color'] != 'custom') {
    $class[] = $type . '-mvpcreator-theme-bgcolor-' . str_replace('_', '-', $settings['background_color']);
  }
  elseif (mvpcreator_theme_is_valid_color($settings['background_color_custom'])) {
    $style['background-color'] = $settings['background_color_custom'];
  }

  // Background image.
  if ($settings['background_image'] == 'none') {
    // Do nothing.
  }
  elseif ($settings['background_image'] != 'custom') {
    $class[] = $type . '-mvpcreator-theme-bgimage-' . str_replace('_', '-', $settings['background_image']);
  }
  elseif ($file = file_load($settings['background_image_custom'])) {
    $image_url = file_create_url($file->uri);
    $style['background-image'] = 'url(' . $image_url . ')';
    $class[] = $type . '-mvpcreator-theme-bgimage-type-' . $settings['background_image_type'];
  }

  // Text settings.
  if (!empty($settings['text_color'])) {
    $class[] = $type . '-mvpcreator-theme-text-' . $settings['text_color'];
  }

  // Size settings.
  if (!empty($settings['min_height'])) {
    $style['min-height'] = $settings['min_height'] . 'px';
  }
  if (!empty($settings['padding_top'])) {
    $style['padding-top'] = $settings['padding_top'] . 'px';
  }
  if (!empty($settings['padding_left'])) {
    $style['padding-left'] = $settings['padding_left'] . 'px';
  }
  if (!empty($settings['padding_bottom'])) {
    $style['padding-bottom'] = $settings['padding_bottom'] . 'px';
  }
  if (!empty($settings['padding_right'])) {
    $style['padding-right'] = $settings['padding_right'] . 'px';
  }

  // Convert into an array.
  $attributes = array();
  if (!empty($class)) {
    $attributes['class'] = $class;
  }
  if (!empty($style)) {
    $attributes['style'] = $style;
  }
  return $attributes;
}

/**
 * Collapse an associative style array into a simple array.
 */
function _mvpcreator_theme_fancy_style_array($style) {
  $style_array = array();
  foreach ($style as $style_name => $style_value) {
     $style_array[] = "$style_name: $style_value;";
  }
  return $style_array;
}

/**
 * Take an attributes array and generate the string.
 */
function _mvpcreator_theme_fancy_render_attributes($attributes) {
  // Collapse style into an array.
  if (!empty($attributes['style'])) {
    $attributes['style'] = _mvpcreator_theme_fancy_style_array($attributes['style']);
  }
  return drupal_attributes($attributes);
}

/**
 * Theme function for the pane style.
 */
function theme_mvpcreator_theme_fancy_render_pane($vars) {
  $content = $vars['content'];
  $pane = $vars['pane'];
  $display = $vars['display'];
  $plugin = $vars['style'];
  $settings = $vars['settings'];

  $attributes = _mvpcreator_theme_fancy_build_attributes($settings, 'pane');
  if (!empty($attributes['class'])) {
    $content->css_class .= ' ' . implode(' ', $attributes['class']);
  }
  if (!empty($attributes['style'])) {
    $content->style_attribute = implode(' ', _mvpcreator_theme_fancy_style_array($attributes['style']));
  }
  
  // if the style is gone or has no theme of its own, just display the output.
  return theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));
}

/**
 * Theme function for the region style.
 */
function theme_mvpcreator_theme_fancy_render_region($vars) {
  $display = $vars['display'];
  $region_id = $vars['region_id'];
  $panes = $vars['panes'];
  $settings = $vars['settings'];

  $add_container = $vars['full_width'] && empty($settings['full_width_content']) && mvpcreator_theme_is_region_full_width($vars['region_id'], $vars['display']->layout);

  $attributes = _mvpcreator_theme_fancy_build_attributes($settings, 'region');

  // If adding a container, move some attributes to the container.
  $container_attributes = array();
  if ($add_container) {
    foreach ($attributes['style'] as $style_name => $style_value) {
      // Move all the padding attributes.
      if (strpos($style_name, 'padding') === 0) {
        $container_attributes['style'][$style_name] = $style_value;
        unset($attributes['style'][$style_name]);
      }
    }
  }

  // Add our default classes.
  if (empty($attributes['class'])) {
    $attributes['class'] = array();
  }
  $attributes['class'] = array_merge(
    array('region', 'region-' . $vars['region_id'], 'region-mvpcreator-theme-bgcolor'),
    $attributes['class']);

  $output = '<div' . _mvpcreator_theme_fancy_render_attributes($attributes) . '>';

  if ($add_container) {
    $output .= '<div class="container">';
    if (!empty($container_attributes)) {
      $output .= '<div class="container-inner"' . _mvpcreator_theme_fancy_render_attributes($container_attributes) . '>';
    }
  }
  $output .= implode('<div class="panel-separator"></div>', $panes);
  if ($add_container) {
    $output .= '</div>';
    if (!empty($container_attributes)) {
      $output .= '</div>';
    }
  }

  $output .= '</div>';

  return $output;
}
