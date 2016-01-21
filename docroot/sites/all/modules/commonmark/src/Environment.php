<?php
/**
 * @file
 * Contains Drupal\CommonMark\Environment.
 */

namespace Drupal\CommonMark;

use League\CommonMark\Extension\CommonMarkCoreExtension;

/**
 * Class Converter.
 *
 * @package Drupal\CommonMark
 */
class Environment extends \League\CommonMark\Environment {

  /**
   * A placeholder to use for defining method arguments in extensions.
   *
   * @var int
   */
  const EXTENSION_INSTANCE_PLACEHOLDER = -1;

  /**
   * {@inheritdoc}
   */
  public static function createCommonMarkEnvironment($filter = NULL) {
    return new static(['filter' => $filter]);
  }

  /**
   * {@inheritdoc}
   */
  public function __construct(array $config = []) {
    parent::__construct($config);

    $this->addExtension(new CommonMarkCoreExtension());
    $this->mergeConfig([
      'renderer' => [
        'block_separator' => "\n",
        'inner_separator' => "\n",
        'soft_break'      => "\n",
      ],
    ]);

    $filter = $this->config->getConfig('filter');

    // Iterate over each hook defined extension and add it to the environment.
    foreach (commonmark_get_extensions($filter) as $extension) {
      try {
        $class = new \ReflectionClass($extension['class']);
        $args = $extension['class arguments'];
        if ($extension['class arguments callback'] && is_callable($extension['class arguments callback'])) {
          $args = call_user_func_array($extension['class arguments callback'], [$extension, $this]);
        }

        // Instantiate the extension.
        $extension_instance = $class->newInstanceArgs($args);

        // If this is a Drupal\CommonMark\Extension, load in the settings.
        if (in_array('Drupal\CommonMark\ExtensionInterface', class_implements($extension['class']))) {
          // Set the extension's default settings.
          $extension_instance->setSettings($extension['default settings']);

          // Set the extension's current settings.
          $extension_instance->setSettings($extension['settings']);
        }

        // Replace the placeholder for the extension instance in the method
        // arguments with the real class instance.
        $method_args = $extension['method arguments'];
        $index = array_search(self::EXTENSION_INSTANCE_PLACEHOLDER, $method_args, TRUE);
        if ($index !== FALSE) {
          $method_args[$index] = $extension_instance;
        }
        else {
          $method_args[] = $extension_instance;
        }

        // Actually add the extension to the environment.
        call_user_func_array([$this, $extension['method']], $method_args);
      }
      catch (\Exception $e) {
        watchdog('commonmark', $e->getMessage());
      }
    }
  }

}
