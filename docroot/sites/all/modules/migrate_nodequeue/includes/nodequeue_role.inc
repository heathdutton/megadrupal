<?php

/**
 * Destination handler for nodequeues.
 */
class NodequeueRoleDestination extends MigrateDestinationTable {
  public function __construct() {
    parent::__construct('nodequeue_roles');
  }

  /**
   * {nodequeue_roles} has no primary key so we define it excplicitely here.
   */
  public static function getKeySchema() {
    return array(
      'qid' => array(
        'description' => 'Queue id',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'rid' => array(
        'description' => 'Role id',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    );
  }

  /**
   * @override
   */
  public function import(stdClass $d7role, stdclass $d6role) {
    // Make sure D7 queue exists.
    $qname = migrate_nodequeue_get_qname($d7role->qid);
    if (!$qname || !nodequeue_load($qname)) {
      throw new MigrateException(t('Invalid destination queue ID @d7qid (source queue ID was @d6qid)', array('@d6qid' => $d6role->qid, '@d7qid' => $d7role->qid)));
    }

    // Make sure D7 role  exists.
    if (!user_role_load($d7role->rid)) {
      throw new MigrateException(t('Invalid destination role ID @d7rid (source role ID was @d6rid)', array('@d6rid' => $d6role->rid, '@d7rid' => $d7role->rid)));
    }
    
    // Import record.
    $status = drupal_write_record('nodequeue_roles', $d7role);
    if ($status) {
      ($status == SAVED_NEW) ? $this->numCreated++ : $this->numUpdated++;
    }

    return array($d7role->qid, $d7role->rid);
  }

  /**
   * @override
   *
   * Rollback record.
   */
  public function rollback(array $key) {
    $qid = $key['destid1'];
    $rid = $key['destid2'];
    db_delete('nodequeue_roles')
      ->condition('qid', $qid)
      ->condition('rid', $rid)
      ->execute();
  }

}

