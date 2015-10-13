<?php

/**
 * @file
 * Common functions.
 */

define('INLINE_STYLES_CLEANER_PATH_TO_CSS_FILE', 'public://inline_styles_cleaner.css');
define('INLINE_STYLES_CLEANER_DOWNLOAD_LINK', 'admin/config/content/inline-styles-cleaner/download');

/**
 * Function for $options generation from form's checkboxes.
 */
function inline_styles_cleaner_common_get_text_fields_names() {
  $text_fields = array();

  foreach (field_info_fields() as $field_name => $field_info) {
    if ($field_info['module'] == 'text') {
      $appears_in = array();

      foreach ($field_info['bundles'] as $entity_type => $bundles) {
        foreach ($bundles as $bundle) {
          $appears_in[] = $entity_type . ':' . $bundle;
        }
      }

      $text_fields[$field_name] = t('Field name: <strong>@field_name</strong>. Field type: <strong>@field_type</strong>. Appears in: <strong>@appears_in</strong>', array(
          '@field_name' => $field_name,
          '@field_type' => $field_info['type'],
          '@appears_in' => implode(', ', $appears_in),
        )
      );
    }
  }

  return $text_fields;
}

/**
 * Function for detecting classes with similar css property and values.
 *
 * @param array $styles
 *   Array with classes as keys and array with property-value pairs as values.
 *
 * @return array $similar_classes_values
 *   Array with hashes from property-value as keys and array
 *   with classes as values.
 */
function inline_styles_cleaner_detect_classes_with_similar_values(array $styles) {
  // Generate array with hashes from $styles values as keys to detect similar
  // values from different classes.
  $similar_classes_values = array();

  foreach ($styles as $class_name => &$properties) {
    // First of all we need to sort $styles array by keys
    // and its values by values.
    // Because
    // array(color => array('red'), border => array('solid', '2px', 'red')
    // and array(border => array('solid', 'red', '2px'), color => array('red')
    // will be 'different' css rules but logically it's not true.
    ksort($properties);

    foreach ($properties as &$property) {
      sort($property);
    }

    $similar_classes_values[md5(serialize($properties))][] = $class_name;
  }

  return $similar_classes_values;
}

/**
 * Function for adding dots to classes in array.
 *
 * @param array $classes
 *   Array with classes without dots.
 *
 * @return array $classes
 *   Array with dotted classes.
 */
function inline_styles_cleaner_add_dot_to_classes(array $classes) {
  foreach ($classes as &$class) {
    $class = ".$class";
  }

  return $classes;
}

/**
 * Function for css rules generation.
 *
 * @param array $styles
 *   Array with classes as keys and array with property-value pairs as values.
 *
 * @return string $css_rules
 *   String with generated css rules.
 */
function inline_styles_cleaner_common_generate_css_rules(array $styles) {
  $css_rules = '';

  foreach (inline_styles_cleaner_detect_classes_with_similar_values($styles) as $classes) {
    $property_value = array();

    foreach ($styles[$classes[0]] as $prop => $value) {
      $property_value[] = $prop . ': ' . implode(' ', $value) . ';';
    }

    $css_rules .= implode(', ', inline_styles_cleaner_add_dot_to_classes($classes)) . ' {' . "\n\t" . implode("\n\t", $property_value) . "\n" . '}' . "\n\n";
  }

  return $css_rules;
}

/**
 * Function for updating field's value.
 *
 * @param string $table_name
 *   Table name for sql query.
 * @param string $column_name
 *   Column name for sql query.
 * @param string $new_value
 *   String without inline styles.
 * @param \stdClass $field_instance
 *   Object with field's metadata.
 */
function inline_styles_cleaner_update_field_value($table_name, $column_name, $new_value, stdClass $field_instance) {
  db_update($table_name)
    ->fields(array($column_name => $new_value))
    ->condition('entity_id', $field_instance->entity_id)
    ->execute();

  cache_clear_all("field:{$field_instance->entity_type}:{$field_instance->entity_id}", 'cache_field');
}

/**
 * Function for cleaning inline styles from field's value.
 *
 * @param string $column_name
 *   Column name for sql query.
 * @param array $field_settings
 *   Array with field_name and field_instance variables.
 * @param array $css_settings
 *   Array with generate_css_needed and class_name variables.
 * @param array $context
 *   Array with batch's metadata.
 */
