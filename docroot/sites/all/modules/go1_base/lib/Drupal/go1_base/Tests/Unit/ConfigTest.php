<?php

namespace Drupal\go1_base\Tests\Unit;

use \Drupal\go1_base\Helper\Test\UnitTestCase;
use \Drupal\go1_base\Helper\ModulesFetcher;

class ConfigTest extends UnitTestCase {
  public function getInfo() {
    return array('name' => 'GO1 Unit: Config') + parent::getInfo();
  }

  /**
   * Test case for go1_config() function.
   */
  public function testConfigGet() {
    // Test getPath(), case #1
    $expected_path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'go1test_config') . '/config/config.yml';
    $actual_path = go1_config('go1test_config')->getPath();
    $this->assertEqual($expected_path, $actual_path);

    // Test getPath(), case #2
    $expected_path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'go1test_config') . '/config/to_be_imported.yml';
    $actual_path = go1_config('go1test_config', '.to_be_imported')->getPath();
    $this->assertEqual($expected_path, $actual_path);

    // Test simple value getting
    $foo = go1_config('go1test_config')->get('foo');
    $this->assertEqual($foo, 'bar');

    // Test not found exception
    try {
      go1_config('go1test_config')->get('not_there');
      $this->assertTrue('No exception thrown');
    }
    catch (\Drupal\go1_base\Config\NotFoundException $e) {
      $this->assertTrue('Throw NotFoundException if config item is not configured.');
    }

    // Test import data
    $config = go1_config('go1test_config', '/import_resources');
    $this->assertEqual('imported_data', $config->get('imported_data'));
    $array_data = $config->get('array_data');
    $this->assertEqual('A',   $array_data['a']);
    $this->assertEqual('CCC', $array_data['c']);
  }

  /**
   * Test for service: helper.config_fetcher
   */
  public function testConfigFetcher() {
    $config_fetcher = go1_container('helper.config_fetcher');

    // Get all
    $items = $config_fetcher->getItems('go1_base', 'services', 'services', TRUE);
    $this->assertTrue(isset($items['twig']));

    // Get specific item
    $item = $config_fetcher->getItem('go1_base', 'services', 'services', 'twig', TRUE);
    $this->assertEqual('@twig.core', $item['arguments'][0]);
  }

  /**
   * Module weight can be updated correctly
   */
  public function testWeight() {
    go1_container('wrapper.db')->resetLog();
    go1_id(new \Drupal\go1_base\Hook\FlushCache())->resolveModuleWeight('go1test_base', 10);
    $db_log = go1_container('wrapper.db')->getLog('update', 'system');

    $expected = array(
      'condition' => array('name', 'go1test_base'),
      'fields' => array('weight' => 10)
    );

    $this->assertTrue(in_array($expected['condition'], $db_log['condition']));
    $this->assertTrue(in_array($expected['fields'], $db_log['fields'][0]));
  }

  /**
   * Make sure go1_modules() function is working correctly.
   */
  public function testAtModules() {
    $enabled_modules = array();

    // Just check with two modules
    foreach (array('go1_base', 'go1test_base') as $module_name) {
      $enabled_modules[$module_name] = drupal_get_path('module', $module_name) . '/'. $module_name .'.info';
      $enabled_modules[$module_name] = file_get_contents($enabled_modules[$module_name]);
      $enabled_modules[$module_name] = drupal_parse_info_format($enabled_modules[$module_name]);
      $enabled_modules[$module_name] = (object)array(
        'name' => $module_name,
        'stauts' => 1,
        'info' => $enabled_modules[$module_name],
      );
    }

    // Case 1: Do not need other modules has any config file.
    $expected = array('go1test_base');
    $actual = go1_id(new ModulesFetcher('go1_base', $config_file = ''))->fetch($enabled_modules);
    $this->assertEqual($expected, $actual);

    // Case 2: The modules have to have a specific config file
    $expected = array('go1test_base');
    $actual = go1_id(new ModulesFetcher('go1_base', $config_file = 'services'))->fetch($enabled_modules);
    $this->assertEqual($expected, $actual);

    $expected = array();
    $actual = go1_id(new ModulesFetcher('go1_base', $config_file = 'un_real_config_file'))->fetch($enabled_modules);
    $this->assertEqual($expected, $actual);
  }
}
