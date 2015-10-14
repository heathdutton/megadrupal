<?php
/**
 * @file
 * Migration script for users migration.
 */

/**
 *
 * Migrate Backend Users
 */
class Typo3BeUserMigration extends BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Backend users of Typo3 site.');
    $this->map = new MigrateSQLMap($this->machineName,
        array('uid' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'User ID.'
                )
            ),
        MigrateDestinationUser::getKeySchema()
    );

    $query = db_select(TYPO3_DATABASE_NAME . '.be_users', 'u')
              ->fields('u', $this->queryParams['beuser']['fields'])
              ->orderBy($this->queryParams['beuser']['order_field'], $this->queryParams['beuser']['order_field_operator']);
    $query->condition('u.deleted', '0');
    $query->condition('u.disable', $this->queryParams['beuser']['disable'], $this->queryParams['beuser']['disable_operator']);

    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser(array('md5_passwords' => TRUE));

    // Mapped fields
    $this->addFieldMapping('name', 'username');
    $this->addFieldMapping('created', 'crdate');
    $this->addFieldMapping('mail', 'email')
          ->defaultValue('test@test.in');
    $this->addFieldMapping('pass', 'password')
          ->defaultValue('1234');
    $this->addFieldMapping('roles')
          ->defaultValue(drupal_map_assoc(array(2)));
    $this->addFieldMapping('status', 'disable');
    $this->addFieldMapping('signature')
          ->defaultValue('be');

    // Unmapped destination fields
    $this->addFieldMapping('theme')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('access')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('login')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('timezone')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('picture')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('init')
          ->issueGroup(t('DNM'));
  }

  /**
   *
   * Catch the raw data and process according to need. Here if username is admin, in Drupal its name is changed
   * in order to migrate this user
   * @param stdClass $account
   * @param stdClass $row
   */
  public function prepare($account, stdClass $row) {
    if ($row->username == 'admin') {
      $account->name = 'typo3_admin';
    }
    // block user in drupal if it is disabled in typo3
    $account->status  = ($row->disable) ? '0' : '1';
  }
}


/**
 *
 * Front end User Migration
 */
class Typo3FeUserMigration extends BaseMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('FrondEnd users of the Typo3 site.');
    $this->map = new MigrateSQLMap($this->machineName,
        array('uid' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'FeUser ID.'
                )
            ),
        MigrateDestinationUser::getKeySchema()
    );

    $query = db_select(TYPO3_DATABASE_NAME . '.fe_users', 'u')
              ->fields('u', $this->queryParams['feuser']['fields'])
              ->orderBy($this->queryParams['feuser']['order_field'], $this->queryParams['feuser']['order_field_operator']);

    $query->condition('u.deleted', '0');
    $query->condition('u.disable', $this->queryParams['feuser']['disable'], $this->queryParams['feuser']['disable_operator']);

    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();

    // Mapped fields
    $this->addFieldMapping('name', 'username');
    $this->addFieldMapping('created', 'crdate');
    $this->addFieldMapping('mail', 'email')
          ->defaultValue('test@test.in');
    $this->addFieldMapping('pass', 'password')
          ->defaultValue('1234');
    $this->addFieldMapping('roles')
          ->defaultValue(drupal_map_assoc(array(2)));
    $this->addFieldMapping('status', 'disable');
    $this->addFieldMapping('signature')
          ->defaultValue('fe');

    // Unmapped destination fields
    $this->addFieldMapping('theme')
          ->issueGroup(t('DNM'));

    $this->addFieldMapping('access')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('login')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('timezone')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('picture')
          ->issueGroup(t('DNM'));
    $this->addFieldMapping('init')
          ->issueGroup(t('DNM'));
  }

  /**
   *
   * If account is disabled, disable it in Drupal
   * @param stdClass $account
   * @param stdClass $row
   */
  public function prepare($account, stdClass $row) {
    $account->status  = ($row->disable) ? '0' : '1';
  }
}
