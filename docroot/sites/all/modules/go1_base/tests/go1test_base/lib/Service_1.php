<?php
namespace Drupal\go1test_base;

class Service_1 {
  public function hello($name = 'GO1') {
    return "Hello {$name}";
  }

  public static function helloStatic($name = 'GO1') {
    return "Hello {$name}";
  }
}
