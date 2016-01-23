<?php

/**
 * @file
 * Miscellaneous GlobalLink functions for node translations (non-entity).
 */

/**
 * Returns GlobalLink modules root menu.
 *
 * @param string $menu_item
 *   The current menu item.
 *
 * @return string
 *   String that represents menu item path.
 */
function globallink_get_root_menu($menu_item) {
  return 'admin/' . $menu_item;
}

/**
 * Builds a filter query.
 *
 * @param SelectQuery $query
 *   SelectQuery object.
 * @param array $filter_array
 *   Array of filter parameters.
 */
function transpefect_build_filter_query(SelectQuery $query, $filter_array) {
  $filters = globallink_build_filters(TRUE);

  $count = 0;

  foreach ($filter_array as $filter) {
    list($key, $value) = $filter;

    switch ($key) {
      case 'modified':
      case 'target_language':
        continue 2;

        break;
      case 'modified-after':
        $query->condition($filters[$key]['field'], $value, '>=');

        break;
      case 'title':
        $query->condition($filters[$key]['field'], '%' . $value . '%', 'LIKE');

        break;
      case 'status':
        if (module_exists('revisioning')) {
          continue 2;
        }

        list($key, $value) = explode('-', $value, 2);

        $query->condition($key, $value, '=');

        break;
      case 'node_parent':
        if ($value == 1) {
          $query->where('tnid = nid OR tnid = 0');
        }

        break;
      default:
        $query->condition($filters[$key]['field'], $value);
    }

    $count++;
  }
}

/**
 * Inserts child element into DOM.
 *
 * @param array $dom
 *   Array representation of the DOM.
 * @param array $root
 *   The root of the DOM.
 * @param string $elem_name
 *   The desired name of the element.
 * @param string $elem_value
 *   The desired value of the element.
 * @param array $attributes
 *   Array of desired attributes for the element.  Defaults to NULL.
 */
function globallink_insert_child_element($dom, $root, $elem_name, $elem_value, $attributes = NULL) {
  if ($elem_name && $elem_value) {
    $item = $dom->createElement($elem_name);

    if (isset($attributes) && is_array($attributes)) {
      foreach ($attributes as $key => $value) {
        $item->setAttribute($key, $value);
      }
    }

    $text = $dom->createTextNode($elem_value);
    $item->appendChild($text);
    $root->appendChild($item);
  }
}

/**
 * Builds filters.
 *
 * @param string $content_translation
 *   The content translation type.
 *
 * @return array
 *   Associative array of filters.
 */
function globallink_build_filters($content_translation) {
  $filters = array();

  $filters['modified'] = array(
    'title' => 'Show',
    'field' => 'changed',
    'form-type' => 'radios',
    'options' => array(
      '&nbsp;&nbsp;' . t('Modified Content') . '&nbsp;&nbsp;&nbsp;&nbsp;',
      '&nbsp;&nbsp;' . t('Everything') . '&nbsp;&nbsp;',
    ),
  );

  if (module_exists('revisioning')) {
    $filters['status'] = array(
      'title' => t('Status'),
      'options' => array(
        'status-1' => t('Latest Published Revision'),
        'status-0' => t('Latest Modified Revision'),
        'promote-1' => t('Promoted'),
        'promote-0' => t('Not Promoted'),
        'sticky-1' => t('Sticky'),
        'sticky-0' => t('Not Sticky'),
      ),
      'form-type' => 'select',
    );
  }
  else {
    $filters['status'] = array(
      'title' => t('Status'),
      'options' => array(
        '[any]' => t('Any'),
        'status-1' => t('Published'),
        'status-0' => t('Not Published'),
        'promote-1' => t('Promoted'),
        'promote-0' => t('Not Promoted'),
        'sticky-1' => t('Sticky'),
        'sticky-0' => t('Not Sticky'),
      ),
      'form-type' => 'select',
    );
  }

  $languages = globallink_get_mapped_drupal_locales(FALSE);

  foreach ($languages as $key => $lang) {
    $lang_filter[$key] = $lang;
  }

  $filters['language_name'] = array(
    'title' => t('Source Language'),
    'field' => 'language',
    'options' => $lang_filter,
    'form-type' => 'select',
  );

  $filters['node_parent'] = array(
    'title' => t('Parent Node'),
    'field' => 'node_parent',
    'form-type' => 'checkbox',
  );

  $filters['target_language'] = array(
    'title' => t('Target Language'),
    'field' => 'language',
    'options' => $lang_filter,
    'form-type' => 'select',
  );

  $n_arr = array('[any]' => t('Any'));
  $t_arr = globallink_get_translatable_node_types_and_names($content_translation);

  $node_types_filter = $n_arr + $t_arr;

  $filters['type'] = array(
    'title' => t('Content Type'),
    'field' => 'type',
    'options' => $node_types_filter,
    'form-type' => 'select',
  );

  $filters['title'] = array(
    'title' => 'Title',
    'field' => 'title',
    'form-type' => 'textfield',
  );

  $filters['modified-after'] = array(
    'title' => 'Modified After',
    'field' => 'changed',
    'form-type' => 'textfield',
  );

  return $filters;
}

