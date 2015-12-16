<?php
/**
 * @file
 * Contextual links module integration.
 */

/**
 * Implements hook_config_builder_form_config_builder_page_callback_alter() on
 * behalf of contextual.module.
 */
function contextual_config_builder_form_config_builder_page_callback_alter(&$form, $form_state) {
  $config = $form_state['build_info']['args'][0];
  $config = !is_object($config) ? config_builder_crud_load($config) : $config;
  $form['#contextual_links'] = array('config_builder' => array('admin/structure/config_builder/list', array($config->name, 'edit')));
}

/**
 * Implements hook_menu_contextual_links_alter().
 */
function config_builder_ui_menu_contextual_links_alter(&$links, $router_item, $root_path) {
  // Edit configuration page.
  if ('admin/structure/config_builder/list/%/edit' == $root_path) {
    $links['config_builder-edit'] = array_merge(
      $router_item,
      array(
        'title' => 'Edit configuration page',
      )
    );
  }
}
