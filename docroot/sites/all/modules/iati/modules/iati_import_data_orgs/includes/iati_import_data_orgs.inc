<?php

class DataOrgs extends XMLMigration {
  /**
   * Call to the class constructor.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();

    // Instantiate the map.
    $fields = array(
      'iati-identifier' => 'iati-identifier',
      'name' => 'name',
    );

    $item_ID_xpath = '@ref|text()';  
    //$item_ID_xpath = '@ref|@type';          // relative to item_xpath
    //$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    if(!empty($arguments['source_file'])){
      $items_class = new OAMMigrateOrgItemsXML($arguments['source_file'], $this->item_xpath, $item_ID_xpath);
    }else {
    	$items_class = new OAMMigrateOrgItemsXML("http://iati.dfid.gov.uk/iati_files/Country/DFID-Ascension-Island-AC.xml", $this->item_xpath, $item_ID_xpath);
    }
    $this->source = new MigrateSourceMultiItems($items_class, $fields);    
    $this->destination = new MigrateDestinationNode('iati_organisation');
    // Instantiate the map.
    $this->map = new MigrateSQLMap($this->machineName,
    array(
      'iati_identifier' => array(
      'type' => 'varchar',
      'length' => 64,
      'not null' => TRUE,
      'description' => 'iati_identifier',
      'alias' => 'c',
      ),
    ),
    MigrateDestinationNode::getKeySchema()
    );
    // Instantiate the field mapping.
    //$this->addFieldMapping('field_iati_identifier', 'iati_identifier')->xpath('@ref');
    $this->addFieldMapping('field_iati_identifier', 'iati_identifier');
    $this->addFieldMapping('title', 'name')->xpath('text()');
  }

  public function prepare($entity, $row) {
    if(empty($row->iati_identifier)) {
      $tt=0;
      //$row->iati_identifier = str_ireplace(" ", "_",strtoupper((string) $row->xml));
      //$entity->field_iati_identifier['und'] [0] ['value'] = str_ireplace(" ", "_",strtoupper((string) $row->xml));
      //$entity->iati_identifier =str_ireplace(" ", "_",strtoupper((string) $row->xml));
    }    
  }
  public function prepareRow($row) {
    $tt=0;

  }  
  
}



class DataParticipateOrg extends DataOrgs {
  /**
   * Call to the class constructor.
   */
  public function __construct(array $arguments) {
    //$this->arguments = $arguments;

    // Do some general administration.
    $this->description = t('Imports data for the organization from an activity.');
    
    //$items_url = 'http://localhost/test/DFID-Ukraine-UA.xml';
    $this->item_xpath = '/iati-activities/iati-activity/participating-org';  // relative to document
    
    parent::__construct($arguments);
  
  }

  /**
   * Construct the machine name from the source file name.
   */
  protected function generateMachineName($class_name = NULL) {
    return 'DataOrganisation';
  }
  
}


class DataReportOrg extends DataOrgs {
  /**
   * Call to the class constructor.
   */
  public function __construct(array $arguments) {
    
    // Do some general administration.
    $this->description = t('Imports data for the reporting organization from an activity.');
    $this->item_xpath = '/iati-activities/iati-activity/reporting-org';  // relative to document

    parent::__construct($arguments);
  }

  /**
   * Construct the machine name from the source file name.
   */
  protected function generateMachineName($class_name = NULL) {
    return 'DataOrganisation';
  }
  
//   public function prepare($entity, $row) {
//     parent::prepare($entity, $row);
//   }
//   public function prepareRow($row) {
//     $tt=0;
//   }  
}



class OAMMigrateOrgItemsXML extends MigrateItemsXML {
  /**
   * Call to the class constructor.
   */
  public function __construct($xml_url, $item_xpath='item', $itemID_xpath='id') {
    parent::__construct($xml_url, $item_xpath, $itemID_xpath);
  }

  /**
   * Override MigrateItemsXML::getItemID,
   * to allow concatenation of multiple itemIDXpath elements.
   *
   * @param int $itemXML
   *  The itemXML based on itemIDXpath.
   *
   * @return string
   */
  protected function getItemID($itemXML) {
    $value = NULL;
    if ($itemXML) {

        $ref =  $itemXML->xpath('@ref');
        if(count($ref)>0 && !empty($ref[0]['ref'])) {
          $value = (string) $ref[0];
        }
        else {
          $result = $itemXML->xpath('text()');
          $name =(string) $result[0];
          $value = str_replace(" ","_", strtoupper($name));
        }  

    }
    return  $value ;
    //return $value;
  }
}



