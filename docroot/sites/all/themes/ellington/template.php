<?php
// $Id$
/**
 * @file
 *
 *
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com - http://www.kalman-hosszu.com
 */

/**
 * Implementation of hook_perprocess_html().
 *
 * Chcek search box, and a special class to body classes.
 *
 * @param array $vars
 */
function ellington_preprocess_html(&$vars) {
  if (!empty($vars['page']['search_box'])) {
    $vars['classes_array'][] = 'search-box';
  }

  // Add IE6 no more script ot not.
  $vars['ie6nomore'] = theme_get_setting('ellington_ie6nomore');
}

/**
 * Change breadcrumb simbol.
 *
 * @param array $breadcrumb
 * @return string
 */
function ellington_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    return implode('<span></span> ', $breadcrumb);
  }
}
