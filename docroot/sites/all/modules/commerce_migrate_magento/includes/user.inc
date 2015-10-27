<?php

/**
 * @file
 *   Commerce Customer Profile Billing migration.
 *   Magento stores this info with the order. We create one new
 *   profile per order.
 */
class CommerceMigrateMagentoUserMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    // Source fields
    $source_fields = array(
      'pass' => t('Password'),
      // 'first_name' => t('First Name'),
      // 'midde_name' => t('Middle Name'),
      // 'last_name' => t('Last Name'),
      // 'dob' => t('Date of Birth'),
      // 'gender' => t('Gender'),
    );
    $store_id = $arguments['store_id'];
    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'entity_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Magento Entity ID',
          'alias' => 'ce',
        ),
      ),
      MigrateDestinationUser::getKeySchema()
    );
    $user_sharing = commerce_migrate_magento_variable_get('customer/account_share/scope');
    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('customer_entity', 'ce');
    $query->fields('ce')->distinct();
    if($user_sharing){
      $or = db_or();
      $or->condition('ce.website_id', $store_id, '=');
      $or->condition('ce.store_id', $store_id, '=');
      $query->condition($or);
    }
    $this->source = new MigrateSourceSQL($query, $source_fields, NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationUser(array('md5_passwords' => TRUE));

    // Make the mappings
    // $this->addFieldMapping('uid', 'entity_id');
    $this->addFieldMapping('name', 'email')->dedupe('users', 'name');
    $this->addFieldMapping('pass', 'pass');
    $this->addFieldMapping('mail', 'email')->dedupe('users', 'mail');
    $this->addFieldMapping('roles')->defaultValue(DRUPAL_AUTHENTICATED_RID);
    $this->addFieldMapping('language')->defaultValue('');
    $this->addFieldMapping('theme')->defaultValue('');
    $this->addFieldMapping('signature')->defaultValue('');
    $this->addFieldMapping('signature_format')->defaultValue('filtered_html');
    $this->addFieldMapping('created', 'created_at');
    $this->addFieldMapping('status', 'is_active')->defaultValue(1);
    $this->addFieldMapping('picture')->defaultValue(0);
    $this->addFieldMapping('timezone')->defaultValue(NULL);
    $this->addFieldMapping('path')->issueGroup(t('DNM'));
    // custom field mapping
    // $this->addFieldMapping('field_first_name', 'first_name');
    // $this->addFieldMapping('field_mid_name', 'midde_name');
    // $this->addFieldMapping('field_last_name', 'last_name');
    // $this->addFieldMapping('field_dob', 'dob');
    // $this->addFieldMapping('field_gender', 'gender');
    $this->addUnmigratedDestinations(array('uid'));
  }

  public function prepareRow($row) {
    // $row->first_name = ucfirst(commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer', 'firstname'));
    // $row->midde_name = ucfirst(commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer', 'middlename'));
    // $row->last_name  = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer', 'lastname');
    // $row->dob        = strtotime(commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer', 'dob'));
    $row->pass       = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer', 'password_hash');
    // $row->gender     = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer', 'gender');
    $row->created_at = strtotime($row->created_at);
  }

  public function complete($account, $row) {
    db_update('users')->fields(array('pass' => '$M$' . $row->pass))->condition('uid', $account->uid)->execute();
  }
}

