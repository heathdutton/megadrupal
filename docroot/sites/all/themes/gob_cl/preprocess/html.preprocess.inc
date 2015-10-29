<?php

/**
 * @file
 * Contains a pre-process hook for 'html'.
 */

/**
 * Override or insert variables into the html template
 */
function gob_cl_preprocess_html(&$variables) {
  // Code borrowed/adapted from D7 core.
  // Classes originally added by D7 core, then removed by Omega 4 and now put back (ish) by this function.
  // NOTE: D7 core used hyphens in class names, we need to use different classes so we're
  // replacing hyphens with underscores (make sure your CSS is expecting this).
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['classes_array'][] = 'has-two-sidebars has-sidebar-first has-sidebar-second';
  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['classes_array'][] = 'has-one-sidebar has-sidebar-first';
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['classes_array'][] = 'has-one-sidebar has-sidebar-second';
  }
  else {
    $variables['classes_array'][] = 'no-sidebars';
  }

  // Top Region
  if ($matches = preg_grep('/^top/', array_keys($variables['page']) ) ) {
    $count = count(array_intersect($matches, array_keys(array_filter($variables['page']))));

    $words = array('no', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine');

    if (isset($words[$count])) {
      $variables['attributes_array']['class'][] = "has-{$words[$count]}-top-region" . (($count !== 1) ? 's' : '');
    }

    foreach ($matches as $name) {
      if (!empty($variables['page'][$name])) {
        $variables['attributes_array']['class'][] = 'has-' . str_replace('_', '-', $name);
      }
    }
  }

  // Bottom Region
  if ($matches = preg_grep('/^bottom/', array_keys($variables['page']) ) ) {
    $count = count(array_intersect($matches, array_keys(array_filter($variables['page']))));

    $words = array('no', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine');

    if (isset($words[$count])) {
      $variables['attributes_array']['class'][] = "has-{$words[$count]}-bottom-region" . (($count !== 1) ? 's' : '');
    }

    foreach ($matches as $name) {
      if (!empty($variables['page'][$name])) {
        $variables['attributes_array']['class'][] = 'has-' . str_replace('_', '-', $name);
      }
    }
  }

  // Links Region
  if ($matches = preg_grep('/^links/', array_keys($variables['page']) ) ) {
    $count = count(array_intersect($matches, array_keys(array_filter($variables['page']))));

    $words = array('no', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine');

    if (isset($words[$count])) {
      $variables['attributes_array']['class'][] = "has-{$words[$count]}-links-region" . (($count !== 1) ? 's' : '');
    }

    foreach ($matches as $name) {
      if (!empty($variables['page'][$name])) {
        $variables['attributes_array']['class'][] = 'has-' . str_replace('_', '-', $name);
      }
    }
  }

}