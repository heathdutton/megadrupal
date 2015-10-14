<?php
/**
 * @file
 *   Test suite cowering REL.
 */

require 'bootstrap.php';

class SDBRelTest extends PHPUnit_Framework_TestCase {

  public $rel_definition;

  /**
   * @var SDBObjectInterface
   */
  public $parent_object;

  /**
   *
   */
  protected function setUp() {
    $this->parent_object = new SDBExampleParent();
    $this->parent_object->uid = 1;
    $this->parent_object->save();

    $this->rel_definition = $this->parent_object->rel('SDBExampleNChildPolicyCascade')->getDefinition();
  }

  /**
   *
   */
  function tearDown() {
    $this->parent_object->delete();
  }

  /**
   *
   */
  function testRelDefinitionNtoN() {
    $this->assertEquals('sdb_example_mapping', $this->rel_definition['SDBExampleNChildPolicyCascade']['mapping_table']);
    $this->assertEquals('parent_id', $this->rel_definition['SDBExampleNChildPolicyCascade']['my_key']);
    $this->assertEquals('child_id', $this->rel_definition['SDBExampleNChildPolicyCascade']['foreign_key']);
    $this->assertEquals('cascade', $this->rel_definition['SDBExampleNChildPolicyCascade']['del_policy']);
    $this->assertEquals('n_to_n', $this->rel_definition['SDBExampleNChildPolicyCascade']['rel_type']);
  }

  /**
   *
   */
  function testRelDefinition1toN() {
    $this->assertEquals('parent_id', $this->rel_definition['SDBExampleChildPolicyCascade']['foreign_key']);
    $this->assertEquals('cascade', $this->rel_definition['SDBExampleChildPolicyCascade']['del_policy']);
    $this->assertEquals('1_to_n', $this->rel_definition['SDBExampleChildPolicyCascade']['rel_type']);
  }

  /**
   *
   */
  function testRelAddNtoN() {

    $data = array(
      'created' => date('Y-m-d H:i:s'),
      'title' => hash('sha256', microtime() . mt_rand()),
      'body' => 'test body 1'
    );

    // Add REL object
    $this->parent_object->rel('SDBExampleNChildPolicyCascade')->add($data);
    $this->assertEquals(1, count($this->parent_object->rel('SDBExampleNChildPolicyCascade')->getAll(NULL, 'title')));

    $rel_object = $this->parent_object->rel('SDBExampleNChildPolicyCascade')->getByObjectKey($data['title']);

    $this->assertNotEmpty($rel_object);
    $this->assertEquals($data['body'], $rel_object->body);
  }

  /**
   * @depends testRelAddNtoN
   */
  function testRelDelAllNtoN() {
    $this->parent_object->rel('SDBExampleNChildPolicyCascade')->delAll();
    $this->assertEquals(0, count($this->parent_object->rel('SDBExampleNChildPolicyCascade')->getAll(NULL, NULL, TRUE)));
  }

  /**
   * @expectedException SDBException
   */
  function testRelOperationOnNonPersistedObject() {
    $data = array(
      'created' => date('Y-m-d H:i:s'),
      'title' => hash('sha256', microtime() . mt_rand()),
      'body' => 'test body 2'
    );
    $parent = new SDBExampleParent();
    $parent->rel('SDBExampleNChildPolicyCascade')->add($data);
  }

  /**
   *
   */
  function testRelDelPolicyCascadeNtoN() {
    $data = array(
      'created' => date('Y-m-d H:i:s'),
      'title' => hash('sha256', microtime() . mt_rand()),
      'body' => 'test body 3'
    );
    $parent = new SDBExampleParent();
    $parent->save();
    $parent->rel('SDBExampleNChildPolicyCascade')->add($data);
    $parent->delete();

    $child = SDBFactory::loadInstanceOf('SDBExampleNChildPolicyCascade', array('title' => $data['title']), TRUE);

    $this->assertFalse($child->isPersisted());
  }

  /**
   *
   */
  function testRelDelPolicyRestrictNtoN() {
    $data = array(
      'created' => date('Y-m-d H:i:s'),
      'title' => hash('sha256', microtime() . mt_rand()),
      'body' => 'test body 4'
    );
    $parent = new SDBExampleParent();
    $parent->save();
    $parent->rel('SDBExampleNChildPolicyRestrict')->add($data);

    try {
      $parent->delete();
    }
    catch (SDBException $e) {
      if ($e->getMessage() == 'Cannot delete instance having REL objects with restrict delete policy') {

        $parent->rel('SDBExampleNChildPolicyRestrict')->delAll();
        $parent->delete();

        return;
      }
    }

    $this->fail();
  }

  /**
   *
   */
  function testRelDelPolicyKeepNtoN() {
    $data = array(
      'created' => date('Y-m-d H:i:s'),
      'title' => hash('sha256', microtime() . mt_rand()),
      'body' => 'test body 4'
    );
    $parent = new SDBExampleParent();
    $parent->save();
    $parent->rel('SDBExampleNChildPolicyKeep')->add($data);

    $table = $parent->rel('SDBExampleNChildPolicyKeep')->getMappingTable();
    $field = $parent->rel('SDBExampleNChildPolicyKeep')->getMyKey();
    $value = $parent->getPrimaryKeyValue();

    $parent->delete();

    $child = SDBFactory::loadInstanceOf('SDBExampleNChildPolicyKeep', array('title' => $data['title']));

    $this->assertTrue($child->isPersisted());

    $select = db_select($table);
    $select->addField($table, $field);
    $select->condition($field, $value);

    $this->assertEmpty($select->execute()->fetchAll());

  }
}
