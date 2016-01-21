<?php
/**
 * @file
 * Class for uniformly handling big numbers (using either gmp or bcmath).
 */

class BigNum {
  static $div;
  static $mul;
  static $add;
  static $sub;
  static $mod;
  static $cmp;
  static $pow;

  /**
   * Setup BigNum class.
   */
  public static function setup() {
    if (function_exists('gmp_init')) {
      self::$div = 'gmp_div';
      self::$mul = 'gmp_mul';
      self::$add = 'gmp_add';
      self::$sub = 'gmp_sub';
      self::$mod = 'gmp_mod';
      self::$cmp = 'gmp_cmp';
      self::$pow = 'gmp_pow';
    }
    elseif (function_exists('bcadd')) {
      self::$div = 'bcdiv';
      self::$mul = 'bcmul';
      self::$add = 'bcadd';
      self::$sub = 'bcsub';
      self::$mod = 'bcmod';
      self::$cmp = 'bccomp';
      self::$pow = 'bcpow';
    }
    else {
      throw new Exception("You must install either the GMP or BCMath extension for PHP");
    }
  }

  public static function div($a, $b, $scale = NULL) {
    return call_user_func(self::$div, (string) $a, (string) $b, $scale);
  }
  public static function mul($a, $b, $scale = NULL) {
    return call_user_func(self::$mul, (string) $a, (string) $b, $scale);
  }
  public static function add($a, $b, $scale = NULL) {
    return call_user_func(self::$add, (string) $a, (string) $b, $scale);
  }
  public static function sub($a, $b, $scale = NULL) {
    return call_user_func(self::$sub, (string) $a, (string) $b, $scale);
  }
  public static function mod($a, $b) {
    return call_user_func(self::$mod, (string) $a, (string) $b);
  }
  public static function cmp($a, $b, $scale = NULL) {
    return call_user_func(self::$cmp, (string) $a, (string) $b, $scale);
  }
  public static function pow($a, $b, $scale = NULL) {
    return call_user_func(self::$pow, (string) $a, (string) $b, $scale);
  }
  public static function round($a, $scale = 0) {
    $add = $scale ? self::div(5, self::pow(10, ($scale + 1)), $scale + 1) : 0;
    return self::add($a, $add, $scale);
  }
  public static function floor($a, $scale = 0) {
    return self::add($a, 0, $scale);
  }
  public static function ceil($a, $scale = 0) {
    $add = $scale ? self::div(9, self::pow(10, ($scale + 1)), $scale + 1) : 0;
    return self::add($a, $add, $scale);
  }
}

BigNum::setup();
