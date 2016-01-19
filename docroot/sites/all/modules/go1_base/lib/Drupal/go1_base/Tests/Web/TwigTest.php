<?php

namespace Drupal\go1_base\Tests\Web;

/**
 * drush test-run --dirty 'Drupal\go1_base\Tests\Web\TwigTest'
 */
class TwigTest extends \DrupalWebTestCase {
  public function getInfo() {
    return array(
      'name' => 'GO1 Base: Twig Service',
      'description' => 'Test Twig service',
      'group' => 'GO1 Web'
    );
  }

  public function setUp() {
    $this->profile = 'testing';
    parent::setUp('go1test_base');
  }

  public function testTwigFilters() {
    $output = go1_container('twig_string')->render("{{ 'user:1'|drupalEntity }}");
    $this->assertTrue(strpos($output, 'History'), 'Found text "History"');
    $this->assertTrue(strpos($output, 'Member for'), 'Found text: "Member for"');

    $output  = "{% set o = { template: '@go1test_base/templates/entity/user.html.twig' } %}";
    $output .= "{{ 'user:1'|drupalEntity(o) }}";
    $output  = @go1_container('twig_string')->render($output);
    $this->assertTrue(strpos($output, 'History'), 'Found text "History"');
    $this->assertTrue(strpos($output, 'Member for'), 'Found text: "Member for"');
    $this->assertTrue(strpos($output, '@go1test_base/templates/entity/user.html.twig'), 'Found text: path to template');
  }

  /**
   * Test easy block definition.
   */
  public function testEasyBlocks() {
    $block_1 = \GO1::twig_string()->render("{{ 'go1test_base:hi_s'  | drupalBlock(TRUE) }}");
    $block_2 = \GO1::twig_string()->render("{{ 'go1test_base:hi_t'  | drupalBlock(TRUE) }}");
    $block_3 = \GO1::twig_string()->render("{{ 'go1test_base:hi_ts' | drupalBlock(TRUE) }}");

    $expected = 'Hello GO1';
    $this->assertEqual($expected, trim($block_1));
    $this->assertEqual($expected, trim($block_2));
    $this->assertEqual($expected, trim($block_3));
  }

  public function testDrupalView() {
    $twig = go1_container('twig_string');

    $output = $twig->render("{{ 'go1test_theming_user'|drupalView('default', 1) }}");
    $this->assertTrue(strpos($output, 'views-field views-field-name') !== FALSE);

    $output = $twig->render("{{ 'go1test_theming_user'|drupalView({arguments: [1]}) }}");
    $this->assertTrue(strpos($output, 'views-field views-field-name') !== FALSE);

    $output = $twig->render("{{ 'go1test_theming_user'|drupalView('default', 11111) }}");
    $this->assertTrue(strpos($output, 'views-field views-field-name') === FALSE);

    $output = $twig->render("{{ 'go1test_theming_user'|drupalView({arguments: [11111]}) }}");
    $this->assertTrue(strpos($output, 'views-field views-field-name') === FALSE);
  }
}

// class Go1_Base_Cache_Views_Warmer extends DrupalWebTestCase {
//   public function getInfo() {
//     return array(
//       'name' => 'GO1 Theming: GO1 Cache > Views-Cache warmer',
//       'description' => 'Try views-cache warmer of go1_base.',
//       'group' => 'GO1 Theming',
//     );
//   }

//   protected function setUp() {
//     parent::setUp('go1test_theming');
//   }

//   /**
//    * @todo test me.
//    */
//   public function testViewsCacheWarming() {
//     // Build the first time
//     // $output = go1_id(new Drupal\go1_base\Helper\SubRequest('go1test_theming/users'))->request();
//     $output = views_embed_view('go1test_theming_user', 'page_1');

//     // Invoke entity save event
//     $u = $this->drupalCreateUser();

//     // Build the second time
//     // $output = go1_id(new Drupal\go1_base\Helper\SubRequest('go1test_theming/users'))->request();
//     $output = views_embed_view('go1test_theming_user', 'page_1');
//     $this->assertTrue(FALSE !== strpos($output, $u->name), "Found {$u->name}");

//     $this->verbose(print_r(_cache_get_object('cache_views_data'), TRUE));
//   }
// }
