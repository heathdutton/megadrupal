<?php

/**
 * @file
 * Hooks provided by the Taxonomy Tools module.
 */

/**
 * Define informative icons for specified taxonomy term.
 *
 * @param int $tid
 *   Taxonomy term identificator.
 *
 * @return array
 *   An associative array keyed by a unique name of the icon.
 *   Each icon is an associative array containing:
 *   - path: Either the path of the image file (relative to base_path()) or
 *     a full URL.
 *   - width: The width of the image (if known).
 *   - height: The height of the image (if known).
 *   - alt: The alternative text for text-based browsers. HTML 4 and XHTML 1.0
 *     always require an alt attribute. The HTML 5 draft allows the alt
 *     attribute to be omitted in some cases. Therefore, this variable defaults
 *     to an empty string, but can be set to NULL for the attribute to be
 *     omitted. Usually, neither omission nor an empty string satisfies
 *     accessibility requirements, so it is strongly encouraged for code calling
 *     theme('image') to pass a meaningful value for this variable.
 *     http://www.w3.org/TR/REC-html40/struct/objects.html#h-13.8
 *     http://www.w3.org/TR/xhtml1/dtds.html
 *     http://dev.w3.org/html5/spec/Overview.html#alt
 *   - title: The title text is displayed when the image is hovered in
 *     some popular browsers.
 *   - attributes: Associative array of attributes to be placed in the img tag.
 */
function hook_taxonomy_tools_overview_info_icons($tid) {
  $images = array();
  $images['sample_image'] = array(
    'path' => 'sample_image.png',
    'title' => 'Sample image',
    'alt' => 'sample image',
    'attributes' => array(),
  );
  return $images;
}

/**
 * Act on node access grants rebuild.
 *
 * @param stdClass $node
 *   Node object.
 */
function hook_taxonomy_tools_node_access_grants_rebuild($node) {
  drupal_set_message($node->title);
}

/**
 * Define action links for specified taxonomy term.
 *
 * @param int $tid
 *   Taxonomy term identificator.
 *
 * @return array
 *   An array containing any custom links.
 */
function hook_taxonomy_tools_overview_links($tid) {
  $links = array();
  $links['sample_link'] = array(
    '#type' => 'link',
    '#title' => 'sample link',
    '#href' => '<front>',
    '#attributes' => array(
      'class' => array(
        'sample-link',
      ),
      'title' => 'sample link',
    ),
    '#options' => array(),
  );
  return $links;
}
