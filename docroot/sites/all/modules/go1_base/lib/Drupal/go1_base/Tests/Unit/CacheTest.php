<?php

namespace Drupal\go1_base\Tests\Unit;

use Drupal\go1_base\Helper\Test\UnitTestCase;
use Drupal\go1_base\Helper\Test\Cache;

class CacheTest extends UnitTestCase {
  /**
   * @var Cache
   */
  private $cache;

  public function getInfo() {
    return array('name' => 'GO1 Unit: Cache') + parent::getInfo();
  }

  public function setUp() {
    parent::setUp();

    $this->cache = go1_container('wrapper.cache');
  }

  /**
   * Helper method for testObjectCallback().
   * @return int
   */
  public static function time() {
    return time();
  }

  public function testFakeCacheWrapper() {
    $wrapper = go1_container('wrapper.cache');

    // Make sure the cache wrapper is faked correctly
    $this->assertEqual(
      'Drupal\go1_base\Helper\Test\Cache',
      get_class($this->cache)
    );

    // Save __FUNCTION__ then get __FUNCTION__
    $wrapper->set(__FUNCTION__, __CLASS__);
    $this->assertEqual(__CLASS__, $wrapper->get(__FUNCTION__)->data);
  }

  public function testAtCache() {
    $cache_options = array('reset' => FALSE, 'ttl' => '+ 15 minutes');

    $callbacks = array(
      'closure'    => array(function () { return time(); }, array()),
      'string'     => array('time', array()),
      'object'     => array(array($this, 'time'), array()),
      'static'     => array(__CLASS__ . '::time', array()),
      'arguments'  => array('sprintf', array('Timestamp: %d', time())),
    );

    foreach ($callbacks as $type => $callback) {
      list($callback, $arguments) = $callback;
      $o = array('id' => "go1_test:time:{$type}", 'reset' => TRUE) + $cache_options;

      $output = go1_cache($o, $callback, $arguments);
      $cached = $this->cache->get($o['id'])->data;

      $this->assertEqual($output, $cached);
    }
  }

  public function testStringOptions() {
    $id = 'go1testStringOptions';
    $bin = 'cache';
    $ttl = '';

    // $id
    $output = go1_cache("$id", 'time');
    $cached = $this->cache->get($id, $bin)->data;
    $this->assertEqual($cached, $output);

    // $id,$ttl
    $output = go1_cache("$id,$ttl", 'time');
    $cached = $this->cache->get($id, $bin)->data;
    $this->assertEqual($cached, $output);

    // $id,~,$bin
    $output = go1_cache("$id,~,$bin", 'time');
    $cached = $this->cache->get($id, $bin)->data;
    $this->assertEqual($cached, $output);

    // $id,~,~
    $output = go1_cache("$id,~,~", 'time');
    $cached = $this->cache->get($id, $bin)->data;
    $this->assertEqual($cached, $output);

    // $id,$ttl,$bin
    $output = go1_cache("$id,$ttl,$bin", 'time');
    $cached = $this->cache->get($id, $bin)->data;
    $this->assertEqual($cached, $output);
  }

  public function testAtCacheAllowEmpty() {
    $options = array(
      'bin' => 'cache',
      'reset' => FALSE,
      'ttl' => '+ 15 minutes',
      'id' => 'go1_test:time:allowEmpty',
      'reset' => TRUE,
      'allow_empty' => FALSE
    );

    // Init the value
    $time_1 = go1_cache($options, __CLASS__ . '::time');
    sleep(1);

    // Change cached-data to empty string
    if ($ttl = strtotime($options['ttl'])) {
      go1_container('wrapper.cache')->set($options['id'], '', $options['bin'], $ttl);
    }

    // Call go1_cache() again
    $time_2 = go1_cache(array('reset' => FALSE) + $options, __CLASS__ . '::time');

    // The value should not be same
    $this->assertNotEqual($time_1, $time_2);
  }

  public function testCacheTagging() {
    $o = array('bin' => 'cache', 'reset' => FALSE, 'ttl' => '+ 15 minutes');
    $o = array('id' => 'go1test_base:cache:tag:1', 'tags' => array('go1_base', 'go1test')) + $o;

    // ---------------------------------------------------------------
    // Tag must be written when cache with tag(s)
    // ---------------------------------------------------------------
    go1_cache($o, function(){ return 'Data #1'; });

    $db_log = go1_container('wrapper.db')->getLog();
    $tag1_row = array('bin' => 'cache', 'cid' => $o['id'], 'tag' => $o['tags'][0]);
    $tag2_row = array('bin' => 'cache', 'cid' => $o['id'], 'tag' => $o['tags'][1]);

    $this->assertEqual($tag1_row, $db_log['insert']['go1_base_cache_tag']['fields'][0][0]);
    $this->assertEqual($tag2_row, $db_log['insert']['go1_base_cache_tag']['fields'][1][0]);

    go1_container('wrapper.db')->resetLog();

    // ---------------------
    // Tag must be deleted
    // ---------------------
    // Delete items tagged with 'go1test'
    go1_container('cache.tag_flusher')->flush($o['tags']);

    $db_log = go1_container('wrapper.db')->getLog('delete', 'go1_base_cache_tag');

    $con = array('tag', $o['tags']);
    foreach ($db_log['condition'] as $_con) {
      if ('tag' === $_con[0]) {
        $this->assertEqual($con, $_con);
        return;
      }
    }
    $this->assertTrue(FALSE, 'No delete query on tags found');
  }

  /**
   * @todo Test when we can fake the service.
   */
  public function testCacheWarming() {
    // Fake the cache.tag_flusher service

    // Warmer > Simple

    // Warmer > Entity

    // Warmer > Views

    // Warmer service
  }
}
