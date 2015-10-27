<?php

/**
 * @file
 * Smartling log.
 */

namespace Drupal\smartling\Wrappers;

/**
 * Class DrupalAPIWrapper.
 */
class DrupalAPIWrapper {
  public function getDefaultLanguage() {
    return language_default()->language;
  }

  /*
   * A wrapper for Drupal drupal_alter function
   */
  public function alter($hook_name, &$handlers) {
    drupal_alter($hook_name, $handlers);
  }

  public function moduleInvokeAll($hook) {
    return module_invoke_all($hook);
  }

  public function &drupalStatic($name, $default_value = NULL, $reset = FALSE) {
    return drupal_static($name, $default_value, $reset);
  }

  public function drupalRealpath($uri) {
    return drupal_realpath($uri);
  }

  public function fileLoad($fid) {
    return file_load($fid);
  }

  public function elementChildren(&$elements, $sort = FALSE) {
    return element_children($elements, $sort);
  }

  public function rulesInvokeEvent($event_name, $params = array()) {
    if (module_exists('rules')) {
      $params = array_merge(array($event_name), $params);
      call_user_func_array('rules_invoke_event', $params);
    }
  }
}
