
<?php

class RelationOrgRole extends XMLMigration {

  //private $item_xpath;
  //private $item_ID_xpath;
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();

    // Do some general administration
    //$this->description = t('Imports relations of type funding organisation role.');

    $fields = array(
        'source_type', 'source_type',
        'source_id', 'source_id',
        'destination_type', 'destination_type',
        'destination_id', 'destination_id',
        'role_tid', 'role_tid',
        'iati_identifier', 'iati_identifier',
    );

    $options = array();
    if (isset($arguments['source_file']) && !empty($arguments['source_file'])) {
    $items_class = new OAMMigrateItemsXML($arguments['source_file'], $this->item_xpath, $this->item_ID_xpath);
    }
    else {
        $items_class = new OAMMigrateItemsXML("https://www.irishaid.ie/media/irishaid/allwebsitemedia/30whatwedo/iati-12-13-data.xml", $this->item_xpath, $this->item_ID_xpath);
    }
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
    $this->destination = new MigrateDestinationRelation('iati_organisation_role');

    $this->map = new MigrateSQLMap($this->machineName,
        array(
            'source_type' => array(
                'type' => 'varchar',
                'length' => 255,
                'not null' => TRUE,
                'description' => 'source_type',
                'alias' => 'st',
            ),
            'source_id' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'source_id',
                'alias' => 'si',
            ),
            'destination_type' => array(
                'type' => 'varchar',
                'length' => 255,
                'not null' => TRUE,
                'description' => 'destination_type',
                'alias' => 'dt',
            ),
            'destination_id' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'destination_id',
                'alias' => 'di',
            ),
            'role_tid' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'role_tid',
                'alias' => 'rt',
            ),
        ),
        MigrateDestinationRelation::getKeySchema()
    );
    //$sourcemaptable = str_replace('relationorgrolefunding', 'dataactivity', $this->generateMachineName());
    $this->addFieldMapping(NULL, 'iati_identifier')->xpath('parent::*/iati-identifier'); //->defaultValue('1');
    $this->addFieldMapping(NULL, 'role_tid');
    $this->addFieldMapping(NULL, 'source_id');
    $this->addFieldMapping(NULL, 'destination_id');
    $this->addFieldMapping(NULL, 'destination_type');
    $this->addFieldMapping(NULL, 'source_type');

  }

  /**
   * Construct the machine name from the source file name.
   */





  public function prepareKey($source_key, $row) {
    $key = array();
    foreach ($source_key as $field_name => $field_schema) {
      if(isset($row->$field_name)){
        $key[$field_name] = $row->$field_name;
      }
      else {
        $key[$field_name]='';
      }
    }
    return $key;
  }





  public function prepare(stdClass $relation, stdClass $source_row) {

    $source_row->source_type='node';
    $source_row->destination_type='node';

    $orgcode= (string) $source_row->xml->attributes()->ref;
    if(empty($orgcode)) {
      $orgcode = str_replace(" ","_", strtoupper((string) $source_row->xml));
    }

    $destinationmaptable = 'migrate_map_dataorganisation';
    $destinations = db_select($destinationmaptable, 'mt')
    ->fields('mt', array('sourceid1', 'destid1'))
    ->condition('sourceid1', $orgcode)
    ->execute()
    ->fetchAssoc();
    if ($destinations === FALSE) {
      drupal_set_message('You first have to load the required organisations.'.strtoupper((string) $source_row->xml));
      return FALSE;
    }
    $source_row->destination_id = $destinations['destid1'];



    $sourcemaptable = 'migrate_map_dataactivity';
    $sources = db_select($sourcemaptable, 'mt')
    ->fields('mt', array('sourceid1', 'destid1'))
    //->condition('sourceid1', 1)
    ->condition('sourceid1', $source_row->iati_identifier)
    ->execute()
    ->fetchAssoc();
    if ($sources === FALSE) {
      drupal_set_message('You first have to load the activities.');
      return FALSE;
    }
    $source_row->source_id = $sources['destid1'];



    $relation->endpoints[LANGUAGE_NONE] = array(
        array(
            'entity_type' => $source_row->source_type,
            'entity_id' => $source_row->source_id,
        ),
        array(
            'entity_type' => $source_row->destination_type,
            'entity_id' => $source_row->destination_id,
        ),
    );
    $relation->field_organisation_role[LANGUAGE_NONE][0]['tid'] = $source_row->role_tid;

  }
}

