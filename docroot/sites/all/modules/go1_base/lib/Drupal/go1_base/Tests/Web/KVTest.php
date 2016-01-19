<?php

namespace Drupal\go1_base\Tests\Web;

/**
 * Test case for Key-Value storage
 *
 *    drush test-run --dirty 'Drupal\go1_base\Tests\Web\KVTest'
 */
class KVTest extends \DrupalWebTestCase {
  public function getInfo() {
    return array(
      'name' => 'GO1 Web: Key-Value storage',
      'description' => 'Check Key-Value storage functionality',
      'group' => 'GO1 Web',
    );
  }

  public function setUp() {
    $this->profile = 'testing';
    parent::setUp('go1test_base', 'go1test_route');
  }

  private function getKV($collection = 'go1test') {
    return go1_container('kv', $collection);
  }

  private function getKVExpirable($collection = 'go1test') {
    return go1_container('kv.expirable', $collection);
  }

  public function testInfo() {
    $kv = $this->getKV('go1test');
    $this->assertEqual('go1test', $kv->getCollectionName());

    $kv = $this->getKVExpirable('go1test');
    $this->assertEqual('go1test', $kv->getCollectionName());

    $kv = $this->getKV('go1test_other');
    $this->assertNotEqual('go1test', $kv->getCollectionName());

    $kv = $this->getKVExpirable('go1test_other');
    $this->assertNotEqual('go1test', $kv->getCollectionName());
  }

  public function testSetGetDelete() {
    $kv = $this->getKV();

    // String
    $kv->set('first_name', 'Andy');
    $kv->setIfNotExists('first_name', 'Hong');
    $kv->setIfNotExists('last_name', 'Truong');

    $this->assertEqual('Andy', $kv->get('first_name'));
    $this->assertEqual('Truong', $kv->get('last_name'));
    $kv->delete('first_name');
    $kv->delete('last_name');
    $this->assertNull($kv->get('first_name'));
    $this->assertNull($kv->get('last_name'));

    // Array
    $name = array('Andy', 'Truong');
    $kv->set('name', $name);
    $this->assertEqual($name, $kv->get('name'));
    $kv->delete('name');
    $this->assertNull($kv->get('name'));
  }

  public function testSetGetDeleteMultiple() {
    $kv = $this->getKV();

    // Clean everything
    $kv->deleteAll();

    $kv->setMultiple(array('first_name' => 'Andy', 'last_name' => 'Truong'));
    $values = $kv->getMultiple(array('first_name', 'last_name'));
    $this->assertEqual('Andy', $values['first_name']);
    $this->assertEqual('Truong', $values['last_name']);
  }

  public function testGetDeleteAll() {
    $kv = $this->getKV();

    // Clean everything
    $kv->deleteAll();

    // Check getAll()
    $kv->setMultiple(array('first_name' => 'Andy', 'last_name' => 'Truong'));
    $values = $kv->getAll();
    $this->assertEqual('Andy', $values['first_name']);
    $this->assertEqual('Truong', $values['last_name']);

    // Check deleteAll()
    $kv->deleteAll();
    $this->assertNull($kv->get('first_name'));
    $this->assertNull($kv->get('last_name'));
  }

  public function testExpirable() {
    $kv = $this->getKVExpirable();

    // Change
    $time = time();

    // Setup value now $time
    \go1_fake::time(function() use ($time) { return $time; });
    $kv->setWithExpire('minute', 60, 60);
    $this->assertEqual(60, $kv->get('minute'));

    // $time after 61 seconds
    \go1_fake::time(function() use ($time) { return $time + 61; });
    $this->assertNull($kv->get('minute'));

    // Back to $time
    \go1_fake::time(function() use ($time) { return $time; });
    $kv->setMultipleWithExpire(array('hour' => 3600, 'day' => 86400), 60);
    $this->assertEqual(3600,  $kv->get('hour'));
    $this->assertEqual(86400, $kv->get('day'));

    // $time after 61 seconds
    \go1_fake::time(function() use ($time) { return $time + 61; });
    $this->assertNull($kv->get('hour'));
    $this->assertNull($kv->get('day'));
  }
}
