<?php

/**
 * Create a dropdown menu.
 *
 * @param $title
 *   The text to place in the clickable area to activate the dropdown.
 * @param $links
 *   A list of links to provide within the dropdown, suitable for use
 *   in via Drupal's theme('links').
 * @param $image
 *   If true, the dropdown link is an image and will not get extra decorations
 *   that a text dropdown link will.
 * @param $class
 *   An optional class to add to the dropdown's container div to allow you
 *   to style a single dropdown however you like without interfering with
 *   other dropdowns.
 */
function twbs_ctools_dropdown($variables) {
  $link = array();
  $link['text'] = $variables['title'];
  $link['options']['html'] = $variables['image'];

  $links = array();
  $links['links'] = $variables['links'];

  $attributes = array();
  $attributes['class'][] = $variables['class'];

  return theme('twbs_bootstrap_dropdown', array(
    'link' => $link,
    'links' => $links,
    'attributes' => $attributes,
  ));
}
