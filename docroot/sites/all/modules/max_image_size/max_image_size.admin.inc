<?php
/**
 * @file
 * Administration related hook implementations.
 */

/**
 * Implements hook_form().
 */
function max_image_size_system_settings_form() {

  $form = array();

  $dimensions = max_image_size_get_max_dimensions();

  $form['dimensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Maximum dimensions'),
    '#description' => t('The maximum allowable dimensions that images will be used to resize images. Any images over the given dimensions will be scaled to fit.')
  );

  $form['dimensions']['max_image_size_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The maximum allowed width for Drupal managed images.'),
    '#default_value' => $dimensions['width'],
    '#size' => 5,
    '#maxlength' => 6,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['dimensions']['max_image_size_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The maximum allowed height for Drupal managed images.'),
    '#default_value' => $dimensions['height'],
    '#size' => 5,
    '#maxlength' => 6,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['filesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Filesize threshold'),
    '#description' => t('The minimum size (in bytes) before the dimensions of an image are checked. Use 0 or leave blank to disable this feature.'),
    '#default_value' => variable_get('max_image_size_filesize_threshold'),
    '#size' => 5,
    '#maxlength' => 6,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['max_image_size_cron_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cron processing enabled'),
    '#description' => t('Check this box to enable daily resizing of images to be below the specified dimensions.'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('max_image_size_cron_enabled', TRUE),
  );

  $form['max_image_size_presave_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Presave enabled'),
    '#description' => t('Check this box to enable resizing of images when they are added to Drupal.'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('max_image_size_presave_enabled', TRUE),
  );

  // Add a little information about the number of images that will be resized.
  $pending = count(max_image_size_get_unprocessed_images());
  $form['oversized_images'] = array(
    '#type' => 'item',
    '#title' => t('Pending images'),
    '#title_display' => 'invisible',
    '#description' => t('The number of images whose dimensions have not yet been checked.'),
    '#markup' => ($pending <= 0 ?
      t('There are no unprocessed images.') :
      format_plural($pending, 'There is one image not processed.', 'There are @count images not processed.')),
  );

  $form['#submit'][] = 'max_image_size_system_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Form submission handler.
 */
function max_image_size_system_settings_form_submit($form, &$form_state) {

  // Again state that the dimension settings are very important and the process
  // is non-reversible.
  drupal_set_message(t('The process of resizing an image is non-reversible. Please review the entered dimensions to ensure that they are correct. Once an image has been resized it will forever be altered.'), 'warning');

  $values = $form_state['values'];

  // If the user has disabled both processing options then let them know we
  // won't actually do anything until they are enabled.
  if (empty($values['max_image_size_cron_enabled']) && empty($values['max_image_size_presave_enabled'])) {

    drupal_set_message(t('Both the cron and presave options are disabled. This module will not take any action until one of those options is enabled.'), 'warning');
  }
}
