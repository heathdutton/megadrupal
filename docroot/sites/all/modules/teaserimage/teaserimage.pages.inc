<?php

/**
 * Displays module settings page.
 */
function teaserimage_admin_settings($form, &$form_state) {
  $form['default_thumbnail_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default thumbnail settings'),
    '#description' => t('These settings will apply to any selected content type but can be overriden by type specific settings.'),
  );
  $form['default_thumbnail_settings']['teaserimage_img_style'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail preset'),
    '#default_value' => variable_get('teaserimage_img_style', 0),
    '#description' => t('An Image module style preset for building the thumbnails.'),
    '#options' => image_style_options(TRUE),
    '#required' => TRUE,
  );
  $form['default_thumbnail_settings']['teaserimage_css_classes'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class(es)'),
    '#default_value' => variable_get('teaserimage_css_classes', 'teaserimage'),
    '#description' => t('CSS classes for a thumbnails.'),
    '#element_validate' => array('_teaserimage_validate_css_class'),
    '#size' => '50',
  );
  $form['default_thumbnail_settings']['teaserimage_min_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum height'),
    '#default_value' => variable_get('teaserimage_min_height', 0),
    '#description' => t('Minimum height a picture should have to be used as a thumbnail.'),
    '#maxlength' => '50',
    '#size' => '50',
  );
  $form['default_thumbnail_settings']['teaserimage_min_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum width'),
    '#default_value' => variable_get('teaserimage_min_width', 0),
    '#description' => t('Minimum width a picture should have to be used as a thumbnail.'),
    '#maxlength' => '50',
    '#size' => '50',
  );

  return system_settings_form($form);
}

/**
 * Finish callback for Batch
 */
function teaserimage_batch_finished($success, $results, $operations) {
  if ($success) {
    $message[] = format_plural($results['processed'], 'One node processed.', '@count nodes processed.');
    $message[] = format_plural($results['thumbnails'], 'One thumbnail has been created.', '@count thumbnails has been created.');
  }
  else {
    $message[] = t('Finished with an error.');
  }
  drupal_set_message(implode(' ', $message));
}

