<?php

/**
 * Implements hook_html_head_alter().
 * We are overwriting the default meta character type tag with HTML5 version.
 */
function async_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function async_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $heading = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // Uncomment to add current page to breadcrumb
//     $breadcrumb[] = drupal_get_title();
    return '<nav class="breadcrumb grid_24">' . $heading . implode(' » ', $breadcrumb) . '</nav>';
  }
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function async_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Override or insert variables into the html template.
 */
// function async_preprocess_html(&$variables) {
// }

/**
 * Override or insert variables into the page template.
 */
function async_preprocess_page(&$variables) {

  // Set main columns grid
  switch ($variables['layout']){
    case 'both':
      $variables['grid_main'] = ' grid_12 push_6';
      $variables['grid_first'] = ' grid_6 pull_12';
      $variables['grid_second'] = ' grid_6';
      break;
    case 'first':
      $variables['grid_main'] = ' grid_18 push_6';
      $variables['grid_first'] = ' grid_6 pull_18';
      break;
    case 'second':
      $variables['grid_main'] = ' grid_18';
      $variables['grid_second'] = ' grid_6';
      break;
    case 'none':
    default:
      $variables['grid_main'] = ' grid_24';
  };

  // Set preface columns grid
  $preface = Array();

  if ( $variables['page']['preface_one'] ) {
    $preface[] = "preface_one";
  }

  if ( $variables['page']['preface_two'] ) {
    $preface[] = "preface_two";
  }

  if ( $variables['page']['preface_three'] ) {
    $preface[] = "preface_three";
  }

  if ( $variables['page']['preface_four'] ) {
    $preface[] = "preface_four";
  }

  $prefacecount = count($preface);
  switch ($prefacecount){
    case 4:
      $col = "6";
      break;
    case 3:
      $col = "8";
      break;
    case 2:
      $col = "12";
      break;
    case 1:
      $col = "24";
      break;
    default:
  }

  foreach($preface as $key=>$region){
    $alphaomega = "";
    if($key==0){
      $alphaomega .= " alpha";
    }
    if($key==($prefacecount-1)){
      $alphaomega .= " omega";
    }
    $variables['grid_'.$region] = ' grid_' . $col . $alphaomega;
  }

}

/**
 * Override or insert variables into the node template.
 */
function async_preprocess_node(&$variables) {
  $variables['submitted'] = t('Published by !username on !datetime', array('!username' => $variables['name'], '!datetime' => $variables['date']));
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Preprocess variables for region.tpl.php
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
function async_preprocess_region(&$variables, $hook) {
  // Use a bare template for the content region.
  if ($variables['region'] == 'content') {
    $variables['theme_hook_suggestions'][] = 'region__bare';
  }
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function async_preprocess_block(&$variables, $hook) {
  // Use a bare template for the page's main content.
  if ($variables['block_html_id'] == 'block-system-main') {
    $variables['theme_hook_suggestions'][] = 'block__bare';
  }
  $variables['title_attributes_array']['class'][] = 'block-title';
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function async_process_block(&$variables, $hook) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}

/**
 * A Sync credits
 */
function async_credits(){
  return '<div id="credits" class="clearfix"><a href="http://www.themes-drupal.org" title="Themes Drupal">T</a><a href="http://www.siti-drupal.it" title="Siti Drupal Italia">D</a><a href="http://www.realizzazione-siti-vicenza.com" title="Siti Web Vicenza">R</a></div>';
}

/**
 * Changes the search form to use the "search" input element of HTML5.
 */
function async_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}
