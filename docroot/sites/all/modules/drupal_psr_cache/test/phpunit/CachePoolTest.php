<?php
/**
 * @file
 *
 * Basic Drupal cache pool tests.
 */

require_once __DIR__ . '/../../vendor/autoload.php';

/**
 * Class CachePoolTest
 */
class CachePoolTest extends PHPUnit_Framework_TestCase {

  /**
   * @var PHPUnit_Framework_MockObject_MockObject
   */
  private $handlerMock;

  /**
   * @var \Drupal\PSRCache\CachePool
   */
  private $pool;

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();
    $this->handlerMock = $this->getMock('Drupal\PSRCache\Adaptor\DefaultDrupalCacheHandler');
    $this->pool = new \Drupal\PSRCache\CachePool();
    $this->pool->setCacheHandler($this->handlerMock);
  }

  /**
   * Test if cache item params are set and retrievable.
   */
  public function testCacheItemParams() {
    $item = new \Drupal\PSRCache\CacheItem('key');

    $value = 'foo';
    $ttl = 123;

    $item->set($value);
    $item->setExpiration($ttl);

    $this->assertEquals($value, $item->get());
    $this->assertEquals($ttl, $item->getExpiration());
  }

  /**
   * Test if non-cached items are not returning anything.
   */
  public function testNoHitForUncachedItems() {
    $this->handlerMock
      ->expects($this->once())
      ->method('cacheGet')
      ->willReturn(FALSE);

    $key = md5(microtime(TRUE));
    $cacheItem = $this->pool->getItem($key);
    $this->assertFalse($cacheItem->isHit());
    $this->assertNull($cacheItem->get());
  }

  /**
   * Test if cached items are returning correctly.
   */
  public function testHitForCachedItem() {
    $key = md5(microtime(TRUE));
    $data = 'foo';
    $this->handlerMock
      ->expects($this->once())
      ->method('cacheGet')
      ->willReturn((object) array(
        'cid' => $key,
        'data' => $data,
        'created' => time(),
        'expire' => 0,
        'serialized' => 0,
      ));

    $cacheItem = $this->pool->getItem($key);
    $this->assertTrue($cacheItem->isHit());
    $this->assertEquals($data, $cacheItem->get());
  }

}
