<?php

/**
 * @file
 * Alter function extensions.
 */

/**
 * Implements hook_page_alter().
 */
function pure_page_alter(&$page) {
  // Get the themes to iterate through.
  $themes = _pure_theme_collector();

  // Get the current menu item.
  $menu_item = menu_get_item();

  // Call a variant of hook_page_alter() that uses the menu_item callback.
  foreach ($themes as $theme => $theme_info) {
    if (!empty($menu_item['page_callback'])) {
      $function = $theme . '_page_' . $menu_item['page_callback'] . '_alter';
      if (function_exists($function)) {
        $function($page);
      }
    }
  }
}

/**
 * Implements hook_block_view_alter().
 */
function pure_block_view_alter(&$data, $block) {
  // Get the themes to iterate through.
  $themes = _pure_theme_collector();

  // Check for an alter function based on a block's module and run it if it
  // exists.
  foreach ($themes as $theme => $theme_info) {
    $function = $theme . '_block_view_' . $block->module . '_alter';
    if (function_exists($function)) {
      $function($data, $block);
    }
  }
}

/**
 * Implements hook_form_alter().
 */
function pure_form_alter(&$form, &$form_state, $form_id) {
  // Give forms a common theme function so we do not have to declare every
  // single form we want to override in hook_theme().
  if (is_array($form['#theme'])) {
    $hooks = array('form_content');
    $form['#theme'] = array_merge($form['#theme'], $hooks);
  }
  else {
    $form['#theme'] = array(
      $form['#theme'],
      'form_content',
    );
  }
}

