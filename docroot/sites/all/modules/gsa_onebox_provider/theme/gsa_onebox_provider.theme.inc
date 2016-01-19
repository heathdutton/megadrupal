<?php

/**
 * @file
 * Theme related functions for processing our output style plugins.
 *
 * Views bug: http://drupal.org/node/593336
 */


/**
 * Theme a status message.
 */
function theme_gsa_onebox_provider_message($var) {
  $output = '';
  $output .= '<div class="messages status ' . $var['type'] . '">';
  $output .= $var['message'];
  $output .= '</div>';
  return $output;
}

/**
 * Theme a feed link.
 *
 * This theme function uses the theme pattern system to allow it to be
 * overidden in a more specific manner. The options for overiding this include
 * providing per display id; per type; per display id and per type.
 * 
 * @ingroup themeable
 */
function theme_gsa_onebox_provider_feed_icon($variables) {
  extract($variables, EXTR_SKIP);
  $url_options = array('html' => TRUE);
  if ($query) {
    $url_options['query'] = $query;
  }
  $image_array = array('path' => $image_path, 'alt' => $text, 'title' => $text);
  $image = theme('image', $image_array);
  return l($image, $url, $url_options);
}

/**
 * Template preprocess function.
 */
function template_preprocess_gsa_onebox_provider(&$vars) {
  $vars['header'] = $vars['rows']['header'];
  $vars['body'] = $vars['rows']['body'];
  $vars['footer'] = $vars['rows']['footer'];

  $view     = $vars['view'];
  $fields   = &$view->field;
}

/**
 * Preprocess txt output template.
 */
function template_preprocess_gsa_onebox_provider_txt_body(&$vars) {
  _gsa_onebox_provider_header_shared_preprocess($vars);
  _gsa_onebox_provider_body_shared_preprocess($vars);
}

/**
 * Preprocess xml output template.
 */
function template_preprocess_gsa_onebox_provider_xml_header(&$vars) {
  // Compute the root XML node, using the base table, and appending an 's'.
  $root_node = $vars['view']->base_table;
  if (rtrim($root_node, 's') == $root_node) {
    $root_node .= 's';
  }
  $vars['root_node'] = _gsa_onebox_provider_xml_tag_clean($root_node);

  if ($vars['options']['datasource']) {
    $vars['datasource'] = $vars['options']['datasource'];
  }

  if ($vars['options']['feedtype']) {
    switch ($vars['options']['feedtype']) {
      case 'meta':
        $vars['feedtype'] = 'metadata-and-url';
        break;

    }
  }
}

/**
 * Preprocess xml output template.
 */
function template_preprocess_gsa_onebox_provider_xml_footer(&$vars) {
  // Compute the root XML node, using the base table, and appending an 's'.
  $root_node = $vars['view']->base_table;
  if (rtrim($root_node, 's') == $root_node) {
    $root_node .= 's';
  }
  $vars['root_node'] = _gsa_onebox_provider_xml_tag_clean($root_node);
}

/**
 * Preprocess xml output template.
 */
