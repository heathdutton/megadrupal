<?php
/**
 * @file
 * Provides encoding of URLs in fields being saved based on settings.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implements hook_field_attach_presave().
 *
 * Encode the file URLs on a field if it is included in the settings and the
 * selected format includes any of our filters.
 */
function _portable_path_field_attach_presave($entity_type, $entity) {
  // Get information.
  $info = entity_get_info($entity_type);
  $formats = filter_formats();
  $settings = portable_path_config();
  $filter_info = portable_path_filter_info();

  $bundle_key = $info['entity keys']['bundle'];
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
      $language = current(array_keys($entity->$field_name));
      if (empty($entity->{$field_name}[$language])) {
        // Field is not set, i.e., no value.
        continue;
      }
      $format = $entity->{$field_name}[$language][0]['format'];
      $filters = filter_list_format($format);
      // Which of our filters is active in the format and in what order?
      $our_filters = array_filter($filters, 'portable_path_array_filter');

      if (empty($our_filters)) {
        continue;
      }

      if (!empty($entity->$field_name) && !empty($entity->{$field_name}[$language][0][$column])) {
        foreach (array_keys($our_filters) as $key) {
          $function = $filter_info[$key]['encode callback'];
          if (function_exists($function)) {
            $filter = $our_filters[$key];
            $entity->{$field_name}[$language][0][$column] = $function($entity->{$field_name}[$language][0][$column], $filter, $settings, $entity);
          }
        }
      }
    }
  }
}

/**
 * Returns text with all URLs encoded as a file ID token.
 *
 * @param string $text
 *   The text to encode.
 * @param array $filter
 *   The filter settings.
 * @param array $settings
 *   The configuration settings for this module.
 * @param object $entity
 *   The entity with a field containing the text to encode.
 *
 * @return string
 *   The encoded text.
 *
 * Examples
 * <img src="http://example.com/sites/default/files/styles/medium/public/portable/image.jpg?itok=LKVqJ_PC" width="220" height="75" alt="" class="image-medium" />
 * <a href="/sites/default/files/portable/file.pdf?itok=LKVqJ_PC">file.pdf</a>
 */
function portable_path_file_id_encode($text, $filter, $settings, $entity) {
  return preg_replace_callback('@\s+(href|src)\s*=(\'|")(.*?)(\'|")@', 'portable_path_file_id_encode_callback', $text);
}

/**
 * Returns text with all URLs encoded as a file ID token.
 *
 * Callback for preg_replace_callback() within portable_path_file_id_encode().
 *
 * @param array $matches
 *   Chunk of text containing a file path to encode as a file ID. If the path
 *   is to an image styles directory, then the style will be included in the
 *   encoding.
 *
 * @return string
 *   The encoded text.
 */
function portable_path_file_id_encode_callback($matches) {
  if (!isset($matches[3])) {
    return $matches[0];
  }

  $style = $query = '';
  list(, $property, $quote, $url, ) = $matches;
  if (strpos($url, '?') !== FALSE) {
    // Remove any query string and fragment from the URL.
    list($url, $query) = explode('?', $url);
  }

  $found = FALSE;
  $settings = portable_path_config();
  foreach (array_filter($settings['stream_wrappers']) as $scheme) {
    if ($wrapper = file_stream_wrapper_get_instance_by_scheme($scheme)) {
      // Try the absolute URL.
      $path = $wrapper->getExternalUrl();
      if (strpos($url, $path) === 0) {
        $url = ltrim(str_replace($path, '', $url), '/');
        $found = TRUE;
        break;
      }
      // Try the relative URL.
      $path = str_replace($GLOBALS['base_root'], '', $path);
      if (strpos($url, $path) === 0) {
        $url = ltrim(str_replace($path, '', $url), '/');
        $found = TRUE;
        break;
      }
    }
  }

  if (!$found) {
    return $matches[0];
  }

  if (strpos($url, 'styles/') === 0) {
    // Path is in the styles subdirectory.
    $parts = explode('/', $url, 4);
    if (count($parts) == 4) {
      // Path is in a style subdirectory.
      list(, $style, $scheme2, $remainder) = $parts;
      $url = $remainder;
    }
  }

  $file = file_load_uri($scheme . '://' . $url);
  if ($file) {
    $token = "[portable:{$file->fid}:$style:$query]";
    $new = " $property=$quote$token$quote";
    return $new;
  }

  return $matches[0];
}

/**
 * Loads a single file object from the database based on URI.
 *
 * @param string $uri
 *   A file URI.
 *
 * @return
 *   An object representing the file, or FALSE if the file was not found.
 */
function file_load_uri($uri) {
  return db_select('file_managed', 'f')
    ->fields('f', array('fid', 'uid', 'filename', 'uri', 'filemime', 'filesize', 'status', 'timestamp'))
    ->condition('uri', rawurldecode($uri))
    ->condition('status', 1) // Is this needed?
    ->execute()
    ->fetch();
}

/**
 * Returns text with all URLs encoded using stream wrapper notation.
 *
 * @todo How to ignore literal text with a path? Use preg_replace_callback()?
 *
 * @param string $text
 *   The text to encode.
 * @param array $filter
 *   The filter settings.
 * @param array $settings
 *   The configuration settings for this module.
 * @param object $entity
 *   The entity with a field containing the text to encode.
 *
 * @return string
 *   The encoded text.
 */
function portable_path_stream_wrapper_encode($text, $filter, $settings, $entity) {
  // @todo Is a simple replace sufficient or are there other use cases that
  // require more refined search conditions?
  foreach (array_filter($settings['stream_wrappers']) as $scheme) {
    if ($wrapper = file_stream_wrapper_get_instance_by_scheme($scheme)) {
      // Replace absolute URL.
      $path = $wrapper->getExternalUrl();
      $text = str_replace($path, $scheme . '://', $text);
      // Replace relative URL.
      $path = str_replace($GLOBALS['base_root'], '', $path);
      $text = str_replace($path, $scheme . '://', $text);
    }
  }
  return $text;
}
