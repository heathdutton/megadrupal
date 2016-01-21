<?php

/**
 * @file
 * Detamo's template file.
 */

/**
 * Implements theme_image().
 */
function detamo_image($variables) {
  $attributes = $variables['attributes'];
  $attributes['src'] = file_create_url($variables['path']);

  // Don't output height to avoid issues with aspect ratio.
  foreach (array('width', 'alt', 'title') as $key) {

    if (isset($variables[$key])) {
      $attributes[$key] = $variables[$key];
    }
  }

  return '<img' . drupal_attributes($attributes) . ' />';
}

/**
 * Preprocesses the wrapping HTML.
 *
 * @param array &$variables
 *   Template variables.
 */
function detamo_preprocess_html(&$variables) {

  // Add reset.css before core and module css. See
  // http://fourkitchens.com/blog/2010/11/12/promiscuous-stylesheets-drupal-7
  $reset = drupal_get_path("theme", "detamo") . "/css/reset.css";

  $options = array(
    'group'  => CSS_SYSTEM - 1,
    'weight' => -10,
  );

  drupal_add_css($reset, $options);

  // Add a conditional stylesheet for IE.
  drupal_add_css(drupal_get_path("theme", "detamo") . '/css/ie-desktop.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));

}

/**
 * Preprocessor for page.tpl.php template file.
 */
function detamo_preprocess_page(&$vars, $hook) {

  // For easy printing of variables.
  $vars['logo_img'] = '';
  if (!empty($vars['logo'])) {
    $vars['logo_img'] = theme('image', array(
      'path'  => $vars['logo'],
      'alt'   => t('Home'),
      'title' => t('Home'),
    ));
  }
  $vars['linked_logo_img']  = '';
  if (!empty($vars['logo_img'])) {
    $vars['linked_logo_img'] = l($vars['logo_img'], '<front>', array(
      'attributes' => array(
        'rel'   => 'home',
        'title' => t('Home'),
      ),
      'html' => TRUE,
    ));
  }
  $vars['linked_site_name'] = '';
  if (!empty($vars['site_name'])) {
    $vars['linked_site_name'] = l($vars['site_name'], '<front>', array(
      'attributes' => array(
        'rel'   => 'home',
        'title' => t('Home'),
      ),
    ));
  }

  // Site navigation links.
  $vars['main_menu_links'] = '';
  if (isset($vars['main_menu'])) {
    $vars['main_menu_links'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'id' => 'main-menu',
        'class' => array('inline', 'main-menu'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }
  $vars['secondary_menu_links'] = '';
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu_links'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'id'    => 'secondary-menu',
        'class' => array('inline', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

}
