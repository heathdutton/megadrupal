<?php

/**
 * @file
 *   Commerce Customer Profile Billing migration.
 *   Magento stores this info with the order. We create one new
 *   profile per order.
 */
class CommerceMigrateMagentoOrderProfileMigration extends Migration {

  private $type;

  public function __construct($arguments) {
    $this->type = $arguments['type'];
    parent::__construct($arguments);
    // Source fields
    $source_fields = array(
      'type' => t('The type of the customer profile.'),
      // 'phones' => t('Phone numbers.'),
    );
    $store_id = $arguments['store_id'];
    $user_sharing = commerce_migrate_magento_variable_get('customer/account_share/scope');
    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'entity_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Magento Entity ID',
          'alias' => 'sfoa',
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_customer_profile', $arguments['type'])
    );
    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_magento_get_source_connection();
    $query = $connection->select('sales_flat_order', 'sfo');
    $query->leftJoin('sales_flat_order_address', 'sfoa', 'sfoa.parent_id = sfo.entity_id');
    $query->fields('sfo', array('created_at', 'updated_at'));
    $query->fields('sfoa')->distinct();
    if ($user_sharing) {
      $or = db_or();
      $or->condition('sfo.store_id', $store_id, '=');
      $query->condition($or);
    }
    $query->condition('sfoa.address_type', $arguments['type'], '=');
    $this->source = new MigrateSourceSQL($query, $source_fields, NULL, array('map_joinable' => FALSE));
    $this->destination = new MigrateDestinationEntityAPI('commerce_customer_profile', $arguments['type']);
    // Make the mappings
    if ($user_sharing) {
      $this->addFieldMapping('uid', 'customer_id')->sourceMigration('CommerceMagentoUser' . ucfirst($arguments['store_code']))->defaultValue(0);
    }
    else {
      $this->addFieldMapping('uid', 'customer_id')->sourceMigration('CommerceMagentoUserGlobal')->defaultValue(0);
    }
    $this->addFieldMapping('created', 'created_at');
    $this->addFieldMapping('changed', 'updated_at');
    $this->addFieldMapping('type', 'type')->defaultValue($arguments['type']);
    $this->addFieldMapping('status', NULL)->defaultValue(1);
    // $this->addFieldMapping('field_phones', 'telephone');
    // $this->addFieldMapping('field_fax', 'fax');
  }

  public function prepare($profile, stdClass$row) {
    module_load_include('inc', 'addressfield', 'addressfield.administrative_areas');
    $administrative_areas = addressfield_get_administrative_areas($row->country_id);
    if (!empty($administrative_areas)) {
      $row->administrative_area = array_search($row->region, $administrative_areas);
    }
    $row->firstname = ucfirst($row->firstname);
    $row->lastname = ucfirst($row->lastname);
    $name = $row->firstname . ' ' . $row->lastname;
    // There are too many fields to do this through a field handler.
    // @todo Not comfortable with assuming LANGUAGE_NONE.
    $profile->commerce_customer_address[LANGUAGE_NONE][0] = array(
      'xnl' => '<NameDetails PartyType="Person"><NameLine>' . $name . '</NameLine></NameDetails>',
      'name_line' => $name,
      'first_name' => $row->firstname,
      'last_name' => $row->lastname,
      'administrative_area' => $row->administrative_area,
      'country' => $row->country_id,
      'thoroughfare' => $row->street,
      'locality' => $row->city,
      'postal_code' => $row->postcode,
      'organisation_name' => $row->company,
    );
    // Check for a duplicate profile for this user.
    $q = new EntityFieldQuery();
    $q->entityCondition('entity_type', 'commerce_customer_profile')->entityCondition('bundle', $profile->type)->fieldCondition('commerce_customer_address', 'first_name', $row->firstname, '=')->fieldCondition('commerce_customer_address', 'last_name', $row->lastname, '=')->fieldCondition('commerce_customer_address', 'organisation_name', $row->company, '=')->fieldCondition('commerce_customer_address', 'thoroughfare', $row->street, '=')->fieldCondition('commerce_customer_address', 'locality', $row->city, '=')->fieldCondition('commerce_customer_address', 'postal_code', $row->postcode, '=')->range(0, 1);
    $results = $q->execute();
    if (isset($results['commerce_customer_profile'])) {
      // We found a duplicate, mark this as a revision of the existing profile,
      // this keeps the migrate map references intactg.
      $profile->profile_id = current(array_keys($results['commerce_customer_profile']));
      $profile->revision   = TRUE;
      $profile->log        = 'From Magento user id #' . $row->customer_id;
    }
  }

  public function prepareRow($row) {
    // $phone          = $row->telephone;
    // $pattern        = '/[^a-zA-Z0-9]+/';
    // $phone          = preg_replace($pattern, "", $phone);
    // $row->telephone = array($phone);
    // $fax            = $row->fax;
    // $fax            = preg_replace($pattern, "", $fax);
    // $row->fax       = $fax;
    $row->created_at = strtotime($row->created_at);
    $row->updated_at = strtotime($row->updated_at);
  }
}

