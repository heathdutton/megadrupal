<?php

/**
 * Destination class for Open Atrium notifications.
 */
class OaMigrateDestinationNotifications extends MigrateDestination {
  static public function getKeySchema() {
    return array(
      'notification_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination notification',
      ),
    );
  }

  public function fields() {
    $fields = array();
    $fields['notification_id'] = t('Notification: Existing notification ID');
    $fields['source_type'] = t('The type of the item requiring notification, eg "node"');
    $fields['source_id'] = t('The unique ID of the object, such as the nid.');
    $fields['target_type'] = t('The type of entity to be notified, eg "user", "group", "team".');
    $fields['target_id'] = t('The unique ID of the object, such as either the {uid} or {nid}.');
    return $fields;
  }

  public function import(stdClass $object, stdClass $row) {
    $migration = Migration::currentMigration();

    if (method_exists($migration, 'prepare')) {
      $migration->prepare($object, $row);
    }

    if (isset($row->migrate_map_destid1)) {
      if (isset($object->notification_id)) {
        if ($object->notification_id != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming notification_id !notification_id and map destination id !destid1 don't match",
            array('!notification_id' => $object->notification_id, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $object->notification_id = $row->migrate_map_destid1;
      }
    }

    if (empty($object->notification_id)) {
      drupal_write_record('oa_notifications', $object);
      $this->numCreated++;
    }
    else {
      drupal_write_record('oa_notifications', $object, array('notification_id'));
      $this->numUpdated++;
    }

    if (method_exists($migration, 'complete')) {
      $migration->complete($object, $row);
    }

    return !empty($object->notification_id) ? array($object->notification_id) : FALSE;
  }

  public function rollback(array $key) {
    $notification_id = reset($key);
    $result = db_delete('oa_notifications')
      ->condition('notification_id', $notification_id)
      ->execute();
    return (bool)$result;
  }

  /**
   * {@inheritdoc}
   */
  public function __toString() {
    return t('Open Atrium notifications');
  }
}

/**
 * Migrate notifications.
 */
class OaNotificationsMigration extends DrupalMigration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields,
      NULL, $this->sourceOptions);

    $this->destination = new OaMigrateDestinationNotifications();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Unique Subscription id',
        ),
      ),
      OaMigrateDestinationNotifications::getKeySchema()
    );

    $user_migration = $arguments['user_migration'];
    $this->dependencies[] = $user_migration;

    $node_migrations = $arguments['node_migrations'];
    $this->dependencies = array_merge($this->dependencies, $node_migrations);

    $this->addFieldMapping('source_type', NULL)
         ->defaultValue('node');
    $this->addFieldMapping('source_id', 'nid')
         ->sourceMigration($node_migrations);
    $this->addFieldMapping('target_type', NULL)
         ->defaultValue('user');
    $this->addFieldMapping('target_id', 'uid')
         ->sourceMigration($user_migration);
  }

  /**
   * Overrides Migrate::prepareRow().
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Skip notifications if we can't find the node has been migrated.
    $dest_nid = $this->handleSourceMigration($this->arguments['node_migrations'], $row->nid);
    if (!$dest_nid) {
      watchdog('oa_migrate', 'Unable to find migrated node for source nid !nid', array('!nid' => $row->nid), WATCHDOG_WARNING);
      return FALSE;
    }
  }

  /**
   * Query for OA1 notification data.
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('notifications', 's')
      ->fields('s')
      ->condition('s.status', 1)
      // We only want node subscriptions which send e-mails immediately,
      // since that is the equivalent thing we will be creating in OA2.
      ->condition('s.type', 'thread')
      ->condition('s.event_type', 'node')
      ->condition('s.send_interval', 0)
      ->condition('s.send_method', 'mail');

    // Get the actual nid from the fields.
    $query->join('notifications_fields', 'sf', 's.sid = sf.sid and sf.field = :field',
      array(':field' => 'nid'));
    $query->addField('sf', 'value', 'nid');

    return $query;
  }
}
