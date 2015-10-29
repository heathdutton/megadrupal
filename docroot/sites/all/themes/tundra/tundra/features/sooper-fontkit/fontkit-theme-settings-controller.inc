<?php

if (substr(theme_get_setting('body_font_face'), 0, 1) === '0') {
  _tundra_add_goog_font(theme_get_setting('body_font_face'));
}
elseif (substr(theme_get_setting('body_font_face'), 0, 1) === '1') {
  _tundra_add_local_font(theme_get_setting('body_font_face'));
}

if (substr(theme_get_setting('headings_font_face'), 0, 1) === '0') {
  _tundra_add_goog_font(theme_get_setting('headings_font_face'));
}
elseif (substr(theme_get_setting('headings_font_face'), 0, 1) === '1') {
  _tundra_add_local_font(theme_get_setting('headings_font_face'));
}

if (substr(theme_get_setting('nav_font_face'), 0, 1) === '0') {
  _tundra_add_goog_font(theme_get_setting('nav_font_face'));
}
elseif (substr(theme_get_setting('nav_font_face'), 0, 1) === '1') {
  _tundra_add_local_font(theme_get_setting('nav_font_face'));
}

if (substr(theme_get_setting('sitename_font_face'), 0, 1) === '0') {
  _tundra_add_goog_font(theme_get_setting('sitename_font_face'));
}
elseif (substr(theme_get_setting('sitename_font_face'), 0, 1) === '1') {
  _tundra_add_local_font(theme_get_setting('sitename_font_face'));
}

/**
 * Helper function to add google fonts to the page
 */
function _tundra_add_goog_font($font) {
  $font = substr($font, 1);
  $element = array(
    '#tag' => 'link',
    '#attributes' => array(
      'href' => 'http://fonts.googleapis.com/css?family=' . $font,
      'rel' => 'stylesheet',
      'type' => 'text/css',
    ),
  );
  drupal_add_html_head($element, $font);
  return;
}

/**
 * Helper function to add local fonts to the page. Add query string to stylesheet to prevent
 * identically named files to overwrite eachother.
 */
function _tundra_add_local_font($font) {
  global $base_url, $base_path;

  $font = explode('|', substr($font, 1));
  $element = array(
    '#tag' => 'link',
    '#attributes' => array(
      'href' => $base_path . drupal_get_path('theme', $font[0]) . $font[1],
      'rel' => 'stylesheet',
      'type' => 'text/css',
    ),
  );
  drupal_add_html_head($element, $font[2]);
  return;
}
