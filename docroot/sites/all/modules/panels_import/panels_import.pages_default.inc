<?php
/**
 * @file
 * panels_import.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function panels_import_default_page_manager_pages() {
  return _panels_import_load_defaults('page_manager', 'pages', 'page');
}

/**
 * Implements hook_default_page_manager_handlers().
 */
function panels_import_default_page_manager_handlers() {
  return _panels_import_load_defaults('page_manager', 'handlers', 'handler');
}
