<?php
namespace devtools\patterns;

/**
 * Singleton implementation uses PHP's late static binding, requiring
 * PHP >= 5.3
 */
abstract class Singleton extends \devtools\Base {
  // prevent constructors in child classes
  final private function __construct() {
    
  }

  // prevent cloning
  final private function __clone() {

  }
  
  // create & return singleton instance
  final public static function instance() {
    if (is_null(static::$_instance)) {
      static::$_instance = new static;
    }

    return static::$_instance;
  }
}