<?php
/**
 * @file
 * Class Config.
 */

namespace Drupal\tarpit;

/**
 * Class Config.
 */
class Config {

  static protected function defaults($key = NULL) {
    $defaults = array(
      'tarpit.depth' => 2,
      'tarpit.size' => '150',
      'tarpit.links' => '40',
      'tarpit.wordlist' => drupal_get_path('module', 'tarpit') . '/assets/words.txt',
      'tarpit.paths' => array(),
      'tarpit.page_title' => 'Welcome to the tarpit, enjoy yourself !',
      'tarpit.tarpit_forms_path' => 'tarpit',
      'tarpit.tarpit_captcha_path' => 'tarpit',
    );

    if ($key == NULL) {
      return $defaults;
    }

    return isset($defaults[$key]) ? $defaults[$key] : NULL;
  }

  static public function get($parents, $default_value = NULL) {
    $options = \Drupal::service('variable')->get('tarpit_config');

    if (is_string($parents)) {
      $parents = explode('.', $parents);
    }

    if (is_array($parents)) {
      $notfound = FALSE;
      foreach ($parents as $parent) {
        if (isset($options[$parent])) {
          $options = $options[$parent];
        }
        else {
          $notfound = TRUE;
          break;
        }
      }
      if (!$notfound) {
        return $options;
      }
    }

    $value = Config::defaults(implode('.', $parents));
    if (!is_null($value)) {
      return $value;
    }

    if (is_null($default_value)) {
      return FALSE;
    }

    return $default_value;
  }

  static public function set($parents, $value) {
    $config = \Drupal::service('variable')->get('tarpit_config', array());

    if (is_string($parents)) {
      $parents = explode('.', $parents);
    }

    $ref = &$config;
    foreach ($parents as $parent) {
      if (isset($ref) && !is_array($ref)) {
        $ref = array();
      }
      $ref = &$ref[$parent];
    }
    $ref = $value;

    \Drupal::service('variable')->set('tarpit_config', $config);
    return $config;
  }

  static public function clear($parents) {
    $config = \Drupal::service('variable')->get('tarpit_config', array());
    $ref = &$config;

    if (is_string($parents)) {
      $parents = explode('.', $parents);
    }

    $last = end($parents);
    reset($parents);
    foreach ($parents as $parent) {
      if (isset($ref) && !is_array($ref)) {
        $ref = array();
      }
      if ($last == $parent) {
        unset($ref[$parent]);
      }
      else {
        if (isset($ref[$parent])) {
          $ref = &$ref[$parent];
        }
        else {
          break;
        }
      }
    }
    \Drupal::service('variable')->set('tarpit_config', $config);
    return $config;
  }

  static public function delete() {
    variable_del('tarpit_config');
  }
}
