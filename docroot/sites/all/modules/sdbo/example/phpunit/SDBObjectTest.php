<?php
/**
 * @file
 *   Tests of ADD and DELETE operations for SDB objects.
 */

require 'bootstrap.php';

class SDBObjectTest extends PHPUnit_Framework_TestCase {

  function testAddDelete() {
    $data = array(
      'created' => date('d.m.Y H:i:s'),
      'title' => hash('sha256', microtime() . mt_rand()),
      'body' => 'testing body'
    );

    $sdb_object = new SDBExampleChild();
    $sdb_object->created = $data['created'];
    $sdb_object->title = $data['title'];
    $sdb_object->body = $data['body'];

    $sdb_object->save();

    $sdb_object = SDBFactory::loadInstanceOf('SDBExampleChild', array('title' => $data['title']));

    $this->assertEquals($data['created'], $sdb_object->created);
    $this->assertEquals($data['title'], $sdb_object->title);
    $this->assertEquals($data['body'], $sdb_object->body);

    $sdb_object->delete();

    $sdb_object = SDBFactory::loadInstanceOf('SDBExampleChild', array('title' => $data['title']), TRUE);

    $this->assertFalse($sdb_object->isPersisted());
  }
}
