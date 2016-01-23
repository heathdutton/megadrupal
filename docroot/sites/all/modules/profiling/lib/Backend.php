<?php

/**
 * Common logic, static methods and singleton implementation.
 */
class Profiling_Backend
{
  /**
   * Singleton implementation.
   * 
   * @var Profiling_Backend_Interface
   */
  private static $__backend;

  /**
   * Get backend instance.
   * 
   * @return Profiling_Backend_Interface
   */
  public static function getInstance() {
    if (!isset(self::$__backend)) {
      // Use current configured class for backend spawn.
      $class = variable_get('profiling_backend', 'Profiling_Backend_Default');
      self::$__backend = new $class;
    }
    return self::$__backend;
  }
}