class RelationOrgRoleFunding extends RelationOrgRole {

  public function __construct(array $arguments) {
    // Do some general administration
    $this->description = t('Imports relations of type funding organisation role.');

    $this->item_xpath = "/iati-activities/iati-activity/participating-org[contains(translate(@role, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'),'funding')]";  // relative to document
    $this->item_ID_xpath = 'parent::*/iati-identifier | @ref ';          // relative to item_xpath
    //$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);

    parent::__construct($arguments);

  }

  /**
   * Construct the machine name from the source file name.
   */
  public function prepareRow($row) {

    $rolemaptable = 'migrate_map_organisationrole';
    $roles = db_select($rolemaptable, 'mt')
    ->fields('mt', array('sourceid1', 'destid1'))
    ->condition('sourceid1', 'Funding')
    ->execute()
    ->fetchAssoc();
    if ($roles === FALSE) {
      drupal_set_message('You first have to load the Organisation Role code list.');
      return FALSE;
    }
    $row->role_tid = $roles['destid1'];
  }
}


class RelationOrgRoleExtending extends RelationOrgRole {

  public function __construct(array $arguments) {
    //$this->arguments = $arguments;


    // Do some general administration
    $this->description = t('Imports relations of type extending organisation role.');

    $this->item_xpath = "/iati-activities/iati-activity/participating-org[contains(translate(@role, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'),'extending')]";  // relative to document
    $this->item_ID_xpath = 'parent::*/iati-identifier | @ref ';          // relative to item_xpath
    //$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);

    parent::__construct($arguments);

  }

  /**
   * Construct the machine name from the source file name.
   */
  public function prepareRow($row) {

    $rolemaptable = 'migrate_map_organisationrole';
    $roles = db_select($rolemaptable, 'mt')
    ->fields('mt', array('sourceid1', 'destid1'))
    ->condition('sourceid1', 'Extending')
    ->execute()
    ->fetchAssoc();
    if ($roles === FALSE) {
      drupal_set_message('You first have to load the Organisation Role code list.');
      return FALSE;
    }
    $row->role_tid = $roles['destid1'];



  }

}


class RelationOrgRoleImplementing extends RelationOrgRole {

  public function __construct(array $arguments) {
    //$this->arguments = $arguments;


    // Do some general administration
    $this->description = t('Imports relations of type Implementing organisation role.');

    $this->item_xpath = "/iati-activities/iati-activity/participating-org[contains(translate(@role, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'),'implementing')]";;  // relative to document
    $this->item_ID_xpath = 'parent::*/iati-identifier | @ref ';          // relative to item_xpath
    //$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);

    parent::__construct($arguments);

  }

  /**
   * Construct the machine name from the source file name.
   */
  public function prepareRow($row) {

    $rolemaptable = 'migrate_map_organisationrole';
    $roles = db_select($rolemaptable, 'mt')
    ->fields('mt', array('sourceid1', 'destid1'))
    ->condition('sourceid1', 'Implementing')
    ->execute()
    ->fetchAssoc();
    if ($roles === FALSE) {
      drupal_set_message('You first have to load the Organisation Role code list.');
      return FALSE;
    }
    $row->role_tid = $roles['destid1'];
  }

}


class RelationOrgRoleAccountable extends RelationOrgRole {

  public function __construct(array $arguments) {
    //$this->arguments = $arguments;


    // Do some general administration
    $this->description = t('Imports relations of type funding organisation role.');

    $this->item_xpath = "/iati-activities/iati-activity/participating-org[contains(translate(@role, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'),'accountable')]";  // relative to document
    $this->item_ID_xpath = 'parent::*/iati-identifier | @ref ';          // relative to item_xpath
    //$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);

    parent::__construct($arguments);

  }

  /**
   * Construct the machine name from the source file name.
   */
  public function prepareRow($row) {

    //dpm($row);
    $activityid = 0;
    //return FALSE;
    $rolemaptable = 'migrate_map_organisationrole';
    $roles = db_select($rolemaptable, 'mt')
    ->fields('mt', array('sourceid1', 'destid1'))
    ->condition('sourceid1', 'Accountable')
    ->execute()
    ->fetchAssoc();
    if ($roles === FALSE) {
      drupal_set_message('You first have to load the Organisation Role code list.');
      return FALSE;
    }
    $row->role_tid = $roles['destid1'];
  }

}
