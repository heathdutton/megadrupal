<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega)
 * provides all the basic functionality. However, in case you wish to customize
 * the output that Drupal generates through Alpha & Omega.
 * this file is a good place to do so.
 * Alpha comes with a neat solution for keeping this file as clean as possible
 * while the code for your subtheme grows.
 * Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */

/**
 * Implements hook_preprocess_node().
 */
function woodsman_preprocess_node(&$vars) {
  if ($vars['is_front'] == TRUE) {
    $vars['date'] = '<div class="day">' . format_date($vars['node']->created, 'custom', 'j') . '</div>' . ' ' . $vars['date'] = '<div class="month">' . format_date($vars['node']->created, 'custom', 'M') . '</div>';
  }

}

/**
 * Implements hook_form_alter().
 */
function woodsman_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['actions']['#attributes']['class'][] = 'element-invisible';
  }
}

/**
 * Implements hook_preprocess_page().
 */
function woodsman_preprocess_page(&$vars) {
  // Load jQuery Example if it is available .
  if (module_exists('libraries')) {
    libraries_load('jquery_example');
  }

  // Configure the menu .
  $logo_path = theme_get_setting('logo_path');
  $logo_info = image_get_info($logo_path);
  $css = 'ul.menu li:first-child + li { margin-right: ' . $logo_info['width'] . 'px;}';
  drupal_add_css($css, array('type' => 'inline'));

  // Modify the page title if we are in an archive view .
  if (module_exists('views')) {
    $view = views_get_page_view();
    if ($view && $view->name == 'archive') {
      drupal_set_title(t('Archive:') . ' ' . drupal_get_title());
    }
  }
}

/**
 * Implements hook_alpha_preprocess_region().
 */
function woodsman_alpha_preprocess_region(&$vars) {
  $theme = alpha_get_theme();
  if ($vars['elements']['#region'] == 'content') {
    $vars['breadcrumb'] = $theme->page['breadcrumb'];
    if (module_exists('views')) {
      $view = views_get_page_view();
      if ($view && $view->name == 'categories') {
        $vars['article_count'] = format_plural($view->total_rows, '@count article found in this category', '@count articles in this category');
      }
    }
  }
}

/**
 * Implements hook_libraries_info().
 */
function woodsman_libraries_info() {
  $libraries['jquery_example'] = array(
    'name' => 'JQuery example',
    'vendor url' => 'http://mudge.name/jquery_example',
    'download url' => 'https://github.com/mudge/jquery_example',
    'version arguments' => array(
      'file' => 'README.markdown',
      'pattern' => '/jQuery Form Example Plugin (\d+)/',
      'lines' => 5,
    ),
    'files' => array(
      'js' => array('jquery.example.min.js'),
    ),
  );
  return $libraries;
}
