<?php

/**
 * @file
 * Defines UserMigration class.
 */

/**
 * Class UserMigration.
 *
 * @package migration_tools
 */
abstract class UserMigration extends CsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_file) {
    parent::__construct($arguments, $source_file);

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'csvrownum' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Row Number',
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );
    $this->destination = new MigrateDestinationUser();

    $this->addFieldMapping("name", "email");
    $this->addFieldMapping("mail", "email");
    $this->addFieldMapping("role_names", "roles")->separator("|");
    $this->addFieldMapping("status")->defaultValue(1);
  }

  /**
   * {@inheritdoc}
   */
  public function csvcolumns() {
    // Note: Remember to subtract 1 from column number.
    $columns[0] = array('first_name', 'First Name');
    $columns[1] = array('last_name', 'Last Name');
    $columns[2] = array('email', 'Email');
    $columns[3] = array('group_role', 'Group Role');
    $columns[4] = array('roles', 'Roles');

    return $columns;
  }

  /**
   * {@inheritdoc}
   */
  public function fields() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, stdClass $row) {
    $gid = $this->getGID($entity);
    if ($gid) {
      $uid = $entity->uid;
      $roles = og_roles("node", "organization");
      $rid = array_search($row->group_role, $roles);
      og_role_grant("node", $gid, $uid, $rid);
    }

    // Assumes that all migrated users will authenticate via SAML.
    db_merge('authmap')
      ->key(array(
        'uid' => $entity->uid,
        'module' => 'simplesamlphp_auth',
      ))
      ->fields(array(
        'authname' => $entity->mail,
      ))
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRollback($entity_id) {
    foreach ($entity_id as $uid) {
      $user = user_load($uid);
      $gid = $this->getGID($user);
      if ($gid) {
        $roles = og_roles("node", "organization");
        foreach (array_keys($roles) as $rid) {
          og_role_revoke("node", $gid, $user->uid, $rid);
        }
      }
    }
  }

  /**
   * Get the group id from a user.
   *
   * @param object $user
   *   A user entity.
   *
   * @return mixed
   *   Entity id of group to which user belongs, or else FALSE.
   */
  protected function getGID($user) {
    $items = field_get_items("user", $user, "og_user_node");
    if (!empty($items)) {
      foreach ($items as $item) {
        return $item['target_id'];
      }
    }
    return FALSE;
  }
}
