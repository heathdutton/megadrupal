<?php
/**
 * @file
 * Migrate Contacts into Party
 */

/**
 * Migrate contacts from a CSV file into 'Contact' Profile2s
 */
class PartyB2BContactProfile2Migration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migration of P1 contacts from CSV to profile2 entities');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'contact_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationProfile2::getKeySchema()
    );

    $this->source = new MigrateSourceCSV(__DIR__ . '/contact.csv', $this->csvcolumns(), array('header_rows' => 1));

    // MigrateDestination
    $this->destination = new MigrateDestinationProfile2('party_contact');

    // Field Mappings
    $this->addFieldMapping('uid')->defaultValue(0);
    $this->addFieldMapping('party_contact_name', 'contact_first_name')
      ->arguments(array(
        'family' => array('source_field' => 'contact_last_name'),
      ));
    $this->addFieldMapping('party_contact_email', 'contact_email');
    $this->addFieldMapping('party_contact_phone', 'contact_phone');

    // Address
    $address_arguments= array(
        'thoroughfare' => array('source_field' => 'contact_address1'),
        'premise' => array('source_field' => 'contact_address2'),
        'locality' => array('source_field' => 'contact_city'),
        'administrative_area' => array('source_field' => 'contact_state'),
        'postal_code' => array('source_field' => 'contact_zip'),
    );
    $this->addFieldMapping('party_contact_address', 'contact_country')
        ->arguments($address_arguments);

    $this->addUnmigratedDestinations(array('revision_uid', 'language', 'path'));
  }

  /**
   * Define the columns in the CSV
   */
  function csvcolumns() {
    $columns[0]  = array('org_id', 'Organization ID');
    $columns[11] = array('contact_id', 'Contact ID');
    $columns[12] = array('contact_first_name', 'Contact First Name');
    $columns[13] = array('contact_last_name', 'Contact Last Name');
    $columns[14] = array('contact_email', 'Contact Email');
    $columns[15] = array('contact_address1', 'Contact Address 1');
    $columns[16] = array('contact_address2', 'Contact Address 2');
    $columns[17] = array('contact_city', 'Contact City');
    $columns[18] = array('contact_state', 'Contact State');
    $columns[19] = array('contact_zip', 'Contact Zip');
    $columns[20] = array('contact_country', 'Contact Country');
    $columns[21] = array('contact_phone', 'Contact Phone');
    return $columns;
  }
}

/**
 * Create Parties for the Profile2s created by PartyB2BContactProfileMigration
 */
class PartyB2BContactPartyMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migration of organizations from Profile2 to Party entities.');
    $this->dependencies = array('PartyB2BContactProfile2', 'PartyB2BOrganizationParty');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'contact_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('party')
    );

    // MigrateSource
    // This should be the same CSV file as the MigrateSource for PartyOrganizationProfile2 migration.
    $this->source = new MigrateSourceCSV(__DIR__ . '/contact.csv', $this->csvcolumns(), array('header_rows' => 1));

    // MigrateDestination
    $this->destination = new MigrateDestinationEntityAPI(
      'party',  // Entity type
      'party'   // bundle
    );

    // Field Mappings
    $this->addFieldMapping('profile2_id', 'contact_id')
      ->sourceMigration('PartyB2BContactProfile2');
    $this->addfieldMapping('org_id', 'org_id')
      ->sourceMigration('PartyB2BOrganizationParty');
  }

  /**
   * Define csv columns.
   *
   * @return multitype:string multitype:string
   */
  function csvcolumns() {
    $columns[0] = array('org_id', 'Organization ID');
    $columns[11] = array('contact_id', 'Contact ID');
    return $columns;
  }

  /**
   * Attach profile2 from PartyOrganizationProfile2Migration
   */
  function complete($party, $row) {
    // Set the contact hat.
    $hat = party_hat_load('contact');
    party_hat_hats_assign($party, array($hat));

    // Attach the profile 2
    $profile2 = profile2_load($party->profile2_id);
    party_attach_entity($party, $profile2, 'profile2_party_contact');

    // Make a relation
    $relation = relation_create('employee', array(
      array('entity_type' => 'party', 'entity_id' => $party->pid),
      array('entity_type' => 'party', 'entity_id' => $party->org_id),
    ));

    relation_save($relation);
  }
}
