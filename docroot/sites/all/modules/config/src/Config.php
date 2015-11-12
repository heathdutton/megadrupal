<?php

/**
 * @file
 * Contains a Config
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config;

use Drupal\config\ConfigManager\ConfigManager;
use Drupal\config\Parser\ParserManager;

/**
 * Class Config
 * @package Drupal\canvas_core
 */
class Config {

  /**
   * The Parser manager
   *
   * @var ParserManager
   */
  protected $parserManager;

  /**
   * Loaded config files.
   *
   * @var array
   */
  protected $config;

  /**
   * Lazy constructor.
   *
   * @return static
   *   An instance of Config.
   * @static
   */
  public static function load() {

    $parser_manager = ParserManager::init();

    return new static($parser_manager);
  }

  /**
   * Constructor.
   *
   * @param ParserManager $parser_manager
   *   An instance of ParserManager
   */
  public function __construct(ParserManager $parser_manager) {

    $this->parserManager = $parser_manager;
  }

  /**
   * Get a config set.
   *
   * This only works with sets declared through hook_config_api().
   *
   * @param string $set_name
   *   Name of the set
   *
   * @return array
   *   An array of configuration options.
   */
  public function getConfigSet($set_name) {

    $set = ConfigManager::create()->getSet($set_name);

    if (isset($set['files'])) {
      foreach ($set['files'] as $key => $file) {

        $set['files'][$key]['config'] = $this->getConfig($set['module'], $set['path'], $file['name'], $file['type']);
      }
    }

    return $set;
  }

  /**
   * Fetch config.
   *
   * @param string $module
   *   Name of the module providing the configuration
   * @param string $path
   *   [optional] The directory path within the module. Defaults to 'config'
   * @param string $filename
   *   [optional] Name of the config file to load. Defaults to config.json
   * @param string $parser_type
   *   [optional] Parser to use. Defaults to JSON.
   *
   * @return array
   *   An array of configuration settings.
   * @throws \Drupal\config\Exception\ConfigException
   */
  public function getConfig($module, $path = 'config', $filename = 'config.json', $parser_type = CONFIG_DEFAULT_PARSER) {

    $parser = $this->parserManager->getParser($parser_type);

    return $parser->parseConfig($module, $path, $filename);
  }

  /**
   * Get config by path.
   *
   * @param string $full_path
   *   Full path to the config file
   * @param string $parser_type
   *   Parser to use.
   *
   * @return array
   *   An array of config.
   * @throws \Drupal\config\Exception\ConfigException
   */
  public function getConfigByPath($full_path, $parser_type = CONFIG_DEFAULT_PARSER) {
    $parser = $this->parserManager->getParser($parser_type);

    return $parser->getContent($full_path);
  }
}
