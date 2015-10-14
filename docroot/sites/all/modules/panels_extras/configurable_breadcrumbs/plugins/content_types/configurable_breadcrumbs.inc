<?php
// $Id: configurable_breadcrumbs.inc,v 1.0 2011/07/25 16:40:48 chriscalip Exp $

/**
 * @file
 * Plugin to handle the 'configurable_breadcrumb' content type which allows a
 * configurable breadcrumb trail of the current page to be embedded into a panel.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Configurable Breadcrumbs Block'),
  'single' => TRUE,
  'content_types' => array('configurable_breadcrumbs_content_type'),
  'render callback' => 'configurable_breadcrumbs_content_type_render',
  'edit form' => 'configurable_breadcrumbs_content_type_edit_form',
  'description' => t('Add a breadcrumb trail.'),
  'category' => t('Panels Extras'),
);

/**
 * Output function for the 'configurable_breadcrumb' content type.
 *
 * Outputs the breadcrumb for the current page.
 * @param unknown_type $subtype
 * @param unknown_type $conf
 * @param unknown_type $panel_args
 * @return object $block
 */
function configurable_breadcrumbs_content_type_render($subtype, $conf, $panel_args) {
  // do stuff with the breadcrumb before rendering it as a block
  $breadcrumb = drupal_get_breadcrumb();
  // Optionally get rid of the homepage link.
  $show_breadcrumb_home = $conf['config_item_configurable_breadcrumbs_home'];
  if ( !$show_breadcrumb_home ) {
    array_shift($breadcrumb);
  }

  // Return the breadcrumb with separators.
  if ( !empty($breadcrumb) ) {
    $breadcrumb_separator = $conf['config_item_configurable_breadcrumbs_separator'];
    $trailing_separator = $title = '';
    if ( $conf['config_item_configurable_breadcrumbs_title'] ) {
      $menu_item = menu_get_item();
      $title = !empty($menu_item['title']) ? $menu_item['title'] : drupal_get_title();
      if ( !empty($title) ) {
        $trailing_separator = $breadcrumb_separator;
      }
    }
    elseif ( $conf['config_item_configurable_breadcrumbs_trailing'] ) {
      $trailing_separator = $breadcrumb_separator;
    }
    $breadcrumb_content =  '<div class="breadcrumb">' . implode($breadcrumb_separator, $breadcrumb) . "$trailing_separator$title</div>";
    $block = new stdClass();
    $block->content = $breadcrumb_content;
    return $block;
  }
}


/**
 * 'Edit' callback for the content type.
 * This example just returns a form.
 *
 */
function configurable_breadcrumbs_content_type_edit_form($form, &$form_state) {
  $saved_settings = $form_state['conf'];
  $default_settings['config_item_configurable_breadcrumbs_separator'] = ' › ';
  $default_settings['config_item_configurable_breadcrumbs_home']      = 1;
  $default_settings['config_item_configurable_breadcrumbs_trailing']  = 1;
  $default_settings['config_item_configurable_breadcrumbs_title']     = 0;
  $form_state['conf']['config_item_extras_breadcrumbs_separator'] = 0;
  $form_state['conf']['config_item_extras_breadcrumbs_trailing'] = 0;
  // first time this form is gonna get set.
  if ( empty($saved_settings) ) {
    $settings = $default_settings;
  }
  else {
    $settings = $saved_settings;
  }

  $form['config_item_configurable_breadcrumbs_separator'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 10,
    '#description' => t('Text only. Don’t forget to include spaces.'),
    '#default_value' => $settings['config_item_configurable_breadcrumbs_separator'],
    '#title' => t('Breadcrumb Separator'),
  );
  $form['config_item_configurable_breadcrumbs_home'] = array(
    '#type' => 'checkbox',
    '#default_value' => $settings['config_item_configurable_breadcrumbs_home'],
    '#title' => t('Show home page link in breadcrumb'),
  );
  $form['config_item_configurable_breadcrumbs_trailing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append a separator to the end of the breadcrumb'),
    '#default_value' => $settings['config_item_configurable_breadcrumbs_trailing'],
    '#description' => t('Useful when the breadcrumb is placed just before the title.'),
  );
  $form['config_item_configurable_breadcrumbs_title'] = array(
    '#type' => 'checkbox',
    '#default_value' => $settings['config_item_configurable_breadcrumbs_title'],
    '#title' => t('Append the content title to the end of the breadcrumb'),
    '#description' => t('Useful when the breadcrumb is not placed just before the title.'),
  );
  return $form;
}

/**
 * Submit handler of configurable_breadcrumbs_content_type_edit_form
 */
function configurable_breadcrumbs_content_type_edit_form_submit($form, &$form_state) {
  foreach ( element_children($form) as $key ) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
