<?php
/**
 * @file
 * Provides filtering of encoded URLs in fields being edited based on settings.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implements hook_field_attach_load().
 *
 * @todo
 * This hook is called on entity view and edit. In both cases, each filter
 * process callback has already been invoked. Because the filtered text is in
 * the safe_value key which is rendered on entity view, we do not need to do
 * anything further on the view case.
 *
 * On entity edit, the unfiltered text (from the value key) is displayed in the
 * textarea field. In order for someone to edit this text or for an image to
 * display in a WYSIWYG editor, the portable path filters need to be processed
 * on the raw text value.
 *
 * How can we determine if this hook is being invoked as part of an edit?
 *
 * On an user entity, the safe_value omits the style attribute on image tags?
 */
function _portable_path_field_attach_load($entity_type, $queried_entities, $age, $options) {
  // Get information.
  $info = entity_get_info($entity_type);
  $formats = filter_formats();
  $settings = portable_path_config();
  $filter_info = portable_path_filter_info();

  // An entity may not have a bundle key, e.g., user.
  $bundle_key = $info['entity keys']['bundle'];
  foreach ($queried_entities as &$entity) {
    $bundle_name = isset($entity->$bundle_key) ? $entity->$bundle_key : NULL;
    $instances = field_info_instances($entity_type, $bundle_name);
    if (isset($instances[$entity_type])) {
      // Array structure is one level deeper on user than node.
      // User has $instances['user']['field_description'].
      $instances = $instances[$entity_type];
    }
    foreach ($instances as $instance) {
      if (in_array($instance['field_name'], $settings['field_names'])) {
        $field_name = $instance['field_name'];
        // Load the field to read its columns and use first column for value.
        $field = field_info_field($instance['field_name']);
        $column = current(array_keys($field['columns']));
        // Get the language key from the field array.
        $keys = array_keys($entity->$field_name);
        if (empty($keys)) {
          // Field is not set, i.e., no value.
          continue;
        }
        $language = current($keys);
        $format = $entity->{$field_name}[$language][0]['format'];
        $filters = filter_list_format($format);
        // Which of our filters is active in the format and in what order?
        $our_filters = array_filter($filters, 'portable_path_array_filter');

        if (empty($our_filters)) {
          continue;
        }

        if (!empty($entity->$field_name) && !empty($entity->{$field_name}[$language][0][$column])) {
          foreach (array_keys($our_filters) as $key) {
            $function = $filter_info[$key]['process callback'];
            if (function_exists($function)) {
              $filter = $our_filters[$key];
              $entity->{$field_name}[$language][0][$column] = $function($entity->{$field_name}[$language][0][$column], $filter, $formats[$format]);
            }
          }
        }
      }
    }
  }
}
