<?php
/**
 * @file
 * Provides a LazyVars interface.
 */

 /**
  * An interface for the LazyVars API.
  */
interface LazyVarsInterface {
  public static function set($name, $var, $cache = FALSE);
  public static function get($name, $default = NULL, $cache = TRUE);
  public static function del($name);
}