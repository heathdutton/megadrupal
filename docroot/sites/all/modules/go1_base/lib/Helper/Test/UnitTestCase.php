<?php

namespace Drupal\go1_base\Helper\Test;

use Drupal\go1_base\Autoloader;

require_once dirname(__FILE__) . '/Cache.php';
require_once dirname(__FILE__) . '/Database.php';

/**
 * cache_get()/cache_set() does not work on unit test cases.
 */
abstract class UnitTestCase extends \DrupalUnitTestCase {
  protected $container;

  public function getInfo() {
    return array(
      'name' => 'GO1 Unit',
      'description' => 'Make sure the go1_cache() is working correctly.',
      'group' => 'GO1 Unit'
    );
  }

  public function setUp() {
    $this->container = go1_container('container');

    // Mock db, cache
    unset($this->container['wrapper.db']);
    unset($this->container['wrapper.cache']);

    $this->container['wrapper.db'] = function() { return new \Drupal\go1_base\Helper\Test\Database(); };
    $this->container['wrapper.cache'] = function() { return new \Drupal\go1_base\Helper\Test\Cache(); };

    // Make our autoloader run first — drush_print_r(spl_autoload_functions());
    spl_autoload_unregister('drupal_autoload_class');
    spl_autoload_unregister('drupal_autoload_interface');
    go1_id(new Autoloader('Drupal'))->register(FALSE, TRUE);

    $this->setUpModules();

    parent::setUp('go1_base', 'go1test_base');
  }

  protected function setUpModules() {
    // go1_modules() > system_list() > need db, fake it!
    // 'id' => "GO1Config:{$module}:{$id}:{$key}:" . ($include_go1_base ? 1 : 0),
    $cids_1 = array('go1modules:go1_base:', 'go1modules:go1_base:services', 'go1modules:go1_base:twig_functions');
    $data_1 = array('go1_base', 'go1test_base');
    foreach ($cids_1 as $cid) {
      go1_container('wrapper.cache')->set($cid, $data_1, 'cache_bootstrap');
    }

    $cids_2 = array('go1modules:go1_base:twig_filters');
    $data_2 = array('go1_base');
    foreach ($cids_2 as $cid) {
      go1_container('wrapper.cache')->set($cid, $data_2, 'cache_bootstrap');
    }
  }
}
