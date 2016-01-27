<?php
/**
 * @file
 * Users migration class for Recruiter Demo.
 */

/**
 * Class for migration users.
 */
class RecruiterUserMigration extends RecruiterMigration {

  public function __construct() {
    parent::__construct();
    $this->description = t('Import users.');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );

    $csvcolumns = array();
    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'recruiter_demo') . '/import/user.csv', $csvcolumns, array('header_rows' => 1));

    $this->destination = new MigrateDestinationUser();

    $this->addSimpleMappings(array('mail'));

    $this->addFieldMapping('name', 'name')->dedupe('users', 'name');

    // Set the user's password to be the same as the username.
    $this->addFieldMapping('pass', 'name');

    $this->addFieldMapping('role_names', 'roles')->separator(',');

    $this->addFieldMapping('status')->defaultValue(1);
  }

  public function postImport() {
    if (defined('MAINTENANCE_MODE')) {
      // Workaround to execute rules_invoke_event_by_args in MAINTENANCE mode.
      foreach ($this->getMap() as $item) {
        $uid = $item->destid1;
        if ($user = user_load($uid)) {
          if ($event = rules_get_cache('event_user_insert')) {
            $event->executeByArgs(array('user' => $user));
          }
        }
      }
    }
  }
}
