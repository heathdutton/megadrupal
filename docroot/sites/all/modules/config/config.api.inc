<?php

/**
 * @file
 * API information
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

/**
 * Defines parsers for turning files into actionable code objects.
 *
 * @return array
 *   An array whose keys are parser names and whose corresponding values
 *   are arrays containing the following key-value pairs:
 *   - name: The human-readable name of the component, to be shown on the
 *     permission administration page. This should be wrapped in the t()
 *     function so it can be translated.
 *   - class: The FQN of the class which handles the parser.
 */
function hook_config_parsers() {

  return array(
    CONFIG_PARSER_PHP => array(
      'name' => t('PHP'),
      'class' => '\Drupal\config\Parser\PhpParser',
    ),
    CONFIG_PARSER_YAML => array(
      'name' => t('YAML'),
      'class' => '\Drupal\config\Parser\YamlParser',
    ),
    CONFIG_PARSER_JSON => array(
      'name' => t('JSON'),
      'class' => '\Drupal\config\Parser\JsonParser',
    ),
  );
}

/**
 * Declare that a module provides configuration.
 *
 * This is only required if you want autodetection of configuration. The
 * Config UI module uses this to find Config settings.
 *
 * @return array
 *   An array containing one or more of the following:
 *   - 'api': (required) The api compatibility of this module.
 *     Should be 1 currently.
 *   - 'sets': (optional) An array containing a list of config sets keyed
 *     with a name unique to this function. If sets is omitted, then the
 *     'config' folder will be loaded and all files in it parsed as JSON.
 *     - 'name': (optional) A human readable name.
 *     - 'path': (optional) The path to the folder containing the set. Defaults
 *       to 'config'.
 *     - 'path_type': (optional) The path type. Either relative or absolute.
 *       Defaults to relative. This is relative to the root of the containing
 *       module.
 *     - 'type': (optional) The default config type for files in this set.
 *       Defaults to CONFIG_PARSER_JSON (currently JSON).
 *     - 'files': (optional) An array of config files to load.
 *        If these are flat array they are assumed to match the type
 *        provided in 'type', above. To provide a type per file, they must be
 *        an associative array with the keys 'name' and 'type', where name is
 *        the full file name, and type is the parser type. IF the 'files' key
 *        is not provided, then all files will be loaded.
 */
function hook_config_api() {

  return array(
    // Required.
    'api' => 1,
    // Optional.
    'sets' => array(
      'config' => array(
        'name' => t('Example'),
        'description' => t('Example configuration'),
        'path' => 'config',
        'path_type' => 'relative',
        'type' => CONFIG_PARSER_JSON,
        'files' => array(
          'config_file_one.json',
          'config_file_two.json',
          'not_a_json_file' => array(
            'name' => 'config_file_three.php',
            'type' => CONFIG_PARSER_PHP,
          ),
        ),
      ),
    ),
  );
}
