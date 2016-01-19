<?php

function ckeditor_insert_settings_form() {
  $display_name = 'CKEditor Insert';
  $file_fields = $image_fields = array(0 => t('None'));
  $file_fields_count = $image_fields_count = 0;
  $unlimited = array();
  foreach (field_info_fields() as $field_name => $info) {
    if (in_array($info['type'], array(
      'file',
      'image',
    ))) {
      if ($info['type'] == 'file') {
        $file_fields_count++;
      }
      else {
        $image_fields_count++;
      }
      if ($info['cardinality'] == -1) {
        $unlimited[] = $field_name;
        foreach ($info['bundles'] as $entity_type => $bundles) {
          foreach ($bundles as $bundle_name) {
            $instance = field_info_instance($entity_type, $field_name, $bundle_name);
            if (isset($instance['widget']['settings']['insert']) && $instance['widget']['settings']['insert']) {
              if ($info['type'] == 'image') {
                $image_fields[$field_name] = $instance['label'];
              }
              else {
                $file_fields[$field_name] = $instance['label'];
              }
            }
          }
        }
      }
    }
  }
  if ($file_fields_count == 0 and $image_fields_count == 0) {
    drupal_set_message(t('@name did not find file or image fields.', array('@name' => $display_name)), 'warning');
  }
  else if (count($unlimited) == 0) {
    drupal_set_message(t('@name did not find file or image fields with "Number of values" set to Unlimited.', array('@name' => $display_name)), 'warning');
  }
  else if (count($file_fields) == 0 and count($image_fields) == 0) {
    drupal_set_message(t('@name did not find file or image fields with the widget "insert" switched on.', array('@name' => $display_name)), 'warning');
  }
  $form['ckeditor_insert_notes'] = array('#markup' => '<p>' . t('@name works on file and image fields. We found @file fields of type file and @image fields of type image.', array(
      '@name' => $display_name,
      '@file' => $file_fields_count,
      '@image' => $image_fields_count,
    )) . '</p>' . '<p>' . t('@name requires fields with "Number of values" set to Unlimited. The file and image fields set to unlimited are: @unlimited', array(
      '@name' => $display_name,
      '@unlimited' => implode(', ', $unlimited)
    )) . '</p>' . '<p>' . t('@name requires fields with the widget "insert" switched on. The fields listed in the selection boxes below have insert switched on.', array('@name' => $display_name)) . '</p><p>' . t('If your field is missing, select Administration, Structure, Content types, your content type, then edit the file or image field. Scroll down to the Insert section and expand the section. Switch on the checkbox for "Enable insert button". Save the changes to the field settings.') . '</p>');
  $form['ckeditor_insert_file_field'] = array(
    '#type' => 'select',
    '#title' => t('File field for insert'),
    '#options' => $file_fields,
    '#default_value' => variable_get('ckeditor_insert_file_field', 0),
  );

  $form['ckeditor_insert_image_field'] = array(
    '#type' => 'select',
    '#title' => t('Image field for insert'),
    '#options' => $image_fields,
    '#default_value' => variable_get('ckeditor_insert_image_field', 0),
  );

  return system_settings_form($form);
}
