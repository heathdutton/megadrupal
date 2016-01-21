<?php
/**
 * @file
 *   SDBObjects classes.
 */

/**
 * Example SDBObject implementation that in examples represents parent objects.
 */
class SDBExampleParent extends SDBObject {

  public $uid;


  const TABLE = 'sdbo_example_parents';

  function getTable() {
    return self::TABLE;
  }

  function getRelDefinition() {
    return array(
       'SDBExampleChildPolicyCascade' => array(
         'foreign_key' => 'parent_id',
         'del_policy' => 'cascade', //keep|restrict|cascade
         'rel_type' => '1_to_n',
        ),
       'SDBExampleNChildPolicyCascade' => array(
         'mapping_table' => 'sdbo_example_mapping',
         'my_key' => 'parent_id',
         'foreign_key' => 'child_id',
         'del_policy' => 'cascade', //keep|restrict|cascade
         'rel_type' => 'n_to_n',
        ),
       'SDBExampleNChildPolicyRestrict' => array(
         'mapping_table' => 'sdbo_example_mapping_restrict',
         'my_key' => 'parent_id',
         'foreign_key' => 'child_id',
         'del_policy' => 'restrict', //keep|restrict|cascade
         'rel_type' => 'n_to_n',
        ),
       'SDBExampleNChildPolicyKeep' => array(
         'mapping_table' => 'sdbo_example_mapping_keep',
         'my_key' => 'parent_id',
         'foreign_key' => 'child_id',
         'del_policy' => 'keep', //keep|restrict|cascade
         'rel_type' => 'n_to_n',
        ),
      );
  }

  function getPrimaryKey() {
    return 'id';
  }

  function getUniqueKey() {
    return array('id', 'uid');
  }

  function initUniqueKeyValues($init) {
    if (isset($init['id'])) {
      $this->id = $init['id'];
    }
    if (isset($init['uid'])) {
      $this->uid = $init['uid'];
    }
  }
}

class SDBExampleChild extends SDBObject {

  // Persisted attributes
  public $created;
  public $title;
  public $body;

  const TABLE = 'sdbo_example_children';

  /**
   * Implementation should return name of table where its
   * instances get persisted.
   *
   * @return string
   */
  function getTable() {
    return self::TABLE;
  }

  function getUniqueKey() {
    return array('id', 'title');
  }

  function getPrimaryKey() {
    return 'id';
  }

  function initUniqueKeyValues($vals) {
    if (isset($vals['id'])) {
      $this->id = $vals['id'];
    }

    if (isset($vals['title'])) {
      $this->title = $vals['title'];
    }
  }

  function beforeSave() {
    if (is_string($this->created)) {
      $this->created = strtotime($this->created);
    }
  }

  function afterSave() {
    $this->afterLoad();
  }

  function afterLoad() {
    if (is_numeric($this->created)) {
      $this->created = date('d.m.Y H:i:s', $this->created);
    }
  }
}

class SDBExampleNChildPolicyCascade extends SDBExampleChild {

}

class SDBExampleNChildPolicyRestrict extends SDBExampleChild {

}

class SDBExampleNChildPolicyKeep extends SDBExampleChild {

}

class SDBExampleChildPolicyCascade extends SDBExampleChild {

}