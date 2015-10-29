<?php

/**
 * @file
 * Theme template.php file.
 */

/**
 * Implements hook_preprocess_page().
 *
 * @see page.tpl.php
 */
function CHILDTHEME_preprocess_page(&$vars) {
  // Add information about the number of sidebars.
  if (!empty($vars['page']['sidebar_first']) &&
    !empty($vars['page']['sidebar_second'])) {
    $vars['content_column_class'] = 'col-sm-6 ';
  }
  elseif (!empty($vars['page']['sidebar_first']) ||
    !empty($vars['page']['sidebar_second'])) {
    $vars['content_column_class'] = 'col-sm-9 ';
  }
  else {
    $vars['content_column_class'] = 'col-sm-12 ';
  }
}

/**
 * Implements template_preprocess().
 *
 * Uncomment to strip out all default classes.
 * Remove all default classes.
 */

/*
function CHILDTHEME_preprocess(&$vars, $hook) {
  // Truncate the classes array for the following.
  $truncate_classes = array(
    'page',
    'node',
    'region',
    'block',
    'field',
    'comment'
  );

  if (in_array($hook, $truncate_classes)) {
    // Empty the classes array to get rid of default classes.
    array_splice($vars['classes_array'], 0);
  }
  // Strip classes from HTML if they exist.
  $html_class = array('html');
  $strip_classes = array(
    'html',
    'no-sidebars',
    '-node',
    '-node-',
    '-logged-',
    'toolbar',
    'toolbar-drawer'
  );

  if (in_array($hook, $html_class)) {
    foreach ($strip_classes as $class) {
      foreach ($vars['classes_array'] as $key => $value) {
        if (strpos($vars['classes_array'][$key], $class) !== FALSE) {
          unset($vars['classes_array'][$key]);
        }
      }
    }
  }
}
*/
