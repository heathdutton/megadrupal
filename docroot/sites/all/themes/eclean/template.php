<?php

/**
 * Override or insert variables into the html template.
 */
function eclean_preprocess_html(&$variables) {
  // Hook into color.module
//  if (module_exists('color')) {
//    _color_html_alter($variables);
//  }
  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));

  $variables['rdf'] = new stdClass;

  if (module_exists('rdf')) {
    $variables['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN">' . "\n";
    $variables['rdf']->version = ' version="HTML+RDFa 1.0"';
    $variables['rdf']->namespaces = $variables['rdf_namespaces'];
    $variables['rdf']->profile = ' profile="' . $variables['grddl_profile'] . '"';
  }
  else {
    $variables['doctype'] = '<!DOCTYPE html>' . "\n";
    $variables['rdf']->version = '';
    $variables['rdf']->namespaces = '';
    $variables['rdf']->profile = '';
  }

  $meta_viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0'
    )
  );
  drupal_add_html_head($meta_viewport, 'viewport');

  // Remove the sidebars if the is set to do it in the admin pages
  $show_sidebars = theme_get_setting('eclean_admin_show_sidebars');
  if ((!$show_sidebars) && (arg(0) == 'admin')) {
    $pos = array_search('two-sidebars', $variables['classes_array']);
    unset($variables['classes_array'][$pos]);
  }
  // Script that make media query work on Internet Explorer 6, 7 and 8
  drupal_add_js('http://css3-mediaqueries-js.googlecode.com/files/css3-mediaqueries.js', 'external');
}

/**
 * Override or insert variables into the page template.
 */
function eclean_preprocess_page(&$variables) {
  // Prepare header.
  $site_fields = array();
  if (!empty($variables['site_name'])) {
    $site_fields[] = $variables['site_name'];
  }
  if (!empty($variables['site_slogan'])) {
    $site_fields[] = $variables['site_slogan'];
  }
  $variables['site_title'] = implode(' ', $site_fields);
  if (!empty($site_fields)) {
    $site_fields[0] = '<span>' . $site_fields[0] . '</span>';
  }
  $variables['site_html'] = implode(' ', $site_fields);

  // Set a variable for the site name title and logo alt attributes text.
  $slogan_text = $variables['site_slogan'];
  $site_name_text = $variables['site_name'];
  $variables['site_name_and_slogan'] = $site_name_text . ' ' . $slogan_text;

  // Remove the sidebars if a admin page is been displayed
  $show_sidebars = theme_get_setting('eclean_admin_show_sidebars');
  if ((!$show_sidebars) && (arg(0) == 'admin')) {
    $variables['page']['sidebar_first'] = array();
    $variables['page']['sidebar_second'] = array();
  }
}

/**
 * Override or insert variables into the node template.
 */
function eclean_preprocess_node(&$variables) {
  if (isset($variables['name'])) {
    $variables['author'] = t('by') . ' ' . $variables['name'];
  }
  $variables['submitted_date'] = date('M d Y, H:i:s', $variables['created']);
}

/**
 * Override or insert variables into the user picture template.
 */
function eclean_preprocess_user_picture(&$variables) {
  // Show default avatar if the user has no profile picture
  if (!isset($variables['account']->picture) || !$variables['account']->picture) {
    $path = path_to_theme();
    $variables['user_picture'] = theme('image', array('path' => $path . '/img/avatar.png', 'alt' => 'test', 'title' => 'test'));
  }
}

/**
 * Override or insert variables into the comment template.
 */
function eclean_preprocess_comment(&$variables) {
  if ($variables['author']) {
    $variables['author'] = t('by') . ' ' . $variables['author'];
  }
  $variables['submitted_date'] = date('M d Y, H:i:s', $variables['elements']['#comment']->created);
}

/**
 * Override theme_breadcrumb().
 */
function eclean_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  // Breadcrumb is set ONLY if there is more than 1 element in it
  if (count($breadcrumb) <= 1) {
    $breadcrumb = '';
  }
  if (!empty($breadcrumb)) {
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $output .= '<div class="breadcrumb">' . implode('&nbsp;&nbsp;›&nbsp;&nbsp;', $breadcrumb) . '</div>';
    return $output;
  }
}

