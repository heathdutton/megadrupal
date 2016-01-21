<?php
/**
 * @file
 * Support for migrate module
 */

// @TODO: remove and commit to migrate_extras.module,
class MigratePhoneFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('phone'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    foreach ($values as $delta => $value) {
      $item = array();

      $item['value'] = $value;
      $return[$language][$delta] = $item;
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}

class PartyB2BOrganizationProfile2Migration extends Migration {
  /**
   * Set up the migration.
   */
  public function __construct() {
    parent::__construct();

    $this->description = t('Migration of organizations from CSV to Profile2 entities.');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'org_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationProfile2::getKeySchema()
    );

    $this->source = new MigrateSourceCSV(__DIR__ . '/org.csv', $this->csvcolumns(), array('header_rows' => 1));

    // MigrateDestination
    $this->destination = new MigrateDestinationProfile2('party_org');

    // Field Mappings
    $this->addFieldMapping('uid')->defaultValue(0);
    $this->addFieldMapping('party_org_name', 'org_name');
    $this->addFieldMapping('party_org_phone', 'org_phone');
    $this->addFieldMapping('party_org_web', 'org_web');
    $this->addFieldMapping('party_org_email', 'org_email');

    // Map the addressfield components.
    $address_arguments= array(
      'thoroughfare' => array('source_field' => 'org_address1'),
      'premise' => array('source_field' => 'org_address2'),
      'locality' => array('source_field' => 'org_city'),
      'administrative_area' => array('source_field' => 'org_state'),
      'postal_code' => array('source_field' => 'org_zip'),
    );
    $this->addFieldMapping('party_org_address', 'org_country')
      ->arguments($address_arguments);
  }

  /**
   * Define columns in csv file
   *
   * @return multitype:string multitype:string
   */
  function csvcolumns() {
    $columns[0] = array('org_id', 'Organization ID');
    $columns[1] = array('org_name', 'Organization Name');
    $columns[2] = array('org_web', 'Organization Web Address');
    $columns[3] = array('org_email', 'Organization Email');
    $columns[4] = array('org_address1', 'Organization Address 1');
    $columns[5] = array('org_address2', 'Organization Address 2');
    $columns[6] = array('org_city', 'Organization City');
    $columns[7] = array('org_state', 'Organization State');
    $columns[8] = array('org_zip', 'Organization Zip');
    $columns[9] = array('org_country', 'Organization Country');
    $columns[10] = array('org_phone', 'Organization Phone');
    return $columns;
  }

  /**
   * prepare()
   */
  function prepareRow($row) {
  }

  function complete($party, $row) {
  }
}

/**
 * Dependant migration that creates Party's from Profile2's.
 */
class PartyB2BOrganizationPartyMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migration of organizations from Profile2 to Party entities.');
    $this->dependencies = array('PartyB2BOrganizationProfile2');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'org_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('party')
    );

    // MigrateSource
    // This should be the same CSV file as the MigrateSource for PartyOrganizationProfile2 migration.
    $this->source = new MigrateSourceCSV(__DIR__ . '/org.csv', $this->csvcolumns(), array('header_rows' => 1));

    // MigrateDestination
    $this->destination = new MigrateDestinationEntityAPI(
      'party',  // Entity type
      'party'   // bundle
    );

    // Field Mappings
    $this->addFieldMapping('profile2_id', 'org_id')
      ->sourceMigration('PartyB2BOrganizationProfile2');
    $this->addFieldMapping('label', 'org_name');
  }

  /**
   * Define cs columns.
   *
   * @return multitype:string multitype:string
   */
  function csvcolumns() {
    $columns[0] = array('org_id', 'Organization ID');
    $columns[] = array('org_name', 'Organization Name');
    return $columns;
  }

  /**
   * Attach profile2 from PartyOrganizationProfile2Migration
   */
  function complete($party, $row) {
    // Set the contact hat.
    $hat = party_hat_load('org');
    party_hat_hats_assign($party, array($hat));

    $profile2 = profile2_load($party->profile2_id);
    party_attach_entity($party, $profile2, 'profile2_party_org');
  }
}