/**
 * Gets translatable node types and names based on a content or entity translation.
 *
 * @param string $content_translation
 *   The content translation type.
 *
 * @return array
 *   Array of translatable node types and names.
 */
function globallink_get_translatable_node_types_and_names($content_translation = '') {
  $arr = array();

  $node_types = node_type_get_types();

  foreach ($node_types as $node_type) {
    if ($content_translation) {
      // Show only Node Translation (i18n) enabled content types.
      if (translation_supported_type($node_type->type)) {
        $arr[$node_type->type] = $node_type->name;
      }
    }
    elseif ($content_translation === FALSE) {
      // Show only Entity Translation enabled content types.
      if (module_exists('entity_translation') && entity_translation_node_supported_type($node_type->type)) {
        $arr[$node_type->type] = $node_type->name;
      }
    }
    else {
      // Show all translation enabled content types.
      if ((module_exists('entity_translation') && entity_translation_node_supported_type($node_type->type)) || translation_supported_type($node_type->type)) {
        $arr[$node_type->type] = $node_type->name;
      }
    }
  }

  asort($arr);

  return $arr;
}

/**
 * Gets locale name based on its code.
 *
 * @param string $locale_code
 *   The locale code.
 *
 * @return string
 *   The locale name.
 */
function globallink_get_locale_name($locale_code) {
  $result = db_select('globallink_locale', 'tl')
  ->fields('tl')
  ->condition('locale_code', $locale_code, '=')
  ->execute();

  $locale_name = '';

  foreach ($result as $row) {
    $locale_name = $row->locale_desc;
  }

  return $locale_name;
}

/**
 * Gets Drupal locale name based on its code.
 *
 * @param string $tpt_locale_code
 *   The locale code.
 *
 * @return string
 *   The locale name.
 */
function globallink_get_drupal_locale_name($tpt_locale_code) {
  $result = db_select('globallink_locale', 'tl')
  ->fields('tl')
  ->condition('locale_code', $tpt_locale_code, '=')
  ->execute();

  $locale_name = '';

  foreach ($result as $row) {
    $locale_name = $row->drupal_locale_desc;
  }

  return $locale_name;
}

/**
 * Gets sent TPT rows by node id.
 *
 * @param int $nid
 *   The node id.
 *
 * @return array
 *   Array of TPT rows.
 */
function globallink_get_sent_rows_by_nid($nid) {
  $result = db_select('globallink_core', 'tc')
  ->fields('tc')
  ->condition('nid', $nid, '=')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->execute();

  $rows = array();

  foreach ($result as $row) {
    $rows[] = $row;
  }

  return $rows;
}

/**
 * Gets GlobalLink row by id.
 *
 * @param int $row_id
 *   The id of the row.
 *
 * @return object
 *   The row.
 */
function globallink_get_row($row_id) {
  $result = db_select('globallink_core', 'tc')
  ->fields('tc')
  ->condition('rid', $row_id, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }
}

/**
 * Gets GlobalLink row by node id and locale.
 *
 * @param int $nid
 *   The node id.
 * @param string $source
 *   The locale.
 * @param string $target
 *   The target.
 *
 * @return object
 *   The row.
 */
function globallink_get_row_by_nid_and_locale($nid, $source, $target) {
  $result = db_select('globallink_core', 'tc')
  ->fields('tc')
  ->condition('nid', $nid, '=')
  ->condition('source', $source, '=')
  ->condition('target', $target, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }

  return FALSE;
}

