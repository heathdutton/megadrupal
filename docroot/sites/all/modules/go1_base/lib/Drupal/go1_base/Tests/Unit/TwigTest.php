<?php

namespace Drupal\go1_base\Tests\Unit;

use Drupal\go1_base\Helper\Test\UnitTestCase;

/**
 * drush test-run --dirty 'Drupal\go1_base\Tests\Unit\TwigTest'
 */
class TwigTest extends UnitTestCase {
  public function getInfo() {
    return array('name' => 'GO1 Unit: Twig') + parent::getInfo();
  }

  public function testServiceContainer() {
    $this->assertEqual('Twig_Environment', get_class(go1_container('twig')));
    $this->assertEqual('Twig_Environment', get_class(go1_container('twig_string')));
  }

  public function testDefaultFilters() {
    $twig = go1_container('twig');
    $filters = $twig->getFilters();

    $array = array('render', 't', 'url', '_filter_autop');
    $array = array_merge($array, array('drupalBlock', 'drupalEntity', 'drupalView'));
    $array = array_merge($array, array('go1_config'));

    foreach ($array as $filter) {
      $this->assertTrue(isset($filters[$filter]), "Found filter {$filter}");
    }
  }

  public function testLazyFiltersFunctions() {
    $twig = go1_container('twig_string');

    // Use trim() function
    $this->assertEqual($twig->render("{{  '  Drupal 7  '|fn__trim  }}"),  'Drupal 7');
    $this->assertEqual($twig->render("{{  fn__trim('  Drupal 7  ')  }}"), 'Drupal 7');

    // Use Go1_Base_Test_Class::helloStatic()
    $this->assertEqual($twig->render("{{  'Drupal 8'|Go1_Base_Test_Class__class__helloStatic  }}"),  'Hello Drupal 8');
    $this->assertEqual($twig->render("{{  Go1_Base_Test_Class__class__helloStatic('Drupal 8')  }}"), 'Hello Drupal 8');

    // Use Go1_Base_Test_Class::helloProperty()
    $this->assertEqual($twig->render("{{  'PHP'|Go1_Base_Test_Class__obj__helloProperty  }}"), 'Hello PHP');
    $this->assertEqual($twig->render("{{  Go1_Base_Test_Class__obj__helloProperty('PHP')  }}"), 'Hello PHP');

    // Namespace
    $this->assertEqual($twig->render("{{  'Namespace'|ns_Drupal__go1test_base__Service_1__class__helloStatic  }}"),  'Hello Namespace');
    $this->assertEqual($twig->render("{{  ns_Drupal__go1test_base__Service_1__class__helloStatic('Namespace')  }}"), 'Hello Namespace');
  }

  public function testTwigStringLoader() {
    $output = \GO1::twig_string()->render('Hello {{ name }}', array('name' => 'GO1'));
    $this->assertEqual('Hello GO1', $output, 'Template string is rendered correctly.');
  }

  public function testCacheFilter() {
    $twig = go1_container('twig_string');

    $string_1  = "{% set options = { cache_id: 'go1testTwigCache:1' } %}";
    $string_1 .= "\n {{ 'go1test_base.service_1:hello' | cache(options) }}";
    $string_2  = "{% set options = { cache_id: 'go1testTwigCache:2' } %}";
    $string_2 .= "\n {{ 'Go1_Base_Test_Class::helloStatic' | cache(options) }}";
    $string_3  = "{% set options = { cache_id: 'go1testTwigCache:3' } %}";
    $string_3 .= "\n {{ 'go1test_base_hello' | cache(options) }}";
    $string_4  = "{% set options  = { cache_id: 'go1testTwigCache:4' } %}";
    $string_4 .= "\n {% set callback = { callback: 'go1test_base_hello', arguments: ['GO1'] } %}";
    $string_4 .= "\n {{ callback | cache(options) }}";
    for ($i = 1; $i <= 4; $i++) {
      $actual = "string_{$i}";
      $actual = $twig->render($$actual);
      $actual = trim($actual);
      $this->assertEqual('Hello GO1', $actual);
    }
  }

  public function testFilters() {
    $twig = go1_container('twig_string');

    $expected = 'Hello Drupal';

    $this->assertEqual($expected, $twig->render("{{ go1test_1('Drupal') }}"));
    $this->assertEqual($expected, $twig->render("{{ go1test_2('Drupal') }}"));
    $this->assertEqual($expected, $twig->render("{{ go1test_3('Drupal') }}"));
  }
}
