<?php

/**
 * Migration for user roles.
 */
class OaRoleMigration extends DrupalRole6Migration {
  /**
   * Overrides Migration::prepareRow().
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Include a mode to skip importing roles that aren't explicitly mapped.
    if (!empty($this->arguments['skip_unknown_roles'])) {
      if (!isset($this->roleMappings[$row->name])) {
        return FALSE;
      }
    }
  }
}

/**
 * Migration for users that takes the global user store into account.
 */
class OaUserMigration extends DrupalUser6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Map the user profile fields if available.
    if (module_exists('oa_migrate_legacy_user')) {
      $this->addFieldMapping('field_oa_migrate_legacy_user_org', 'field_profile_organization');
      $this->addFieldMapping('field_oa_migrate_legacy_user_tel', 'field_profile_telephone');
      $this->addFieldMapping('field_oa_migrate_legacy_user_url', 'field_profile_url');
      $this->addFieldMapping('field_oa_migrate_legacy_user_adr', 'field_profile_address');
      $this->addFieldMapping('field_oa_migrate_legacy_user_adr:format', 'field_profile_address:format')
           ->callbacks(array($this, 'mapFormat'));
    }

    // We want to map the user picture to the field_user_picture.
    $this->addFieldMapping('field_user_picture', 'picture')
         ->sourceMigration($arguments['picture_migration']);
    $this->addFieldMapping('field_user_picture:file_class')
         ->defaultValue('MigrateFileFid');
  }

  /**
   * Overrides Migration::complete().
   */
  public function complete($entity, $row) {
    parent::complete($entity, $row);

    // If given, we'll put all new users under a default top-level Space.
    if (!empty($this->arguments['parent_space_default'])) {
      og_group('node', $this->arguments['parent_space_default'], array(
        'entity_type' => 'user',
        'entity' => $entity,
      ));
    }
  }
}

/**
 * Migrating group memberships.
 */
class OaMembershipMigration extends DrupalMigration {
  public function __construct(array $arguments) {
    $user_migration = $arguments['user_migration'];
    $this->dependencies[] = $user_migration;

    $group_migration = $arguments['group_migration'];
    $this->dependencies[] = $group_migration;

    parent::__construct($arguments);

    // Document the fields on the 'og_uid' table.
    $this->sourceFields += array(
      'nid' => t('Node ID of group.'),
      'uid' => t('User ID of member.'),
      'og_role' => t('Not used.'),
      'is_active' => t('Is this membership active or pending?'),
      'is_admin' => t('Is this user a group administrator?'),
      'created' => t('Time when this memebership was created.'),
      'changed' => t('Time when this membership was last changed.'),
    );

    // Setup the core Migrate relationships.
    $query = $this->query();
    $this->map = new MigrateSQLMap($this->machineName, array(
      'nid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'node ID of group.',
      ),
      'uid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'User ID of member.',
      ),
    ), MigrateDestinationOGMembership::getKeySchema());
    $this->source = new MigrateDrupal6SourceSQL($query, $this->sourceFields, NULL, $this->sourceOptions);
    $this->destination = new MigrateDestinationOGMembership();

    // Mappings.
    $this->addFieldMapping('group_type', NULL)
         ->defaultValue('node');
    $this->addFieldMapping('gid', 'nid')
         ->sourceMigration($group_migration);
    $this->addFieldMapping('entity_type', NULL)
         ->defaultValue('user');
    $this->addFieldMapping('etid', 'uid')
         ->sourceMigration($user_migration);
    $this->addSimpleMappings(array('is_admin', 'created'));
  }

  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
      ->select('og_uid', 'ogu')
      ->fields('ogu', array('nid', 'og_role', 'is_active', 'is_admin', 'uid', 'created', 'changed'))
      ->orderBy('ogu.changed');
    return $query;
  }
}
