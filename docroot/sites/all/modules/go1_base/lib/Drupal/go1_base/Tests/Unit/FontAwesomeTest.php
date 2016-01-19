<?php

namespace Drupal\go1_base\Tests\Unit;

use Drupal\go1_base\Helper\Test\UnitTestCase;

/**
 * drush test-run --dirty 'Drupal\go1_base\Tests\Unit\FontAwesomeTest'
 */
class FontAwesomeTest extends UnitTestCase {
  public function getInfo() {
    return array('name' => 'GO1 Unit: Icon Fontawesome') + parent::getInfo();
  }

  public function testIconRendering() {
    $css_code = 'camera-retro';

    \go1_fake::drupal_add_css(function($data = NULL, $options = NULL) {
      static $included = array();

      if (!is_null($data)) {
        $included[$data] = TRUE;
      }

      return $included;
    });

    $expected = '<i class="fa fa-'. $css_code .'"></i>';
    $actual = go1_icon($css_code, 'icon.fontawesome');
    $included_css = \go1_fn::drupal_add_css();

    $this->assertEqual($expected, $actual, 'go1_icon() returns the the correct html for icon.');
    $this->assertTrue(isset($included_css[go1_library('fontawesome', NULL, FALSE) . 'css/font-awesome.css']), "fontawesome's css is included to page.");
  }
}
