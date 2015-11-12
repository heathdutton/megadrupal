<?php

/**
 * @file ./includes/go.autload.inc
 *
 * @author Andy Truong
 */

/**
 * You can tell go.module to find your directory by define go_autoload() in your
 * settings.php
 */
if (!function_exists('go_autoload')) {
  /**
   * Wrapper for _go_autoload()
   *
   * @param  string $class
   * @return boolean
   */
  function go_autoload($class) {
    $base_dirs[] = trim(substr(dirname(dirname(__FILE__)) . DIRECTORY_SEPARATOR . 'lib', strlen(DRUPAL_ROOT)), '/');
    $base_dirs[] = 'sites'. DIRECTORY_SEPARATOR .'all'. DIRECTORY_SEPARATOR .'libraries';
    return _go_autoload($class, $base_dirs);
  }
}

/**
 * Register auto loader.
 */
spl_autoload_register('go_autoload');

/**
 * Simple PSR-0 autoloader.
 *
 * - \Drupal\%module\Class: If the %module is enabled, go will load the file
 *     /path/to/%module/lib/Drupal/%module/Class.php
 * - \Drupal\Components\Foo: go will load the file
 *     /sites/%site/Drupal/Components/Foo.php
 * - Other cases, please use composer autoloader.
 *
 * @param  string $class
 * @param  array  $base_dirs
 */
function _go_autoload($class, $base_dirs) {
  // PSR-0 loader, do not support normal class
  $supported = strpos($class, '\\') !== FALSE;

  // Only Drupal namespace
  $supported = $supported && (strpos($class, 'Drupal\\') !== FALSE);

  if ($supported && $file = go_autoload_get_file($class, $base_dirs)) {
    require_once $file;
  }
}

/**
 * Get autoloading file.
 *
 * @param  string $class
 */
function go_autoload_get_file($class, $base_dirs, $reset = FALSE) {
  $apc = function_exists('apc_store');
  $cid = "go_autoload:{$class}";

  if ($apc && !$reset) {
    $file = apc_fetch($cid);
    if (FALSE !== $file) {
      return $file;
    }
  }

  $file = _go_autoload_get_file($class, $base_dirs);

  if ($apc) {
    // apc_fetch return FALSE on failure, that's why we cast FALSE to 0.
    apc_store($cid, $file ? $file : 0);
  }

  return $file;
}

/**
 * Details implementation for go_autoload_get_file. Heart is here.
 *
 * @param  string $class
 * @return mixed
 */
function _go_autoload_get_file($class, $base_dirs) {
  // #####################
  // Global \Drupal
  // #####################
  // Convert \ to DIRECTORY_SEPARATOR
  $path = str_replace('\\', DIRECTORY_SEPARATOR, $class);
  $path = DRUPAL_ROOT . "/%s/{$path}.php";

  foreach ($base_dirs as $site) {
    $file = sprintf($path, $site);
    if (is_file($file)) {
      return $file;
    }
  }

  // #####################
  // \Drupal\%module
  // #####################
  $path = str_replace('\\', DIRECTORY_SEPARATOR, $class);
  $path = DRUPAL_ROOT . "/%s/lib/{$path}.php";
  foreach (go_get_module_apis() as $module => $info) {
    if (strpos($suffix, "Drupal/{$module}/") !== FALSE) {
      $file = sprintf($path, drupal_get_path('module', $module));
      if (file_exists($file)) {
        return $file;
      }
    }
  }

  return FALSE;
}
