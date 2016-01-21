<?php

/**
 * @file
 * Contains all hooks and related methods for the lightgallery_formatter field
 * formatter.
 */

/**
 * Implements hook_field_formatter_info().
 */
function lightgallery_field_formatter_info() {
  return array(
    'lightgallery' => array(
      'label' => t('Lightgallery'),
      'field types' => array('image'),
      'settings' => array(
        'image_styles' => array(
          'lightgallery_image_style' => NULL,
          'lightgallery_image_thumb_style' => NULL,
        ),
        'lightgallery_use_thumbs' => 1,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function lightgallery_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $image_styles = image_style_options(FALSE);

  $element['image_styles']['lightgallery_image_style'] = array(
    '#title' => t('Lightgallery image style'),
    '#type' => 'select',
    '#default_value' => $settings['image_styles']['lightgallery_image_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => $image_styles,
    '#description' => t('Light gallery image style.'),
  );

  $element['image_styles']['lightgallery_image_thumb_style'] = array(
    '#title' => t('Content image style'),
    '#type' => 'select',
    '#default_value' => $settings['image_styles']['lightgallery_image_thumb_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => $image_styles,
    '#description' => t('Content image style.'),
  );

  $element['lightgallery_use_thumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use thumbnails'),
    '#default_value' => $settings['lightgallery_use_thumbs'],
    '#description' => t('Indicate if you want to use thumbnails in the LightGallery.')
  );


  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function lightgallery_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  $image_styles = image_style_options(FALSE);
  // Unset possible 'No defined styles' option.
  unset($image_styles['']);
  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  if (isset($image_styles[$settings['image_styles']['lightgallery_image_style']])) {
    $summary[] = t('Lightgallery image style: @style',
        array('@style' => $image_styles[$settings['image_styles']['lightgallery_image_style']]));
  }
  else {
    $summary[] = t('Lightgallery image style: Original image');
  }

  if (isset($image_styles[$settings['image_styles']['lightgallery_image_thumb_style']])) {
    $summary[] = t('Content image style: @style',
        array('@style' => $image_styles[$settings['image_styles']['lightgallery_image_thumb_style']]));
  }
  else {
    $summary[] = t('Content image style: Original image');
  }

  $summary[] = ($settings['lightgallery_use_thumbs']) ? t('Use thumbs in gallery: Yes') : t('Use thumbs in gallery: No');


  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function lightgallery_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  // Render elements as one piece of markup so that items can be rendered as item list.
  $element[0] = array(
    '#theme' => 'lightgallery_image_formatter',
    '#items' => $items,
    '#entity_type' => $entity_type,
    '#entity' => $entity,
    '#node' => $entity, // Left for legacy support.
    '#field' => $field,
    '#display_settings' => $display['settings'],
  );


  return $element;
}
