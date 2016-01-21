<?php
/**
 * @file
 * Fullscreen gallery field formatter implementation.
 */

/**
 * Implements hook_field_formatter_info().
 */
function fullscreen_gallery_field_formatter_info() {
  return array(
    'fullscreen_gallery' => array(
      'label' => t('Fullscreen gallery'),
      'field types' => array('image'),
      'settings' => array(
        'image_style' => '',
        'use_default' => variable_get('fullscreen_gallery_use_default', TRUE),
        'right_sidebar_width' => variable_get('fullscreen_gallery_rs_width', ''),
        'right_sidebar_width_type' => variable_get('fullscreen_gallery_rs_width_type', 'px'),
        'disable_titles' => variable_get('fullscreen_gallery_disable_titles', FALSE),
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function fullscreen_gallery_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display_settings = $instance['display'][$view_mode]['settings'];
  $element = array();

  // Set the image style for displaying images.
  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#default_value' => $display_settings['image_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => image_style_options(FALSE, PASS_THROUGH),
  );

  // Checkbox for using default fullscreen gallery settings.
  $element['use_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use default settings'),
    '#default_value' => $display_settings['use_default'],
    '#description' => t('Default settings are inherited from <a href="@url">admin/config/media/fullscreen_gallery</a>', array('@url' => url('admin/config/media/fullscreen_gallery'))),
    '#required' => FALSE,
  );

  // Right sidebar width value.
  $element['right_sidebar_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of the right side bar'),
    '#default_value' => $display_settings['right_sidebar_width'],
    '#size' => 4,
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    // Hide this setting when the use default checkbox is checked.
    '#states' => array(
      'visible' => array(
        ':input[name="fields[field_image][settings_edit_form][settings][use_default]"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Right sidebar width type (px or %).
  $element['right_sidebar_width_type'] = array(
    '#type' => 'select',
    '#title' => '',
    '#default_value' => $display_settings['right_sidebar_width_type'],
    '#description' => t('Right sidebar appears if there is any content on <em>Full screen gallery right sidebar</em> region, and a valid sidebar width is given.'),
    '#options' => array(
      'px' => t('pixels'),
      'pe' => t('percent'),
    ),
    // Hide this setting when the use default checkbox is checked.
    '#states' => array(
      'visible' => array(
        ':input[name="fields[field_image][settings_edit_form][settings][use_default]"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Checkbox for disabling title display.
  $element['disable_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable image titles'),
    '#default_value' => $display_settings['disable_titles'],
    '#description' => t("Hide image titles in fullscreen gallery."),
    '#required' => FALSE,
    // Hide this setting when the use default checkbox is checked.
    '#states' => array(
      'visible' => array(
        ':input[name="fields[field_image][settings_edit_form][settings][use_default]"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function fullscreen_gallery_field_formatter_settings_summary($field, $instance, $view_mode) {
  // Load css file for configuration form styling.
  drupal_add_css(drupal_get_path('module', 'fullscreen_gallery') . '/css/config_form.css');

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  $image_styles = image_style_options(FALSE, PASS_THROUGH);

  $style = (isset($image_styles[$settings['image_style']]) and $image_styles[$settings['image_style']]) ? $image_styles[$settings['image_style']] : t('Original image');

  // Create summary text about current settings.
  $summary = array();
  $summary[] = t('Image style: @style', array('@style' => $style));

  if (isset($settings['use_default']) and $settings['use_default']) {
    // If default settings used, provide link for updating default settings.
    $summary[] = t('Settings: <a href="@url">default</a>', array('@url' => url('admin/config/media/fullscreen_gallery')));
  }
  else {
    // Add custom settings to summary text.
    if (isset($settings['right_sidebar_width']) and $settings['right_sidebar_width']) {
      $summary[] = t('Right sidebar: @width@type', array('@width' => $settings['right_sidebar_width'], '@type' => ($settings['right_sidebar_width_type'] == 'pe' ? '%' : 'px')));
    }
    else {
      $summary[] = t('Right sidebar disabled.');
    }
    if (isset($settings['disable_titles']) and $settings['disable_titles']) {
      $summary[] = t('Image titles disabled.');
    }
  }
  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 *
 * Adds a fullscreen gallery url to images.
 */
function fullscreen_gallery_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $entity_ids = entity_extract_ids($entity_type, $entity);
  $entity_id = array_shift($entity_ids);
  $destination = drupal_get_destination();

  // Create gallery link.
  $uri = entity_uri($entity_type, $entity);
  $uri['path'] = 'fullscreen_gallery/' . $entity_type . '/' . $entity_id . '/' . $field['field_name'];

  foreach ($items as $delta => $item) {
    $element[$delta] = array(
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => $display['settings']['image_style'],
    );

    if (isset($uri)) {
      $item_uri = $uri;
      $item_uri['path'] .= '/' . $delta;
      $item_uri['options']['query']['thumbnail_offset'] = $delta;
      if (isset($destination['destination'])) {
        $item_uri['options']['query']['destination'] = $destination['destination'];
      }
      $element[$delta]['#path'] = $item_uri;
      $element[$delta]['#item']['attributes'] = array(
        'id' => 'fs_gallery_item_' . $entity_type . '_' . $entity_id . '_' . $field['field_name'] . '_' . $delta,
      );
    }
  }
  return $element;
}
