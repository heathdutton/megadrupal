<?php

/**
 * Implements hook_default_page_manager_handlers().
 *
 * Provides default pages for page manager
 */
function page_manager_redirect_examples_default_page_manager_pages() {
  // Copy exported page code here.

  $pages = array();
  $files = file_scan_directory(drupal_get_path('module', 'page_manager_redirect_examples') . '/pages/default', '/\.page.inc$/', array('recurse' => FALSE));

  foreach ($files as $file) {
    include DRUPAL_ROOT . '/' . $file->uri;
    $pages[$file->name] = $page;
  }

  return $pages;
}

/**
 * Implements hook_default_page_manager_handlers().
 *
 * Provides default "variants" (or 'handlers') for page manager
 */
function page_manager_redirect_examples_default_page_manager_handlers() {
  $handlers = array();
  $files = file_scan_directory(drupal_get_path('module', 'page_manager_redirect_examples') . '/pages/default', '/\.handler.inc$/', array('recurse' => FALSE));

  foreach ($files as $file) {
    include DRUPAL_ROOT . '/' . $file->uri;
    $handlers[$file->name] = $handler;
  }
  return $handlers;
}
