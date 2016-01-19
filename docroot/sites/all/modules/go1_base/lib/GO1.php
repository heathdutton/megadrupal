<?php

use Drupal\go1_base\Autoloader;
use Symfony\Component\ExpressionLanguage\ExpressionLanguage;

/**
 * Class for type hint.
 */
class GO1 {
  protected static $container;

  /**
   * Factory method to get container.
   * @return \Drupal\go1_base\Container
   */
  public static function getContainer() {
    if (!static::$container) {
      static::$container = new \Drupal\go1_base\Container();
    }
    return static::$container;
  }

  /**
   * @return \Twig_Environment
   */
  public static function twig() {
      return go1_container('twig');
  }

  /**
   * @return \Twig_Environment
   */
  public static function twig_string() {
    return go1_container('twig_string');
  }

  /**
   * Get expression language object.
   *
   * @return ExpressionLanguage
   * @todo  Use service container when PSR-4 autoloading can be attached there.
   */
  public static function getExpressionLanguage() {
    static $engine;

    if (!$engine) {
      go1_id(new Autoloader('Symfony\Component\ExpressionLanguage', go1_library('expression_language')))
        ->register();

      $engine = new ExpressionLanguage();
    }

    return $engine;
  }
}
