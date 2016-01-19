<?php

/**
 * @file
 * Plugin definition.
 *
 * In-depth description of a boilerplate code below is in
 * site_map_panels_blogs.inc file.
 */

$plugin = array(
  'title' => t('Site Map (menu)'),
  'description' => t('Displays site map (menu tree).'),
  'category' => t('Miscellaneous'),
  'defaults' => array(),
  'single' => TRUE,
);

/**
 * Render callback.
 */
function site_map_panels_site_map_panels_menu_tree_content_type_render($subtype, $conf, $args, $contexts) {
  $block = new stdClass();

  $block->title = !empty($conf['override_title']) ? check_plain($conf['override_title_text']) : '';
  // _site_map_menus() is a function from module Site Map. It displays all menus
  // as trees.
  $block->content = _site_map_menus();

  return $block;
}

function site_map_panels_site_map_panels_menu_tree_content_type_edit_form($form, &$form_state) {
  return $form;
}

function site_map_panels_site_map_panels_menu_tree_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

