<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 9:01 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Log;

class Log {

  private static $buffer;

  public static function start() {
    self::$buffer = '';
  }

  public static function write($str) {
    self::$buffer .= $str . '<br>';
  }

  public static function flush() {
    $buf = self::$buffer;
    self::$buffer = '';
    return $buf;
  }

}
