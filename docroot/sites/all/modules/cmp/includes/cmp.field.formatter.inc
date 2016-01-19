<?php
/**
 * @file
 * Hooks and callbacks for field formatter.
 */

/**
 * Implements hook_field_formatter_info().
 */
function cmp_field_formatter_info() {
  return array(
    'cmp_field_formatter' => array(
      'label'           => t('CaptureMyPage: Default'),
      'field types'     => array('cmp'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'settings'        => array(
        'url'         => '',
        'image_style' => '',
        'image_link'  => '',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function cmp_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $image_styles = image_style_options(FALSE, PASS_THROUGH);

  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => $image_styles,
  );

  $link_types = array(
    'content' => t('Content'),
    'website' => t('Website URL'),
    'file' => t('File'),
  );

  $element['image_link'] = array(
    '#title' => t('Link image to'),
    '#type' => 'select',
    '#default_value' => $settings['image_link'],
    '#empty_option' => t('Nothing'),
    '#options' => $link_types,
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function cmp_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  $image_styles = image_style_options(FALSE, PASS_THROUGH);
  // Unset possible 'No defined styles' option.
  unset($image_styles['']);
  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  if (isset($image_styles[$settings['image_style']])) {
    $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
  }
  else {
    $summary[] = t('Original image');
  }

  $link_types = array(
    'content' => t('Linked to content'),
    'website' => t('Linked to Website URL'),
    'file' => t('Linked to file'),
  );
  // Display this setting only if image is linked.
  if (isset($link_types[$settings['image_link']])) {
    $summary[] = $link_types[$settings['image_link']];
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function cmp_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  // Check if the formatter involves a link.
  switch ($display['settings']['image_link']) {
    case 'content':
      $uri = entity_uri($entity_type, $entity);
      break;

    case 'website':
      $link_website = TRUE;
      break;

    case 'file':
      $link_file = TRUE;
      break;
  }

  foreach ($items as $delta => $item) {
    if (isset($link_website) && $link_website) {
      $uri = array(
        'path' => $item['url'],
        'options' => array(),
      );
    }

    if (isset($item['uri']) && isset($link_file) && $link_file) {
      $uri = array(
        'path' => file_create_url($item['uri']),
        'options' => array(),
      );
    }

    $element[$delta] = array(
      '#theme' => 'cmp_default_formatter',
      '#item' => $item,
      '#image_style' => $display['settings']['image_style'],
      '#path' => isset($uri) ? $uri : '',
    );
  }

  return $element;
}
