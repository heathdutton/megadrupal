<?php

/**
 * @file
 * Plugin to handle the 'page' content type which allows the standard page
 * template variables to be embedded into a panel.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */ 
$plugin = array(
  'single' => TRUE,
  'title' => t('Route progress indicator'),
  'description' => t('Adds route journey progress bar.'),
  'category' => t('Route'),
  'defaults' => array(
    'form' => NULL,
  ),
);

/**
 * Output function for the 'page_title' content type.
 *
 * Outputs the page title of the current page.
 */
function route_route_progress_indicator_content_type_render($subtype, $conf, $panel_args) {
  $route = route_get(route_token());
  
  $block = new stdClass();
  $block->content = theme('route_progress_indicator', array('route' => $route));
  
  return $block;
}

/**
 * Implements settings for for this content type
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function route_route_progress_indicator_content_type_edit_form($form, &$form_state) {
  // provide a blank form so we have a place to override title
  // and stuff.
  return $form;
}