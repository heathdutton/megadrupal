<?php

/**
 * @file
 * Admin pages for Gallery Archive module.
 */

/**
 * Gallery Archive settings form.
 */
function gallery_archive_settings_form() {
  // Content type for gallery archive.
  $content_types = node_type_get_types();
  foreach ($content_types as $key => $value) {
    $content_types[$key] = $value->name;
  }
  $content_types = array(0 => t('- Select -')) + $content_types;
  $form['gallery_archive_gallery_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Content Type'),
    '#options' => $content_types,
    '#description' => t("Choose the node type you're using for image galleries."),
    '#default_value' => variable_get('gallery_archive_gallery_content_type', 0),
  );

  // Image field in all content types, with content type.
  $all_fields = field_info_instances();
  $image_fields = array(0 => t('- Select -'));
  foreach ($all_fields as $key => $value) {
    if ($key == 'node') {
      foreach ($value as $type => $fields) {
        foreach ($fields as $field_name => $field_info) {
          // Add image fields to the $images_fields array.
          if ($field_info['widget']['module'] == 'image') {
            $image_fields[$field_name] = $field_info['label'] . ' (' . $field_info['bundle'] . ')';
          }
        }
      }
    }
  }
  $form['gallery_archive_gallery_image_field'] = array(
    '#type' => 'select',
    '#title' => t('Image Field'),
    '#options' => $image_fields,
    '#description' => t("Choose the image field attached to this content type that stores the images that will be archived."),
    '#default_value' => variable_get('gallery_archive_gallery_image_field', 0),
    // Only show when a content type is selected.
    '#states' => array(
      'invisible' => array(
        '#edit-gallery-archive-gallery-content-type' => array('value' => 0),
      ),
    ),
  );

  // Archive deletion frequency.
  $form['gallery_archive_deletion_frequency'] = array(
    '#type' => 'select',
    '#title' => 'Deletion Frequency',
    '#options' => array(
      0 => t('Never'),
      1 => t('When Cron runs'),
    ),
    '#description' => t('To save space on your server, you can have the gallery archives folder purged on cron runs.'),
    '#default_value' => variable_get('gallery_archive_deletion_frequency', 0),
  );

  return system_settings_form($form);
}
