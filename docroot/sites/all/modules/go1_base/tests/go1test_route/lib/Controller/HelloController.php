<?php
namespace Drupal\go1test_route\Controller;

class HelloController {
  public function helloAction($name) {
    return "Hi {$name}!";
  }
}
