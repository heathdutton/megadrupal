<?php
/**
 * Admin settings interface.
 */

/**
 * Create admin settings form.
 */
function commerce_options_as_images_settings_form($form, $form_state) {
  $form = array(
    'commerce_options_as_images_image_style' => array(
      '#type' => 'select',
      '#title' => t('Select image style for displaying the preview images'),
      '#options' => commerce_options_as_images_get_image_styles(),
      '#default_value' => variable_get('commerce_options_as_images_image_style'),
    ),
    'commerce_options_as_images_image_field' => array(
      '#type' => 'select',
      '#title' => t('Select product image field to make it as a widget'),
      '#options' => commerce_options_as_images_get_product_image_fields(),
      '#default_value' => variable_get('commerce_options_as_images_image_field'),
    ),
  );

  return system_settings_form($form);
}

/**
 * Get all the image styles.
 */
function commerce_options_as_images_get_image_styles() {
  $result = array();
  $image_styles = image_styles();

  foreach ($image_styles as $key => $image_style) {
    $result[$key] = $image_style['label'];
  }

  return $result;
}

/**
 * Get all the image fields from product bundle.
 */
function commerce_options_as_images_get_product_image_fields() {
  $result = array();
  $fields = field_info_instances('commerce_product', 'product');
  foreach ($fields as $key => $field) {
    if ((array_key_exists('module', $field['display']['line_item']) && $field['display']['line_item']['module'] == 'image') ||
      (array_key_exists('module', $field['widget']) && $field['widget']['module'] == 'image')) {
      $result[$key] = $field['label'] . ' (' . $key . ')';
    }
  }

  return $result;
}
