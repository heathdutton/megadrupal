<?php

/**
 * Describes Views hooks.
 */
class XtoolsCallableTypeHookViews extends XtoolsCallableTypeHook {

  /**
   * Implements XtoolsCallableType::getCallables().
   */
  function getCallables($module) {
    // Include Views include files.
    if ($views_api = module_invoke($module, 'views_api')) {
      $path = isset($views_api['path']) ? $views_api['path'] : drupal_get_path('module', $module);
      $suffixes = array('', '_default');
      foreach ($suffixes as $suffix) {
        $file = $path . '/' . "$module.views$suffix.inc";
        if (file_exists($file)) {
          require_once $file;
        }
      }
    }
    return parent::getCallables($module);
  }
}