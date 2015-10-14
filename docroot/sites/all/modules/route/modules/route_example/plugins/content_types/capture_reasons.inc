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
  'title' => t('Capture reasons for registering'),
  'description' => t('Adds a route enabled form that captures reasons for registration.'),
  'category' => t('Route Example'),
);

/**
 * Output function for the content type.
 *
 * Outputs the page title of the current page.
 */
function route_example_capture_reasons_content_type_render($subtype, $conf, $panel_args) {
  $block = new stdClass();
  
  // Provide content only for active wizards
  module_load_include('inc', 'route_example', 'route_example.capture_reasons_form');
  $block->title = t('Why have you registered here?');
  $block->content = route_get_form('route_control_form', 'route_example_capture_reasons_form');
  
  return $block;
}

/**
 * Implements settings for for this content type
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function route_example_capture_reasons_content_type_edit_form($form, &$form_state) {
  // provide a blank form so we have a place to override title
  // and stuff.
  return $form;
}