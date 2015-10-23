<?php

define('HOTBLOCKS_WYSIWYG_REGEX', '/<img.*?data-hotblocks\s*?=\s*?"(.*?)".*?>/');

/**
 * Implements hook_filter_info().
 *
 */
function hotblocks_wysiwyg_filter_info() {
  $filters['hotblocks_wysiwyg_filter'] = array(
    'title' => t('Hotblocks WYSIWYG filter'),
    'description' => t('Allows users to insert content into WYSIWYG fields using the Hotblocks interface.'),
    'process callback'  => 'hotblocks_wysiwyg_filter_process',
    'default settings' => array(),
    'tips callback' => 'hotblocks_wysiwyg_filter_tips',
  );

  return $filters;
}

/**
 * Implements hook_filter_tips
 */
function hotblocks_wysiwyg_filter_tips($filter, $format, $long) {
  $text = t('Insert content into WYSIWYG fields using the Hotblocks interface.');

  if ($long) {
    return $text;
  }
  else {
    return $text;
  }
}

/**
 * Process callback for hotblocks_wysiwyg_filter
 */
function hotblocks_wysiwyg_filter_process($text, $filter, $format, $langcode, $cache, $cache_id) {
  $text = preg_replace_callback(HOTBLOCKS_WYSIWYG_REGEX, 'hotblocks_wysiwyg_filter_process_helper', $text);
  return $text;
}

function hotblocks_wysiwyg_filter_process_helper($matches) {
  if (sizeof($matches) > 1) {
    // Try to get a hotblock_item object from the data-hotblocks HTML attribute, which is in $matches[1]
    $hotblock_item = hotblocks_wysiwyg_filter_parse_wysiwyg_token($matches[1]);

    // If we couldn't parse the data attribute, strip out the image entirely and return.
    if(empty($hotblock_item)) {
      return '';
    }

    $item_view = hotblocks_item_view($hotblock_item);

    // Remove the item controls
    unset($item_view['controls']);
    return '<div class="hotblock-item-wysiwyg">' . render($item_view) . '</div>';
  }
}

/**
 * Parses the data-hotblocks HTML attribute that's encoded by our WYSIWYG plugin
 *
 * @param $data_hotblocks_attr
 * - A raw, double html entity encoded attribute from the data-hotblocks attribute found on the WYSIWYG img tag
 * @return bool|object
 * - False if the data can't be parsed, otherwise returns on object with title, entity_type, and entity_id attributes.
 */
function hotblocks_wysiwyg_filter_parse_wysiwyg_token($data_hotblocks_attr) {
  // The string is double-encoded
  $string_decoded = html_entity_decode($data_hotblocks_attr, ENT_QUOTES, 'UTF-8');
  $string_decoded = html_entity_decode($string_decoded, ENT_QUOTES, 'UTF-8');

  // In order to prevent drupal_strip_dangerous_protocols() from breaking the content in this attribute, our javascript
  // plugin has prepended a '/', which causes Drupal to treat it as a relative path - aka a 'safe' protocol.
  $string_decoded = trim($string_decoded, '/');

  // At least, we have a pure json encoded string
  $data = json_decode($string_decoded);

  // If we couldn't parse the data attribute, strip out the image entirely and return.
  if(empty($data)) {
    return FALSE;
  }

  // todo - consider $hotblock_item->hid - what should it be?
  return (object) array(
    'title'       => $data->title,
    'entity_type' => $data->entity_type,
    'entity_id'   => $data->entity_id,
  );
}