/**
 * Gets node source languages by id.
 *
 * @param int $nid
 *   The node id.
 *
 * @return string
 *   The source language.
 */
function globallink_get_node_source_languages($nid) {
  $d_query = db_select('node', 'n');
  $d_query->condition('nid', $nid, '=');
  $d_query->join('languages', 'l', 'l.language = n.language');
  $d_query->fields('l', array('language'));

  $results = $d_query->execute();

  foreach ($results as $item) {
    return $item->language;
  }

  return '';
}

/**
 * Gets GlobalLink locale code.
 *
 * @param string $drupal_locale
 *   The Drupal locale code.
 *
 * @return string
 *   The GlobalLink locale code.
 */
function globallink_get_locale_code($drupal_locale) {
  $result = db_select('globallink_locale', 'tl')
  ->fields('tl')
  ->condition('drupal_locale_code', $drupal_locale, '=')
  ->execute();

  $locale_code = '';

  foreach ($result as $row) {
    $locale_code = $row->locale_code;
  }

  return $locale_code;
}

/**
 * Gets Drupal locale code.
 *
 * @param string $locale_code
 *   The locale code.
 *
 * @return string
 *   The Drupal locale code.
 */
function globallink_get_drupal_locale_code($locale_code) {
  $result = db_select('globallink_locale', 'tl')
  ->fields('tl')
  ->condition('locale_code', $locale_code, '=')
  ->execute();

  $drupal_code = '';

  foreach ($result as $row) {
    $drupal_code = $row->drupal_locale_code;
  }

  return $drupal_code;
}

/**
 * Updates node tnid.
 *
 * @param int $nid
 *   The node id.
 * @param int $tnid
 *   The node tid.
 */
function globallink_update_node_tnid($nid, $tnid) {
  db_update('node')
  ->fields(array('tnid' => $tnid))
  ->condition('nid', $nid, '=')
  ->execute();
}

/**
 * Converts date to timestamp.
 *
 * @param string $date
 *   Formatted date string in MM/DD/YY.
 *
 * @return int
 *   The date in milliseconds.
 */
function globallink_convert_date_to_timestamp($date) {
  list($month, $day, $year) = explode('/', $date);

  $seconds = mktime(0, 0, 0, $month, $day, $year);
  $due_date_in_milli_sec = $seconds * 1000;

  return $due_date_in_milli_sec;
}

/**
 * Checks for existence of node id object.
 *
 * @param object $globallink
 *   The GlobalLink object.
 *
 * @return
 *   True if the node exists.
 */
function globallink_get_nid(&$globallink) {
  $translated_arr = globallink_get_content_attributes_from_xml($globallink->targetXML);

  if (count($translated_arr) > 0) {
    $globallink->nid = isset($translated_arr['nid']) ? $translated_arr['nid'] : 0;
    $globallink->tptRowId = isset($translated_arr['rid']) ? $translated_arr['rid'] : 0;

    return TRUE;
  }

  return FALSE;
}

/**
 * Gets content attributes from XML input.
 *
 * @param string $xml
 *   The raw XML input.
 *
 * @return array
 *   Array of content attributes.
 */
function globallink_get_content_attributes_from_xml($xml) {
  if (is_null($xml) || !is_string($xml) || $xml == '') {
    return array();
  }

  $dom = new DomDocument;
  $dom->preserveWhiteSpace = FALSE;
  $dom->loadXML($xml);

  $arr = array();

  $contents = $dom->getElementsByTagName('content');

  foreach ($contents as $content) {
    if (is_null($content->attributes)) {
      continue;
    }

    foreach ($content->attributes as $attr_name => $attr_node) {
      switch ($attr_name) {
        case 'rid':
          $arr['rid'] = $attr_node->value;

          break;
        case 'nid':
          $arr['nid'] = $attr_node->value;

          break;
        case 'vid':
          $arr['vid'] = $attr_node->value;

          break;
      }
    }
  }

  return $arr;
}

/**
 * Splices the pager array.
 *
 * @param array $data
 *   Array of page data.
 * @param int $limit
 *   Page limit.  Defaults to 9.
 * @param int $element
 *   The current page.  Defaults to 0.
 *
 * @return array
 *   The sliced pager array.
 */