function template_preprocess_gsa_onebox_provider_xml_body(&$vars) {
  _gsa_onebox_provider_header_shared_preprocess($vars);
  _gsa_onebox_provider_body_shared_preprocess($vars);

  // Compute the tag name based on the views base table, minus any trailing 's'.
  $vars['item_node'] = _gsa_onebox_provider_xml_tag_clean(rtrim($vars['view']->base_table, 's'));

  $options = array();
  $options['absolute'] = TRUE;
  $vars['record_url'] = array();
  foreach ($vars['themed_rows'] as $num => $row) {
    $this_url = 'node/' . $vars['view']->result[$num]->nid;
    $vars['record_url'][$num] = url($this_url, $options);
    foreach ($row as $field => $content) {
      // Prevent double encoding of the ampersand.
      $content = preg_replace('/&(?!(amp|quot|#039|lt|gt);)/', '&amp;', $content);

      // If it isn't properly utf-8 encoded, don't include the value.
      if (utf8_encode(utf8_decode($content)) != $content) {
        watchdog('php', "Node %nid GSA Onebox Provider content improperly encoded (not utf-8) on field %field. Field content was thrown out.", array('%field' => $field, '%nid' => $vars['view']->result[$num]->nid), WATCHDOG_WARNING);
        $content = '';
      }
      // Convert < and > to HTML entities.
      $content = str_replace(
        array('<', '>'),
        array('&lt;', '&gt;'),
        $content);
      $vars['themed_rows'][$num][$field] = $content;
    }
  }
  drupal_alter('gsa_onebox_provider_url_content', $vars['record_url'], $vars);

  foreach ($vars['header'] as $field => $header) {
    // If there is no field label, use 'no name'.
    $vars['xml_tag'][$field] = !empty($header) ? $header : 'no name';
    if ($vars['options']['transform']) {
      switch ($vars['options']['transform_type']) {
        case 'dash':
          $vars['xml_tag'][$field] = str_replace(' ', '-', $header);
          break;

        case 'underline':
          $vars['xml_tag'][$field] = str_replace(' ', '_', $header);
          break;

        case 'camel':
          $vars['xml_tag'][$field] = str_replace(' ', '', ucwords(strtolower($header)));
          // Convert the very first character of the string to lowercase.
          $vars['xml_tag'][$field][0] = strtolower($vars['xml_tag'][$field][0]);
          break;

        case 'pascal':
          $vars['xml_tag'][$field] = str_replace(' ', '', ucwords(strtolower($header)));
          break;

      }
    }
    // We should always try to output valid XML.
    $vars['xml_tag'][$field] = _gsa_onebox_provider_xml_tag_clean($vars['xml_tag'][$field]);
  }
}

/**
 * Returns a valid XML tag formed from the given input.
 *
 * @param string $tag
 *   The string that should be made into a valid XML tag.
 * 
 * @return string 
 *   The valid XML tag or an empty string if the string contained no valid
 * XML tag characters.
 */
function _gsa_onebox_provider_xml_tag_clean($tag) {

  // This regex matches characters that are not valid in XML tags, and the
  // unicode ones that are. We don't bother with unicode, because it would so
  // the preg_replace down a lot.
  static $invalid_tag_chars_regex = '#[^\:A-Za-z_\-.0-9]+#';

  // These characters are not valid at the start of an XML tag:
  static $invalid_start_chars = '-.0123456789';

  // Convert invalid chars to '-':
  $tag = preg_replace($invalid_tag_chars_regex, '-', $tag);

  // Need to trim invalid characters from the start of the string:
  $tag = ltrim($tag, $invalid_start_chars);

  return $tag;
}

/**
 * Shared helper function for preprocess functions.
 */
function _gsa_onebox_provider_header_shared_preprocess(&$vars) {
  $view   = $vars['view'];
  $fields = &$view->field;

  $vars['header'] = array();
  foreach ($fields as $key => $field) {
    if (empty($field->options['exclude'])) {
      $vars['header'][$key] = check_plain($field->label());
    }
  }

}

/**
 * Shared helper function for preprocess functions.
 */
function _gsa_onebox_provider_body_shared_preprocess(&$vars) {
  $view   = $vars['view'];
  $fields = &$view->field;

  $rows = $vars['rows'];

  $vars['themed_rows'] = array();
  $keys = array_keys($fields);
  foreach ($rows as $num => $row) {
    $vars['themed_rows'][$num] = array();

    foreach ($keys as $id) {
      if (empty($fields[$id]->options['exclude'])) {
        $vars['themed_rows'][$num][$id] = check_plain($view->style_plugin->rendered_fields[$num][$id]);
      }
    }
  }
  // Allow custom modules to alter the row data before it is printed.
  drupal_alter('gsa_onebox_provider_content', $vars['themed_rows'], $vars);
}
