<?php
/**
 * @file.
 * Import the data for the activity.
 */
class DataActivity extends XMLMigration {
  /**
   * Call to the class constructor.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports data for the activity.');

    // Instantiate the map.
    $fields = array(
      'iati-identifier' => 'iati-identifier',
      'title' => 'title',
      'default-currency' => 'default-currency',
      'other-identifier' => 'other-identifier',
      'description' => 'description',
      'actual_date_start' => 'actual_date_start' ,
      'actual_date_end' => 'actual_date_end' ,
      'planned_date_start' => 'planned_date_start' ,
      'planned_date_end' => 'planned_date_end' ,
      'finance_type' => 'finance_type' ,
      'sector' => 'sector',
      'owner_ref' => 'owner_ref',
      'owner_name' => 'owner_name',
      'aid_type' => 'aid_type',
      'collabo_type' =>'collabo_type',
      'flow_type' => 'flow_type',
      'status' => 'status',
      'document_link' => 'document_link',
    );
//    $items_url = 'http://projects.dfid.gov.uk/iati_files/Country/DFID-Ascension-Island-AC.xml';
    $item_xpath = '/iati-activities/iati-activity';  // relative to document
    $item_ID_xpath = 'iati-identifier';          // relative to item_xpath
    dd($this);
    dd($this->getSource());
   if (isset($arguments['source_file']) && !empty($arguments['source_file'])) {
    	$this->source = new MigrateSourceXML($arguments['source_file'], $item_xpath, $item_ID_xpath, $fields);
    }
    else {
    	$this->source = new MigrateSourceXML("http://iati.dfid.gov.uk/iati_files/Country/DFID-Ascension-Island-AC.xml", $item_xpath, $item_ID_xpath, $fields);
    }
    $this->destination = new MigrateDestinationNode('iati_activity');
    // Instantiate the map.
    $this->map = new MigrateSQLMap($this->machineName,
    array(
      'iati-identifier' => array(
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'description' => 'iati-identifier',
        'alias' => 'c',
      ),
    ),
    MigrateDestinationNode::getKeySchema()
    );
    // Instantiate the field mapping.
    $this->addFieldMapping('field_iati_identifier', 'iati-identifier')->xpath('iati-identifier');
    $this->addFieldMapping('title', 'title')->xpath('title');
    $this->addFieldMapping('field_iati_default_currency', 'default-currency')->xpath('@default-currency');
    $this->addFieldMapping('field_other_identifier', 'other-identifier')->xpath('other-identifier');
    $this->addFieldMapping('field_iati_activity_description', 'description')->xpath('description');
    $this->addFieldMapping('field_iati_activity_actual_date_start', 'actual_date_start')->xpath('activity-date[@type="start-actual"]');
    $this->addFieldMapping('field_iati_activity_actual_date_end', 'actual_date_end')->xpath('activity-date[@type="end-actual"]');
    $this->addFieldMapping('field_iati_activity_planned_date_start', 'planned_date_start')->xpath('activity-date[@type="start-planned"]');
    $this->addFieldMapping('field_iati_activity_planned_date_end', 'planned_date_end')->xpath('activity-date[@type="end-planned"]');
    $this->addFieldMapping('field_iati_activity_finance_type', 'finance_type');
    $this->addFieldMapping('iati_activity_sector', 'sector');
    $this->addFieldMapping('field_iati_owner_ref', 'owner_ref')->xpath('other-identifier/@owner-ref');
    $this->addFieldMapping('field_iati_owner_name', 'owner_name')->xpath('other-identifier/@owner-name');
    $this->addFieldMapping('field_iati_activity_aid_type', 'aid_type');
    $this->addFieldMapping('field_iati_activity_collabo_type', 'collabo_type');
    $this->addFieldMapping('field_iati_activity_flow_type', 'flow_type');
    $this->addFieldMapping('field_iati_activity_status', 'status');
    $this->addFieldMapping('field_field_iati_activity_doc', 'document_link')->xpath('document-link/@url');
  }

  /**
   * Construct the machine name from the source file name.
   */
  /*
  protected function generateMachineName($class_name = NULL) {
    return 'dataactivity' . str_replace('-', '', drupal_strtolower(pathinfo($this->arguments['source_file'], PATHINFO_FILENAME)));
  }
*/
  public function prepareRow($row) {
    $maptable = 'dataactivity' . str_replace('-', '', drupal_strtolower(pathinfo($this->arguments['source_file'], PATHINFO_FILENAME)));
    if(isset($row->xml->{'default-finance-type'}['code'])) {
      $row->finance_type = iati_import_data_get_activity_term($row->xml->{'default-finance-type'}['code'], $maptable, 'financialtypelevel1', $this->getMap()->getMapTable());
    }
    if(isset($row->xml->{'default-aid-type'}['code'])){
    	$row->aid_type = iati_import_data_get_activity_term($row->xml->{'default-aid-type'}['code'], $maptable, 'aidtypelevel1', $this->getMap()->getMapTable());
    }
   /*  if(isset($row->xml->{'activity-status'}['code'])){
    	$status = (string)$row->xml->{'activity-status'}->attributes()->code;
    	$row->status = iati_import_data_get_activity_term($status, $maptable, 'activitystatus', $this->getMap()->getMapTable());
    } */
    if(isset($row->xml->{'default-flow-type'}['code'])){
    	$flowtype = (string)$row->xml->{'default-flow-type'}->attributes()->code;
    	$row->flow_type = iati_import_data_get_activity_term($flowtype, $maptable, 'activityflowtype', $this->getMap()->getMapTable());
    }
    if(isset($row->xml->{'collaboration-type'}['code'])){
    	$collabo = (string)$row->xml->{'collaboration-type'}->attributes()->code;
    	$row->collabo_type[] = iati_import_data_get_activity_term($collabo, $maptable, 'collaborationtype', $this->getMap()->getMapTable());
    }
    if (isset($row->xml->sector)){
      foreach($row->xml->sector as $sector) {
        $sector_code = (string)$sector->attributes()->code;
        $row->sector[] = iati_import_data_get_activity_term($sector_code, $maptable, 'sectorlevel1', $this->getMap()->getMapTable());
      }

    }
  }
  public function prepare($entity, $row) {
    $entity->field_iati_activity_actual_date[LANGUAGE_NONE][0]['value'] = $row->actual_date_start;
    $entity->field_iati_activity_actual_date[LANGUAGE_NONE][0]['value2'] = $row->actual_date_end;
    $entity->field_iati_activity_planned_date[LANGUAGE_NONE][0]['value'] = $row->planned_date_start;
    $entity->field_iati_activity_planned_date[LANGUAGE_NONE][0]['value2'] = $row->planned_date_end;
  }
}
