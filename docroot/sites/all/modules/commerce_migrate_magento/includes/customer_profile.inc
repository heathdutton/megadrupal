<?php

/**
 * @file
 *   Commerce Customer Profile Billing migration.
 *   Magento stores this info with the order. We create one new
 *   profile per order.
 */
class CommerceMigrateMagentoCustomerProfileMigration extends Migration {

  private $type;

  public function __construct($arguments) {
    $this->type = $arguments['type'];
    $user_sharing = commerce_migrate_magento_variable_get('customer/account_share/scope');
    parent::__construct($arguments);
    // Source fields
    $source_fields = array(
      'type' => t('The type of the customer profile.'),
      // 'phones' => t('Phone numbers.'),
      // 'fax' => t('Fax number.')
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
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_customer_profile', $arguments['type'])
    );
    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('customer_address_entity', 'cae');
    $query->leftJoin('customer_entity', 'ce', 'ce.entity_id = cae.entity_id');
    $query->fields('cae')->distinct();
    if($user_sharing){
      $or = db_or();
      $or->condition('ce.website_id', $store_id, '=');
      $or->condition('ce.store_id', $store_id, '=');
      $query->condition($or);
    }
    $this->source = new MigrateSourceSQL($query, $source_fields, NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_customer_profile', $arguments['type']);
    // Make the mappings
    if($user_sharing){
      $this->addFieldMapping('uid', 'parent_id')->sourceMigration('CommerceMagentoUser' . ucfirst($arguments['store_code']))->defaultValue(0);
    }
    else {
      $this->addFieldMapping('uid', 'parent_id')->sourceMigration('CommerceMagentoUserGlobal')->defaultValue(0);
    }
    $this->addFieldMapping('created', 'created_at');
    $this->addFieldMapping('changed', 'updated_at');
    $this->addFieldMapping('type', 'type')->defaultValue($arguments['type']);
    $this->addFieldMapping('status', 'is_active');
    // $this->addFieldMapping('field_phones', 'phones');
    // $this->addFieldMapping('field_fax', 'fax');
  }

  public function prepare($profile, stdClass $row) {
    $row->first_name   = ucfirst(commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'firstname'));
    $row->last_name    = ucfirst(commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'lastname'));
    $row->country_code = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'country_id');
    module_load_include('inc', 'addressfield', 'addressfield.administrative_areas');
    $administrative_areas = addressfield_get_administrative_areas($row->country_code);
    if (!empty($administrative_areas)) {
      $row->administrative_area = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'region');
      $row->administrative_area = array_search($row->administrative_area, $administrative_areas);
    }
    $row->street      = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'street');
    $row->city        = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'city');
    $row->postal_code = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'postcode');
    $row->company     = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'company');
    $name             = $row->first_name . ' ' . $row->last_name;
    // There are too many fields to do this through a field handler.
    // @todo Not comfortable with assuming LANGUAGE_NONE.
    $profile->commerce_customer_address[LANGUAGE_NONE][0] = array(
      'xnl' => '<NameDetails PartyType="Person"><NameLine>' . $name . '</NameLine></NameDetails>',
      'name_line' => $name,
      'first_name' => $row->first_name,
      'last_name' => $row->last_name,
      'administrative_area' => $row->administrative_area,
      'country' => $row->country_code,
      'thoroughfare' => $row->street,
      'locality' => $row->city,
      'postal_code' => $row->postal_code,
      'organisation_name' => $row->company,
    );
    // Check for a duplicate profile for this user.
    $q = new EntityFieldQuery();
    $q->entityCondition('entity_type', 'commerce_customer_profile')
      ->entityCondition('bundle', $profile->type)
      ->fieldCondition('commerce_customer_address', 'first_name', $row->first_name, '=')
      ->fieldCondition('commerce_customer_address', 'last_name', $row->last_name, '=')
      ->fieldCondition('commerce_customer_address', 'organisation_name', $row->company, '=')
      ->fieldCondition('commerce_customer_address', 'thoroughfare', $row->street, '=')
      ->fieldCondition('commerce_customer_address', 'locality', $row->city, '=')
      ->fieldCondition('commerce_customer_address', 'postal_code', $row->postal_code, '=')
      ->range(0, 1);
    $results = $q->execute();
    if (isset($results['commerce_customer_profile'])) {
      // We found a duplicate, mark this as a revision of the existing profile,
      // this keeps the migrate map references intactg.
      $profile->profile_id = current(array_keys($results['commerce_customer_profile']));
      $profile->revision = TRUE;
      $profile->log = 'From Magento user id #' . $row->parent_id;
    }
  }

  public function prepareRow($row) {
    // $phone = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'telephone');
    // $pattern = '/[^a-zA-Z0-9]+/';
    // $phone = preg_replace($pattern, "", $phone);
    // $row->phones = array($phone);
    // $fax = commerce_migrate_magento_get_entity_attr_value($row->entity_id, 'customer_address', 'fax');
    // $fax = preg_replace($pattern, "", $fax);
    // $row->fax = $fax;
    // $entity_id = commerce_migrate_magento_get_entity_attr_value($row->parent_id, 'customer', 'default_' . $this->type);
    $entity_id = commerce_migrate_magento_get_entity_attr_value($row->parent_id, 'customer', 'default_' . $this->type);
    // Migrate only default billing profiles
    if ($row->entity_id != $entity_id && $this->type == 'shipping') {
      return FALSE;
    }
    $row->created_at = strtotime($row->created_at);
    $row->updated_at = strtotime($row->updated_at);
  }
}

