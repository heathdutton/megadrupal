<?php

namespace Drupal\go1_base\Tests\Unit;

use Drupal\go1_base\Helper\Test\UnitTestCase;

/**
 *  drush test-run --dirty 'Drupal\go1_base\Tests\Unit\CommonTest'
 */
class CommonTest extends UnitTestCase {
  public function getInfo() {
    return array('name' => 'GO1 Unit: Basic features') + parent::getInfo();
  }

  /**
   * Autoload feature
   */
  public function testAutoloader() {
    $this->assertTrue(class_exists('Drupal\go1_base\Cache\Warming\Warmer'));
    $this->assertTrue(class_exists('Drupal\go1_base\Container'));
  }

  /**
   * Test for go1_id() function.
   */
  public function testAtId() {
    $container = new \Drupal\go1_base\Container();
    $this->assertTrue(TRUE, 'No exception raised.');
  }

  /**
   * Test for \Drupal\go1_base\Helper\RealPath class
   */
  public function testRealPath() {
    $helper = go1_container('helper.real_path');

    // @module
    $this->assertEqual(
      drupal_get_path('module', 'go1_base') . '/go1_base.module',
      $helper->get('@go1_base/go1_base.module')
    );

    // %theme
    $this->assertEqual(
      path_to_theme() . '/templates/page.home.html.twig',
      $helper->get('%theme/templates/page.home.html.twig')
    );

    // %library
    $this->assertEqual(
      go1_library('pimple') . '/lib/Pimple.php',
      $helper->get('%pimple/lib/Pimple.php')
    );
  }

  /**
   * Test ExpressionLanguage.
   */
  public function testExpressionLanguage() {
    $engine = \GO1::getExpressionLanguage();

    $expected = 'Symfony\Component\ExpressionLanguage\ExpressionLanguage';
    $actual = get_class($engine);
    $this->assertEqual($expected, $actual);

    $expected = 3;
    $actual = $engine->evaluate("constant('MENU_CONTEXT_PAGE') | constant('MENU_CONTEXT_INLINE')");
    $this->assertEqual($expected, $actual);
  }

  /**
   * Test go1_fn()
   */
  public function testAtFn() {
    // Fake the function
    $GLOBALS['conf']['go1fn:entity_bundle'] = function($type, $entity) { return $entity->type; };

    // Make sure the fake function is executed
    $this->assertEqual('page', go1_fn('entity_bundle', 'node', (object)array('type' => 'page')));
  }
}
