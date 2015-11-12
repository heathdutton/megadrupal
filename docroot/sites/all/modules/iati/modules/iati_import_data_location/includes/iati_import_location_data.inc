<?php 
/**
 * @file.
 * Import the data for the organisation.
 */
class LatLongLocationData extends XMLMigration {
  /**
   * Call to the class constructor.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports data for latitude and longitude.');

    // Instantiate the map.
     $fields = array(
      'lid' => 'lid',
      'name' => 'name',
      'latitude' => 'latitude',
      'longitude' => 'longitude',
      'country_code' => 'country_code',
      'activity' => 'activity',
    );
  //    $items_url = 'http://foreignassistance.gov/IATI/Activities/Nepal/Nepal.xml';
    $item_xpath = '/iati-activities/iati-activity/location';  // relative to document
  //  $item_ID_xpath = 'administrative/@country';          // relative to item_xpath
    $item_ID_xpath = 'parent::*/iati-identifier | administrative | administrative/@country | administrative/@adm1 | administrative/@adm2';  // relative to item_xpath
    //$items_class = new OAMMigrateItemsXML($arguments['source_file'], $item_xpath, $item_ID_xpath);
    if(isset($arguments['source_file']) && !empty($arguments['source_file'])){
      $items_class = new OAMMigrateItemsXML($arguments['source_file'], $item_xpath, $item_ID_xpath);
    }else{
      $items_class = new OAMMigrateItemsXML("http://iati.dfid.gov.uk/iati_files/Country/DFID-Ascension-Island-AC.xml", $item_xpath, $item_ID_xpath);
    }
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
    
    $this->destination = new MigrateDestinationEntityAPI('iati_location', 'iati_location');
    // Instantiate the map.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'name' => array(
          'type' => 'varchar',
          'length' => 128,
          'not null' => TRUE,
          'description' => 'name',
          'alias' => 'c',
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('iati_location')
    );
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping(NULL, 'lid')->xpath('administrative | administrative/@country | administrative/@adm1 | administrative/@adm2');
    $this->addFieldMapping(NULL, 'latitude')->xpath('coordinates/@latitude');
    $this->addFieldMapping(NULL, 'longitude')->xpath('coordinates/@longitude');
    $this->addFieldMapping(NULL, 'country_code')->xpath('administrative/@country');
    $this->addFieldMapping(NULL, 'activity')->xpath('parent::*/iati-identifier');
    $geo_arguments = array(
      'lat' => array('source_field' => 'latitude'),
      'lon' => array('source_field' => 'longitude'),
      'geo_type' => array('source_field', 'geo_type'),
      'input_format' => array('source_field' => 'input_format'),
    );
    // The geometry type should be passed in as the primary value.
    $this->addFieldMapping('field_iati_geofield', 'geo_type')->arguments($geo_arguments);
  }

  /**
   * Construct the machine name from the source file name.
   */
  protected function generateMachineName($class_name = NULL) {
    $class_name = 'dataactivitybudget';
    return parent::generateMachineName($class_name);
  }

  public function prepareRow($row) {
  	$row->geo_type = 'point';
  	$row->input_format = 'lat/lon';
  }
  
  public function prepare($entity, $source_row) {
	//$entity->name = implode('^', $source_row->lid);
  	if (isset($source_row->country_code)  && !empty($source_row->country_code)) {
      $source_row->source_type = 'term';
       $adminareamaptable ="migrate_map_codelistlocationlevel1";
      $adminareas = db_select($adminareamaptable, 'mt')
        ->fields('mt', array('sourceid1', 'destid1'))
        ->condition('sourceid1', $source_row->country_code)
        ->execute()
        ->fetchAssoc();
  	  if ($adminareas === FALSE) {
        drupal_set_message('You first have to load the administrative areaxs.');
        return FALSE;
      }
      $entity->iati_admin_boundaries['en'][0]['tid'] = $adminareas['destid1'];
  	}

  	$activitymaptable = str_replace('latlonglocationdata', 'dataactivity', $this->getMap()->getMapTable());
  	$activities = db_select($activitymaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $source_row->activity)
      ->execute()
      ->fetchAssoc();
  	if ($activities === FALSE) {
      drupal_set_message('You first have to load data for activities.');
      return FALSE;
  	}
  	$entity->field_iati_activity_ref['LANGUAGE_NONE'][0]['target_id'] = $activities['destid1'];
  }

}