function inline_style_cleaner_clean_field_value($column_name, array $field_settings, array $css_settings, array $context) {
  $table_name = 'field_data_' . $field_settings['field_name'];
  $generate_css_needed = $css_settings['generate_css_needed'];
  $update_existing_css_needed = $css_settings['update_existing_css_needed'];
  $class_name = $css_settings['class_name'];
  $old_class_name = $css_settings['old_class_name'];

  // Get field value.
  $clean_value = $field_settings['field_instance']->{$column_name};

  // Get all opened tags from field value.
  preg_match_all('/<[^\/]{1,1}.*?>/i', $clean_value, $matches);

  // Walk through each tags and erase inline styles.
  foreach ($matches[0] as $key => $match) {
    $replacement = $match;

    // Create array with style values for css rules if needed.
    if ($generate_css_needed) {
      // Insert/add class only to not processed strings.
      if (strpos($match, "class=\"$class_name-") === FALSE || strpos($match, 'style="') !== FALSE) {
        // Generate css array.

        // New additional class for processed tag. It looks like
        // class_name-{entity_id}-{field_name_(value|summary)}-{element_number}.
        $class_placeholder = "$class_name-{$field_settings['field_instance']->entity_id}-$column_name-$key";

        // Get style attribute with values from current tagged string.
        preg_match_all('/style=".*?"/i', $match, $style_attribute);

        // Get all properties from style="..." attribute as array.
        // For example: style="background: red; border: 2px solid green" will be
        // array('background: red', 'border: 2px solid green').
        if (!empty($style_attribute[0])) {
          $exploded_style = array_filter(explode(';', strtolower(str_replace('"', '', str_replace('style="', '', $style_attribute[0][0])))), 'inline_styles_cleaner_filter_spaced_strings');

          // Walk through each style string and generate array from it and
          // put this array to $context['results'].
          // For example: 'border: 2px solid green' will be
          // array('border' => array('2px', 'solid', 'green')).
          foreach ($exploded_style as $style) {
            $property_value_pair = explode(':', $style);
            $context['results'][$class_placeholder][trim($property_value_pair[0])] = array_values(array_filter(explode(' ', trim($property_value_pair[1]))));
          }

          // If current tagged string contains class attribute we should insert
          // generated new class.
          if (strpos($match, 'class="') !== FALSE) {
            // Insert new class only if there is no it yet.
            if (strpos($match, $class_placeholder) === FALSE) {
              // Remove old class if we want override css rules.
              if (!$update_existing_css_needed) {
                $replacement = str_replace("$old_class_name-{$field_settings['field_instance']->entity_id}-$column_name-$key", '', $replacement);
              }
              $replacement = str_replace("class=\"", "class=\"$class_placeholder ", $replacement);
            }
          }
          // If there is no class attribute we should add it.
          else {
            $replacement = preg_replace('/style=".*?"/i', "class=\"$class_placeholder\"", $replacement);
          }
        }
      }
    }

    // Remove style="..." from tag.
    $replacement = preg_replace('/style=".*?"/i', '', str_replace('  ', ' ', $replacement));

    // Replace current string with style by purified string from field value.
    $clean_value = preg_replace("/$match/", $replacement, $clean_value, 1);
  }

  // Write new clean value to db.
  inline_styles_cleaner_update_field_value($table_name, $column_name, $clean_value, $field_settings['field_instance']);
}

/**
 * Function for filtering array elements.
 *
 * @param string $element
 *   Array element.
 *
 * @return bool
 *   TRUE if element is not an empty string or string that contains only spaces.
 */
function inline_styles_cleaner_filter_spaced_strings($element) {
  return ((bool) strlen(trim($element)));
}

/**
 * Function for updating array with css styles.
 *
 * @param array $styles
 *   Array with css styles.
 *
 * @return array
 *   Array with updated styles.
 */
function inline_styles_cleaner_update_styles_array(array $styles) {
  $updated_styles = variable_get('inline_styles_cleaner_old_styles', array());

  foreach ($styles as $class_name => $property_values) {
    foreach ($property_values as $property_name => $value) {
      $updated_styles[$class_name][$property_name] = $value;
    }
  }

  variable_set('inline_styles_cleaner_old_styles', $updated_styles);

  return $updated_styles;
}
