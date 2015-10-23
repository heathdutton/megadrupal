<?php

/**
 * @file
 * eWAY lib autoloader.
 */

class eWAYAutoloaderInit {
  private static $loader;

  public static function loadClassLoader($class) {
    if ('eWAY\Autoload\ClassLoader' === $class) {
      require __DIR__ . '/eWAY/Autoload.php';
    }
  }

  public static function getLoader() {
    if (NULL !== self::$loader) {
      return self::$loader;
    }

    spl_autoload_register(array(
      'eWAYAutoloaderInit',
      'loadClassLoader'
    ), TRUE, TRUE);
    self::$loader = $loader = new \eWAY\Autoload\ClassLoader();
    spl_autoload_unregister(array('eWAYAutoloaderInit', 'loadClassLoader'));

    $loader->set('eWAY', __DIR__ . '/eWAY');
    $loader->setPsr4('eWAY\\', __DIR__ . '/eWAY');
    $loader->register(TRUE);
    $loader->setUseIncludePath(TRUE);

    return $loader;
  }
}

return eWAYAutoloaderInit::getLoader();
