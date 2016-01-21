<?php

namespace Drupal\krumong\Tests;


class BasicUnitTest extends \DrupalUnitTestCase {

  static function getInfo() {
    return array(
      'name' => 'Krumo NG basic unit test',
      'description' => '...',
      'group' => 'Krumo NG',
    );
  }

  function testObjectSideEffect() {
    $obj = new \stdClass;
    $obj->val = array(0);
    $this->doTestSideEffect($obj, TRUE);
  }

  function testArraySideEffect() {
    $arr = array('val' => array(0));
    $this->doTestSideEffect($arr);
  }

  protected function doTestSideEffect($obj, $inverse_on_devel = FALSE) {
    $this->assertChildIsNotReference($obj, 'val', 'creation');

    // Test krumong()->dpm() side effect.
    krumong()->jsPrefixed($obj);
    $this->assertChildIsNotReference($obj, 'val', 'krumong()->dpm()');

    // Test FirePHP side effect.
    require_once 'sites/all/libraries/FirePHPCore/lib/FirePHPCore/fb.php';
    if (function_exists('fb')) {
      fb($obj);
      $this->assertChildIsNotReference($obj, 'val', 'fb()');
    }

    // Test devel dpm() / krumo::dump() side effect.
    if (module_exists('devel')) {
      @include_once drupal_get_path('module', 'devel') . '/krumo/class.krumo.php';
      if (class_exists('krumo')) {
        ob_start();
        \krumo::dump($obj);
        ob_end_clean();
        $this->assertChildIsNotReference($obj, 'val', 'devel dpm()', $inverse_on_devel);
      }
    }
  }

  /**
   * Assert that $obj->$key / $obj[$key] is not a reference.
   *
   * @param object|array $obj
   *   The object or array.
   * @param string $key
   *   The key.
   * @param string $action
   *   String describing what happened before the assertion.
   * @param boolean $inverse
   *   If TRUE, we already assume that it *will* be a reference,
   *   and the test just confirms the sad fact.
   */
  protected function assertChildIsNotReference($obj, $key, $action, $inverse = FALSE) {
    if (is_object($obj)) {
      $type = 'An object variable';
      $is_ref = $this->objectVarIsReference($obj, $key);
    }
    else {
      $type = 'An array value';
      $is_ref = $this->arrayValueIsReference($obj, $key);
    }
    if (!$inverse) {
      $this->assertTrue(!$is_ref, "$type may not become a reference after $action.");
    }
    else {
      $this->assertTrue($is_ref, "$type sadly does become a reference after $action.");
    }
  }

  protected function objectVarIsReference($obj, $key) {
    $orig = $obj->$key;
    $cl = clone $obj;
    $cl->$key = is_string($orig) ? 99 : 'xx';
    $is_ref = ($obj->$key !== $orig);
    $cl->$key = $orig;
    return $is_ref;
  }

  protected function arrayValueIsReference($arr, $key) {
    $orig = $arr[$key];
    $cl = $arr;
    $cl[$key] = is_string($orig) ? 99 : 'xx';
    $is_ref = ($arr[$key] !== $orig);
    $cl[$key] = $orig;
    return $is_ref;
  }
}
