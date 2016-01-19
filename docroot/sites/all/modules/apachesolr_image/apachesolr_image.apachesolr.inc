<?php
/**
 * @file
 * Apachesolr indexing functionality.
 */

/**
 * Implement hook_apachesolr_field_mappings().
 */
function apachesolr_image_apachesolr_field_mappings() {
  $mappings = array(
    'image' => array(
      'indexing_callback' => array('apachesolr_image_indexing_callback'),
    ),
  );

  return $mappings;
}

/**
 * Indexing callback for images. Index full path and thumbnails.
 */
function apachesolr_image_indexing_callback($entity, $field_name, $index_key, array $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    $all_styles = image_styles();
    $index_styles = variable_get('apachesolr_image_index_styles', array());
    $field = $entity->{$field_name};
    list($lang, $values) = each($field);
    for ($i = 0; $i < count($values); $i++) {
      $fields[] = array(
        'key' => "sm_image_uri_$field_name",
        'value' => $values[$i]['uri'],
      );
      foreach ($index_styles as $style) {
        if (array_key_exists($style, $all_styles)) {
          $fields[] = array(
            'key' => "sm_thumb_{$style}_{$field_name}",
            'value' => theme('image_formatter', array('image_style' => $style, 'item' => $values[$i])),
          );
        }
      }
    }
  }
  return $fields;
}