function globallink_pager_array_splice($data, $limit = 9, $element = 0) {
  global $pager_page_array, $pager_total, $pager_total_items;

  $page = isset($_GET['page']) ? $_GET['page'] : '';

  // Convert comma-separated $page to an array, used by other functions.
  $pager_page_array = explode(',', $page);

  // We calculate the total of pages as ceil(items / limit).
  $pager_total_items[$element] = count($data);
  $pager_total[$element] = ceil($pager_total_items[$element] / $limit);
  $pager_page_array[$element] = max(0, min((int) $pager_page_array[$element], ((int) $pager_total[$element]) - 1));

  return array_slice($data, $pager_page_array[$element] * $limit, $limit, TRUE);
}

/**
 * Formats display string.
 *
 * @param string $string
 *   The string to be formatted.
 *
 * @return string
 *   The formatted string.
 */
function globallink_format_display_string($string) {
  if (mb_strlen($string) > 25) {
    return mb_substr($string, 0, 25) . '...';
  }

  return $string;
}

/**
 * Formats file name.
 *
 * @param string $string
 *   The file name to be formatted.
 *
 * @return string
 *   The formatted file name.
 */
function globallink_format_file_name($string) {
  $bad = array_merge(array_map('chr', range(0, 31)), array('<', '>', ':', '"', '/', '\\', '|', '?', '*', ';', '&', ' ', '.'));
  $name = str_replace($bad, '_', utf8_decode(preg_replace('/[^(\x20-\x7F)]*/', '', $string)));

  if (strlen($name) > 50) {
    $name = substr($name, 0, 50);
  }

  return $name;
}

/**
 * Escapes JSON string.
 *
 * @param string $value
 *   The JSON string to be escaped.
 *
 * @return string
 *   The escaped JSON string.
 */
function globallink_escape_json_string($value) { // list from www.json.org: (\b backspace, \f formfeed)
  $escapers = array("\\", "/", "\"", "\n", "\r", "\t", "\x08", "\x0c");
  $replacements = array("\\\\", "\\/", "\\\"", "\\n", "\\r", "\\t", "\\f", "\\b");
  $result = str_replace($escapers, $replacements, $value);

  return $result;
}

/**
 * Checks to see if a pending submission exists.
 *
 * @return
 *   True if the submission exists.
 */
