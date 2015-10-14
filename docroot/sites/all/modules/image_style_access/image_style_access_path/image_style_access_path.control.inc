<?php
/**
 * @file
 * Access control implementation for path condition.
 */

/**
 * Factory for path condition access control.
 */
class ImageStyleAccessPathControlFactory implements ImageStyleAccessControlFactoryInterface {
  /**
   * {@inheritdoc}
   */
  public function createControl($style, $scheme, $target) {
    return new ImageStyleAccessPathControl($target);
  }
}

/**
 * Path context access control.
 */
class ImageStyleAccessPathControl implements ImageStyleAccessControlInterface {
  /**
   * Path parts.
   * @var string[]
   */
  protected $parts;

  /**
   * Creates a path context control.
   */
  public function __construct($path) {
    $this->parts = explode('/', $path);
  }

  /**
   * {@inheritdoc}
   */
  public function match(ImageStyleAccessConfig $config) {
    // Evaluate using matcher.
    $access = $config->settings['access'];
    $contexts = $this->getPathContexts($config->settings['contexts']);
    return $this->checkAccess($access, $contexts);
  }

  /**
   * Checks access using plugin.
   */
  protected function checkAccess(array $test, array $contexts) {
    ctools_include('context');

    // Evaluate access.
    return ctools_access(array('plugins' => array($test)), $contexts);
  }

  /**
   * Prepares an array of contexts from path arguments.
   */
  protected function getPathContexts(array $arguments = array()) {
    ctools_include('context');
    $contexts = array();
    foreach ($arguments as $argument) {
      $index = (int) $argument['id'];
      $value = $this->parts[$index];
      $id = ctools_context_id($argument, 'argument');
      $context = ctools_context_get_context_from_argument($argument, $value);
      if ($context) {
        $contexts[$id] = $context;
      }
    }
    return $contexts;
  }
}
