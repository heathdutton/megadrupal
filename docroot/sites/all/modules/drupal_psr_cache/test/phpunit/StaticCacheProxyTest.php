<?php
/**
 * @file
 *
 * Static cache proxy test
 */

class StaticCacheProxyTest extends PHPUnit_Framework_TestCase {

  /**
   * @var PHPUnit_Framework_MockObject_MockObject
   */
  private $handlerMock;

  /**
   * @var \Drupal\PSRCache\CachePool
   */
  private $basicCachePool;

  /**
   * @var \Drupal\PSRCache\StaticCacheProxy
   */
  private $pool;

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();
    $this->handlerMock = $this->getMock('Drupal\PSRCache\Adaptor\DefaultDrupalCacheHandler');
    $this->basicCachePool = new \Drupal\PSRCache\CachePool();
    $this->basicCachePool->setCacheHandler($this->handlerMock);
    $this->pool = new \Drupal\PSRCache\StaticCacheProxy($this->basicCachePool);
  }

  /**
   * As a baseline check if normal caches will do an item fetch each time
   * they ask for it.
   */
  public function testNormalCachesDoesNotStaticCacheItems() {
    $this->handlerMock
      ->expects($this->exactly(3))
      ->method('cacheGet');
    $key = 'foo';
    $this->basicCachePool->getItem($key);
    $this->basicCachePool->getItem($key);
    $this->basicCachePool->getItem($key);
  }

  /**
   * Test if static cache proxy will only go to the underlying layer the very
   * first time.
   */
  public function testStaticCacheProxyOnlyRequestItemsOnce() {
    $this->handlerMock
      ->expects($this->exactly(1))
      ->method('cacheGet');
    $key = 'foo';
    $this->pool->getItem($key);
    $this->pool->getItem($key);
    $this->pool->getItem($key);
  }

}
