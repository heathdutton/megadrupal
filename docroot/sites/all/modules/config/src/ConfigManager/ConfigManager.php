<?php

/**
 * @file
 * Contains a ConfigManager
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config\ConfigManager;

use Drupal\config\Exception\ConfigException;
use Drupal\config\Parser\ParserManager;

/**
 * Class ConfigManager
 * @package Drupal\config\ConfigManager
 */
class ConfigManager {

  /**
   * Lazy loader.
   *
   * @return static
   *   An instance of ConfigManager
   * @static
   */
  static public function create() {
    return new static();
  }

  /**
   * List all configuration.
   *
   * @return array
   *   An array of config.
   */
  public function listAllConfig() {
    $config_sets = array();

    $modules = module_implements('config_api');

    if (!empty($modules)) {
      foreach ($modules as $module_name) {

        $module_sets = array();

        $module_settings = module_invoke($module_name, 'config_api');

        // Do not parse incompatible APIs.
        if ($module_settings['api'] != CONFIG_API_VERSION) {
          continue;
        }

        // Set a default 'config' set if none are present.
        if (!isset($module_settings['sets'])) {
          $module_settings['sets']['config'] = $this->setDefaults();
        }

        foreach ($module_settings['sets'] as $set_key => $set) {
          $module_sets[$module_name . '_' . $set_key] = $this->processSet($module_name, $set_key, $set);
        }

        $config_sets = $config_sets + $module_sets;
      }
    }

    return $config_sets;
  }

  /**
   * Get an individual set.
   *
   * @param string $set_name
   *   Name of the set to get.
   *
   * @return array
   *   An array of set information
   */
  public function getSet($set_name) {
    $sets = $this->listAllConfig();

    if (isset($set_name, $sets)) {

      return $sets[$set_name];
    }

    return NULL;
  }

  /**
   * Process a set.
   *
   * @param string $module_name
   *   Name of the owning module.
   * @param string $set_key
   *   Name of the set
   * @param array $set
   *   Set information.
   *
   * @return array|null
   *   The set information
   */
  protected function processSet($module_name, $set_key, $set) {

    $set_name = $module_name . '_' . $set_key;

    $set = $set + $this->setDefaults();

    // Set some defaults for the UI.
    $set['module'] = $module_name;
    $set['name'] = $set_name;

    if (!isset($set['title'])) {
      $set['title'] = $set_name;
    }

    if (!isset($set['description'])) {
      $set['description'] = '';
    }

    if (empty($set['files'])) {

      if ($set['path_type'] == 'relative') {
        $path_to_set = drupal_get_path('module', $module_name) . '/' . $set['path'];
      }
      else {
        $path_to_set = $set['path'];
      }

      try {
        $parser_info = ParserManager::init()->getParserInfo($set['type']);
      }
      catch(ConfigException $e) {
        watchdog('config', 'No matching parser for config set !set', array(
          '!set' => $set_key,
        ));

        return NULL;
      }

      $files = file_scan_directory($path_to_set, '/\.' . $parser_info['extension'] . '/');
      if (!empty($files)) {
        foreach ($files as $file) {
          $set['files'][] = array(
            'name' => $file->filename,
            'uri' => $file->uri,
            'type' => $set['type'],
          );
        }
      }
    }
    else {
      foreach ($set['files'] as $file_index => $file) {
        if (is_string($file)) {
          $set['files'][$file_index] = array(
            'name' => $file,
            'type' => $set['type'],
          );
        }

        if ($set['path_type'] == 'relative') {

          $module_path = drupal_get_path('module', $module_name);
          $set['files'][$file_index]['uri'] = $module_path . '/' . $set['path'] . '/' . $set['files'][$file_index]['name'];
        }
        elseif ($set['path_type'] == 'absolute') {

          $set['files'][$file_index]['uri'] = $set['path'] . '/' . $set['files'][$file_index]['name'];
        }

      }
    }

    return $set;
  }

  /**
   * Get defaults for sets.
   *
   * @return array
   *   Default settings.
   */
  protected function setDefaults() {
    return array(
      'path' => 'config',
      'path_type' => 'relative',
      'type' => CONFIG_PARSER_JSON,
      'files' => array(),
    );
  }
}