function globallink_pending_submission_exists() {
  $result = db_select('globallink_core', 'tc')
  ->fields('tc')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->execute();

  foreach ($result as $row) {
    if (isset($row)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Checks to see if a pending submission exists for a specified content type.
 *
 * @param string $type
 *   The content type.
 *
 * @return
 *   True if the submission exists.
 */
function globallink_pending_submission_exists_for_content_type($type) {
  $result = db_select('globallink_core', 'tc')
  ->fields('tc')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->condition('type', $type, '=')
  ->execute();

  foreach ($result as $row) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Gets mapped Drupal locales.
 *
 * @param bool $remove_default
 *   Whether or not to remove the default locale.  Defaults to true.
 *
 * @return array
 *   Array of locales.
 */
function globallink_get_mapped_drupal_locales($remove_default = TRUE) {
  $languages = array();

  $result = db_select('globallink_locale', 'tl')
  ->fields('tl')
  ->isNotNull('drupal_locale_code')
  ->orderBy('locale_desc', 'ASC')
  ->execute();

  foreach ($result as $row) {
    if ($remove_default) {
      if (language_default()->language == $row->drupal_locale_code) {
        continue;
      }
    }

    $languages[$row->drupal_locale_code] = $row->drupal_locale_desc;
  }

  return $languages;
}

/**
 * Gets config fields by content type.
 *
 * @param string $content_type
 *   The desired content type.
 *
 * @return array
 *   Array of config fields.
 */
function globallink_get_config_fields($content_type) {
  $arr = array();

  $result = db_select('globallink_field_config', 'tf')
  ->fields('tf')
  ->condition('content_type', $content_type, '=')
  ->execute();

  foreach ($result as $row) {
    $arr[$row->field_name] = $row;
  }

  return $arr;
}

/**
 * Gets non-translatable config fields by content type.
 *
 * @param string $content_type
 *   The desired content type.
 *
 * @return array
 *   Array of non-translatable config fields.
 */
function globallink_get_non_translatable_config_fields($content_type) {
  $arr = array();

  $result = db_select('globallink_field_config', 'tf')
  ->fields('tf')
  ->condition('content_type', $content_type, '=')
  ->condition('translatable', 0, '=')
  ->execute();

  foreach ($result as $row) {
    $arr[$row->field_name] = $row;
  }

  return $arr;
}

/**
 * Gets pending fields by content type.
 *
 * @param string $content_type
 *   The desired content type.
 *
 * @return array
 *   Array of pending fields.
 */
function globallink_get_pending_fields($content_type) {
  $config_fields = globallink_get_config_fields($content_type);

  $field_info_arr = field_info_instances('node', $content_type);
  $field_info_arr[] = array('field_name' => 'title', 'label' => 'Title');

  if (module_exists('metatag')) {
    $field_info_arr[] = array('field_name' => 'metatags', 'label' => 'Meta tags');
  }

  $field_arr = array();

  foreach ($field_info_arr as $field_info) {
    $field_name = $field_info['field_name'];

    if ($field_name != 'metatag' && $field_name != 'title') {
      $field = field_info_field($field_name);

      if ($field['type'] != 'list_boolean' && $field['type'] != 'file' && $field['type'] != 'taxonomy_term_reference') {
        if (!isset($config_fields[$field_name])) {
          $field_arr[$field_name] = $field_info['label'];
        }
      }
    }
    else {
      if (!isset($config_fields[$field_name])) {
        $field_arr[$field_name] = $field_info['label'];
      }
    }
  }

  asort($field_arr);

  return $field_arr;
}

/**
 * Gets translatable node types.
 *
 * @param bool $content_translation
 *   Whether or not to translate the content.
 *
 * @return array
 *   Array of translatable node types.
 */
function globallink_get_translatable_node_types($content_translation) {
  $arr = array();

  $node_types = node_type_get_types();

  $entity_translation_enabled = module_exists('entity_translation');

  foreach ($node_types as $node_type) {
    if (($entity_translation_enabled && entity_translation_node_supported_type($node_type->type)) || translation_supported_type($node_type->type)) {
      $arr[] = $node_type->type;
    }
  }

  return $arr;
}

/**
 * Gets parent bundle name of field.
 *
 * @param string $entity_type
 *   The field's entity type.
 * @param string $content_type
 *   The field's content type.
 * @param string $field_name
 *   The field's name.
 *
 * @return string
 *   The parent bundle name.
 */
function globallink_get_parent_bundle_name($entity_type, $content_type, $field_name) {
  $bundle = '';

  $result = db_select('globallink_field_config', 'tf')
  ->fields('tf')
  ->condition('content_type', $content_type, '=')
  ->condition('entity_type', $entity_type, '=')
  ->condition('field_name', $field_name, '=')
  ->execute();

  foreach ($result as $row) {
    $bundle = $row->bundle;
  }

  return $bundle;
}

/**
 * Determines whether or not a field is configured for translation.
 *
 * @param string $entity_type
 *   The field's entity type.
 * @param string $bundle_name
 *   The name of the bundle.
 * @param string $field_name
 *   The field's name.
 * @param string $content_type
 *   The field's content type.
 *
 * @return
 *   True if the field is configured for translation.
 */
function globallink_is_field_configured_for_translation($entity_type, $bundle_name, $field_name, $content_type) {
  $result = db_select('globallink_field_config', 'tf')
  ->fields('tf')
  ->condition('content_type', $content_type, '=')
  ->condition('entity_type', $entity_type, '=')
  ->condition('bundle', $bundle_name, '=')
  ->condition('field_name', $field_name, '=')
  ->execute();

  foreach ($result as $row) {
    if ($row->translatable == 1) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Determines whether or not a string begins with a certain substring.
 *
 * @param string $haystack
 *   The haystack.
 * @param string $needle
 *   The needle.
 *
 * @return int
 *   @see strncmp()
 */
function globallink_starts_with($haystack, $needle) {
  return !strncmp($haystack, $needle, strlen($needle));
}

/**
 * Gets enabled Drupal locales.
 *
 * @return array
 *   Array of enabled Drupal locales.
 */
function globallink_get_drupal_locales() {
  $languages = array();

  $result = db_select('languages', 'l')
  ->fields('l')
  ->condition('enabled', '1', '=')
  ->execute();

  foreach ($result as $row) {
    $languages[$row->language] = $row->name;
  }

  return $languages;
}

/**
 * @todo Document this function.
 */
function globallink_get_mapped_locales_with_drupal_desc($remove_default = TRUE) {
  $languages = array();

  $result = db_select('globallink_locale', 'tl')
  ->fields('tl')
  ->isNotNull('drupal_locale_code')
  ->orderBy('locale_desc', 'ASC')
  ->execute();

  foreach ($result as $row) {
    if ($remove_default) {
      if (language_default()->language == $row->drupal_locale_code) {
        continue;
      }
    }

    $languages[$row->locale_code] = $row->drupal_locale_desc;
  }

  return $languages;
}

/**
 * Builds array out of search criteria specified in request variables.
 *
 * @return array
 *   Array of filters.
 */
function globallink_drupal_search_query() {
  $filters = array();

  drupal_static_reset('language_list');
  $languages = locale_language_list('name');

  unset($languages['en']);

  $filters['string'] = array(
    'title' => t('String contains'),
    'description' => t('Leave blank to show all strings. The filter is case sensitive.'),
  );

  $filters['language'] = array(
    'title' => t('Language'),
    'options' => array_merge(array(
      'all' => t('All languages'),
      'en' => t('English (provided by Drupal)'),
    ), $languages),
  );

  $filters['translation'] = array(
    'title' => t('Filter in'),
    'options' => array(
      'all' => t('Both translated and untranslated strings'),
      'translated' => t('Only translated strings'),
      'untranslated' => t('Only untranslated strings'),
    ),
  );

  return $filters;
}

/**
 * @todo Document this function.
 */
function globallink_get_source($group) {
  $result = db_query('SELECT lid, source, context, location FROM {locales_source} WHERE textgroup = :textgroup', array(
    ':textgroup' => $group,
  ));

  $strings = array();

  foreach ($result as $child) {
    $string = array(
      'lid' => $child->lid,
      'location' => $child->location,
      'source' => $child->source,
      'context' => $child->context,
    );

    $strings[$child->lid] = $string;
  }

  return $strings;
}

/**
 * Loads source data.
 *
 * @param string $lid
 *   The lid.
 *
 * @return array
 *   Array of source data.
 */
function globallink_load_source_data($lid) {
  $results = db_select('locales_source', 'ls')
  ->fields('ls')
  ->condition('lid', $lid, '=')
  ->execute();

  $rows = array();

  foreach ($results as $item) {
    $rows[] = $item;
  }

  return $rows;
}

/**
 * Gets other row.
 *
 * @param int $row_id
 *   The id of the row.
 * @param string $type
 *   The type of GlobalLink core.
 *
 * @return object
 *   Other row.
 */
function globallink_get_other_row($row_id, $type) {
  $result = db_select('globallink_core_' . $type, 'tc')
  ->fields('tc')
  ->condition('rid', $row_id, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }
}

/**
 * Gets project director projects.
 *
 * @return array
 *   Array of project director projects.
 */
function globallink_get_pd_projects() {
  $proj_arr = variable_get('globallink_pd_projects', array());

  if (count($proj_arr) == 0) {
    return;
  }

  $projects = variable_get('globallink_pd_projectid');

  if (empty($projects)) {
    return;
  }

  $arr = explode(',', $projects);

  foreach ($arr as $value) {
    if (isset($proj_arr[$value])) {
      $proj_arr[$value] = $proj_arr[$value];
    }
    else {
      $proj_arr[$value] = $value;
    }
  }

  return $proj_arr;
}

/**
 * Determines if the workbench is enabled for a content type.
 *
 * @param string $type
 *   The content type.
 *
 * @return
 *   True if the workbench is enabled for that content type.
 */
function globallink_content_type_workbench_enabled($type) {
  if (!module_exists('workbench_moderation')) {
    return FALSE;
  }
  elseif (!$variable = variable_get('node_options_' . $type, FALSE)) {
    return FALSE;
  }
  elseif (!in_array('moderation', $variable)) {
    return FALSE;
  }

  return TRUE;
}
