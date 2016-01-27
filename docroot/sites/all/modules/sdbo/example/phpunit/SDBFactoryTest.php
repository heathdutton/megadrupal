<?php
/**
 * @file
 *   Test suite cowering SDBFactory functionality.
 */

require 'bootstrap.php';

class SDBFactoryTest extends PHPUnit_Framework_TestCase {

  /**
   * @var SDBExampleParent
   */
  public $sdb_object;

  /**
   *
   */
  protected function setUp() {
    $this->sdb_object = new SDBExampleParent();
    $this->sdb_object->uid = 1;
    $this->sdb_object->save();
  }

  /**
   *
   */
  function tearDown() {
    $this->sdb_object->delete();
  }

  /**
   *
   */
  function testLoadInstance() {
    $object = SDBFactory::loadInstanceOf('SDBExampleParent', array('uid' => 1));

    $this->assertTrue($object->isPersisted());
    $this->assertEquals('1', $object->uid);
  }

  function testInitInstance() {
    $res = db_select(SDBExampleParent::TABLE)->fields(SDBExampleParent::TABLE)->condition('uid', 1)->execute()->fetchObject();
    $object = SDBFactory::initInstanceOf('SDBExampleParent', $res, array('uid' => 1), TRUE);

    $this->assertTrue($object->isPersisted());
    $this->assertEquals('1', $object->uid);

    $object = SDBFactory::loadInstanceFromRegistry('SDBExampleParent', array('uid' => 1));

    $this->assertTrue($object->isPersisted());
    $this->assertEquals('1', $object->uid);
  }

  function testInitInstances() {

    $object = new SDBExampleParent();
    $object->uid = 7;
    $object->save();

    $res = db_select(SDBExampleParent::TABLE)->fields(SDBExampleParent::TABLE)->execute()->fetchAllAssoc('uid');
    $objects = SDBFactory::initInstancesOf('SDBExampleParent', $res);

    $this->assertNotEmpty($objects[1]);
    $this->assertNotEmpty($objects[7]);

    $this->assertTrue($objects[1]->isPersisted());
    $this->assertEquals('1', $objects[1]->uid);

    $this->assertTrue($objects[7]->isPersisted());
    $this->assertEquals('7', $objects[7]->uid);

    $object->delete();
  }
}
