<?php

/**
 * @file
 *  This file contains all the hooks that are potentially utilized by any of
 * the tools. Always wrap each function with `hap_uses_hook()` to prevent
 * declaring a hook when it is not necessary. Each hook will use `hap_proxy`
 * to delegate tools-specific logic to each corresponding tool's "pseudo-hook".
 */

/**
 * Implements hook_menu().
 */
if (hap_uses_hook('menu')) {
  function hap_menu() {
    return hap_proxy('menu');
  }
}

/**
 * Implements hook_menu_alter().
 */
if (hap_uses_hook('menu_alter')) {
  function hap_menu_alter(&$items) {
    return hap_proxy('menu_alter', $items);
  }
}


/**
 * Implements hook_theme().
 */
if (hap_uses_hook('theme')) {
  function hap_theme($existing, $type, $theme, $path) {
    return hap_proxy('theme', $existing, $type, $theme, $path);
  }
}


/**
 * Implements hook_url_outbound_alter().
 */
if (hap_uses_hook('url_outbound_alter')) {
  function hap_url_outbound_alter(&$path, &$options, $original_path) {
    hap_proxy('url_outbound_alter', $path, $options, $original_path);
  }
}

/**
 * Implements hook_page_build().
 */
if (hap_uses_hook('page_build')) {
  function hap_page_build(&$page) {
    hap_proxy('page_build', $page);
  }
}

/**
 * Implements hook_modules_enabled().
 */
if (hap_uses_hook('modules_enabled')) {
  function hap_modules_enabled($modules) {
    hap_proxy('modules_enabled', $modules);
  }
}

/**
 * Implements hook_form_panels_panel_context_edit_content_alter().
 */
if (hap_uses_hook('form_panels_panel_context_edit_content_alter')) {
  function hap_form_panels_panel_context_edit_content_alter(&$form, &$form_state) {
    hap_proxy('form_panels_panel_context_edit_content_alter', $form, $form_state);
  }
}

/**
 * Implements hook_panels_post_render().
 */
if (hap_uses_hook('panels_post_render')) {
  function hap_panels_post_render($renderer) {
    hap_proxy('panels_post_render', $renderer);
  }
}
