<?php

namespace Drupal\go1_base\Tests\Web;

/**
 * Test cases for routing feature.
 *
 * drush test-run --dirty 'Drupal\go1_base\Tests\Web\RouteTest'
 */
class RouteTest extends \DrupalWebTestCase {
  public function getInfo() {
    return array(
      'name' => 'GO1 Base: Route',
      'description' => 'Make sure the routing feature is working correctly.',
      'group' => 'GO1 Web',
    );
  }

  public function setUp() {
    $this->profile = 'testing';
    parent::setUp('go1test_base', 'go1test_route');
  }

  public function testRoutes() {
    $request = new \Drupal\go1_base\Helper\SubRequest();

    # ---------------------
    # Test /go1test_route/drupal
    # ---------------------
    $output = menu_execute_active_handler('go1test_route/drupal', FALSE);
    $this->assertEqual('Hello GO1', $output);

    # ---------------------
    # Test /go1test_route/controller
    # ---------------------
    $output = menu_execute_active_handler('go1test_route/controller', FALSE);
    $this->assertEqual('Hi GO1!', $output);

    # ---------------------
    # Test /go1test_route/template
    # ---------------------
    $output = menu_execute_active_handler('go1test_route/template', FALSE);
    $output = trim(render($output));
    $this->assertEqual('Hello GO1', $output);

    # ---------------------
    # Test /go1test_route/multiple-template
    # ---------------------
    $output = menu_execute_active_handler('go1test_route/multiple-template', FALSE);
    $output = trim(render($output));
    $this->assertEqual('Hello GO1', $output);

    # ---------------------
    # Test /go1test_route/fancy_template/%user
    # ---------------------
    $response = $request->request('go1test_route/fancy_template/1');
    $this->assertTrue(strpos($response, 'Foo: bar'));
    $this->assertTrue(strpos($response, 'User ID: 1'));

    # ---------------------
    # Test /go1test_route/with_assets
    # ---------------------
    $response = $request->request('go1test_route/with_assets');
    $this->assertTrue(in_array('misc/vertical-tabs.css', $response['#attached']['css']));
    $this->assertTrue(in_array('misc/vertical-tabs.js', $response['#attached']['js']));
    $this->assertTrue(in_array(array('system', 'jquery.bbq'), $response['#attached']['library']));

    # ---------------------
    # Support caching
    # ---------------------
    // bit of hack, more sure the route is cachable
    $_SERVER['REQUEST_METHOD'] = 'GET';
    $response_0 = trim($request->request('go1test_route/cache/1'));
    sleep(1);
    $response_1 = trim($request->request('go1test_route/cache/1'));
    $this->assertEqual($response_0, $response_1);
  }

  public function testRouteBlock() {
    $blocks['help'] = array();
    $blocks['help'][] = 'system:powered-by';
    $blocks['help'][] = array('go1_base:go1test_base|hi_s', array('title' => 'Hello block!', 'weight' => -100));
    $blocks['help'][] = array(
      'delta'   => 'fancy-block',
      'subject' => 'Fancy block',
      'content' => array('content' => 'Hey Andy!'),
      'weight'  => 1000,
    );

    go1_container('container')->offsetSet('page.blocks', $blocks);

    // Render the page array
    $page = array();
    go1_base_page_build($page);
    $output = drupal_render($page);

    // Found two blocks
    $this->assertTrue(FALSE !== strpos($output, 'Powered by'));
    $this->assertTrue(FALSE !== strpos($output, 'Hello block!'));
    $this->assertTrue(FALSE !== strpos($output, 'Fancy block'));
    $this->assertTrue(FALSE !== strpos($output, 'Hey Andy!'));
  }
}
