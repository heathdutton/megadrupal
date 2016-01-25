<?php

/**
 * @file.
 * Import iati budget data.
 */

class DataBudget extends XMLMigration {
  /**
   * Call to the class constructor.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports budget data for the organisations and activities.');

    // Instantiate the field map.
    $fields = array(
      'bid' => 'bid',
      'value_amount' => 'value_amount',
      'value_date' => 'value_date',
      'period_start' => 'period_start',
      'period_start_text' => 'period_start_text',
      'period_end' => 'period_end',
      'period_end_text' => 'period_end_text',
      'parent_entity' => 'parent_entity',
    );
    $this->fields = $fields;

    $this->destination = new MigrateDestinationEntityAPI('iati_budget', 'iati_budget');
    
    // Instantiate the key map.
    $this->map = new MigrateSQLMap(
      $this->machineName,
      array(
        'bid' => array(
          'type' => 'varchar',
          'length' => '64',
          'not null' => TRUE,
          'description' => 'The unique budget id'
        )
      ),
      MigrateDestinationEntityAPI::getKeySchema('iati_budget')
    );

    // Instantiate the field mapping.
    $this->addFieldMapping('value_amount', 'value_amount')->xpath('value');
    $this->addFieldMapping('value_date', 'value_date')->xpath('value/@value-date');
    $this->addFieldMapping('period_start', 'period_start')->xpath('period-start/@iso-date');
    $this->addFieldMapping('period_start_text', 'period_start_text')->xpath('period-start');
    $this->addFieldMapping('period_end', 'period_end')->xpath('period-end/@iso-date');
    $this->addFieldMapping('period_end_text', 'period_end_text')->xpath('period-end');
    $this->addFieldMapping('parent_entity', 'parent_entity')->xpath('parent::*/iati-identifier');
  }

  /**
   * Construct the machine name from the source file name.
   */
  /* protected function generateMachineName($class_name = NULL) {
    return $class_name . str_replace('-', '', drupal_strtolower(pathinfo($this->arguments['source_file'], PATHINFO_FILENAME)));
  } */

  public function prepare($entity, $source_row) {
    $entity->value_date = strtotime($source_row->value_date);
    $entity->period_start = strtotime($source_row->period_start);
    $entity->period_end = strtotime($source_row->period_end);
  }
}

class DataActivityBudget extends DataBudget {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    // Do some general administration.
    $this->description = t('Imports budget data for activities.');

    // Instantiate the field map.
    $fields = $this->fields;
    $fields['budget_type'] = 'budget_type';

    $item_xpath = '/iati-activities/iati-activity/budget';  // relative to document
    $item_ID_xpath = 'parent::*/iati-identifier | period-start/@iso-date';  // relative to item_xpath
    if (isset($arguments['source_file']) && !empty($arguments['source_file'])) {
      $items_class = new OAMMigrateItemsXML($arguments['source_file'], $item_xpath, $item_ID_xpath);
    }
    else {
      $items_class = new OAMMigrateItemsXML("http://selfhelpafrica.net/sha_iati_org.xml", $item_xpath, $item_ID_xpath);
    }
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
    
    // Instantiate the field mapping.
    $this->addFieldMapping(NULL, 'budget_type')->xpath('@type');
  }

  /**
   * Overrides generateMachineName::DataBudget().
   */
  protected function generateMachineName($class_name = NULL) {
    $class_name = 'dataactivitybudget';
    return parent::generateMachineName($class_name);
  }

  public function prepare($entity, $source_row) {
    parent::prepare($entity, $source_row);

    $activitymaptable = str_replace('dataactivitybudget', 'dataactivity', $this->getMap()->getMapTable());
    $activities = db_select($activitymaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $source_row->parent_entity)
      ->execute()
      ->fetchAssoc();
    if ($activities === FALSE) {
      drupal_set_message('You first have to load the activities.');
      return FALSE;
    }
    $entity->field_iati_activity_ref['LANGUAGE_NONE'][0]['target_id'] = $activities['destid1'];

    if (!is_numeric($source_row->budget_type)) {
      $budget_types = array(
        'Original' => '1',
        'Revised' => '2',
      );
      $budget_type = $budget_types[$source_row->budget_type];
    }
    else {
      $budget_type = $source_row->budget_type;
    }
    $budgettypemaptable = str_replace('dataactivitybudget' . str_replace('-', '', drupal_strtolower(pathinfo($this->arguments['source_file'], PATHINFO_FILENAME))), 'codelistbudgettype', $this->getMap()->getMapTable());
    $budgettypes = db_select($budgettypemaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $budget_type)
      ->execute()
      ->fetchAssoc();
    if ($budgettypes === FALSE) {
      drupal_set_message('You first have to load the budget types.');
      return FALSE;
    }
    $entity->field_budget_type['LANGUAGE_NONE'][0]['tid'] = $budgettypes['destid1'];
  }
}

class DataTotalBudget extends DataBudget {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    // Do some general administration.
    $this->description = t('Imports total budget data for organisations.');

    // Instantiate the field map.
    $fields = $this->fields;

    $item_xpath = '/iati-organisations/iati-organisation/total-budget';  // relative to document
    $item_ID_xpath = 'parent::*/iati-identifier | period-start/@iso-date';  // relative to item_xpath
    if (isset($arguments['source_file']) && !empty($arguments['source_file'])) {
      $items_class = new OAMMigrateItemsXML($arguments['source_file'], $item_xpath, $item_ID_xpath);
    }
    else {
      $items_class = new OAMMigrateItemsXML("http://iati.dfid.gov.uk/iati_files/Country/DFID-Ascension-Island-AC.xml", $item_xpath, $item_ID_xpath);
    }
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
  }

  /**
   * Overrides generateMachineName::DataBudget().
   */
  protected function generateMachineName($class_name = NULL) {
    $class_name = 'datatotalbudget';
    return parent::generateMachineName($class_name);
  }

  public function prepare($entity, $source_row) {
    parent::prepare($entity, $source_row);

    $organisationmaptable = str_replace('datatotalbudget', 'dataorganisation', $this->getMap()->getMapTable());
    $organisations = db_select($organisationmaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $source_row->parent_entity)
      ->execute()
      ->fetchAssoc();
    if ($organisations === FALSE) {
      drupal_set_message('You first have to load the organisations.');
      return FALSE;
    }
    $entity->field_iati_org_ref['LANGUAGE_NONE'][0]['target_id'] = $organisations['destid1'];
  }
}

class DataRecipientBudget extends DataBudget {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    // Do some general administration.
    $this->description = t('Imports recipient organisation budget data for organisations.');

    // Instantiate the field map.
    $fields = $this->fields;
    $fields['recipient_org_ref'] = 'recipient_org_ref';
    $fields['recipient_org_name'] = 'recipient_org_name';
    
    $item_xpath = '/iati-organisations/iati-organisation/recipient-org-budget';  // relative to document
    $item_ID_xpath = 'parent::*/iati-identifier | recipient-org/@ref | period-start/@iso-date';  // relative to item_xpath
    if(isset($arguments['source_file']) && !empty($arguments['source_file'])){
      $items_class = new OAMMigrateItemsXML($arguments['source_file'], $item_xpath, $item_ID_xpath);
    }
    else {
      $items_class = new OAMMigrateItemsXML(" ", $item_xpath, $item_ID_xpath);
    }
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
    
    // Instantiate the field mapping.
    $this->addFieldMapping(NULL, 'recipient_org_ref')->xpath('recipient-org/@ref');
    $this->addFieldMapping(NULL, 'recipient_org_name')->xpath('recipient-org');
  }

  /**
   * Overrides generateMachineName::DataBudget().
   */
  protected function generateMachineName($class_name = NULL) {
    $class_name = 'datarecipientbudget';
    return parent::generateMachineName($class_name);
  }

  public function prepare($entity, $source_row) {
    parent::prepare($entity, $source_row);

    $organisationmaptable = str_replace('datarecipientbudget', 'dataorganisation', $this->getMap()->getMapTable());
    $organisations = db_select($organisationmaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $source_row->parent_entity)
      ->execute()
      ->fetchAssoc();
    if ($organisations === FALSE) {
      drupal_set_message('You first have to load the organisations.');
      return FALSE;
    }
    $entity->field_iati_org_ref['LANGUAGE_NONE'][0]['target_id'] = $organisations['destid1'];

    $recipientorgmaptable = str_replace('datarecipientbudget', 'dataorganisation', $this->getMap()->getMapTable());
    $recipientorgs = db_select($recipientorgmaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $source_row->recipient_org_ref)
      ->execute()
      ->fetchAssoc();
    if ($recipientorgs === FALSE) {
      drupal_set_message('You first have to load the organisations.');
      return FALSE;
    }
    $entity->field_recipient_org['LANGUAGE_NONE'][0]['target_id'] = $recipientorgs['destid1'];
  }
}

class DataCountryBudget extends DataBudget {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    // Do some general administration.
    $this->description = t('Imports recipient country budget data for organisations.');

    // Instantiate the field map.
    $fields = $this->fields;
    $fields['recipient_country_code'] = 'recipient_country_code';
    $fields['recipient_country_name'] = 'recipient_country_name';

    $item_xpath = '/iati-organisations/iati-organisation/recipient-country-budget';  // relative to document
    $item_ID_xpath = 'parent::*/iati-identifier | recipient-country/@code | period-start/@iso-date';  // relative to item_xpath
    if(isset($arguments['source_file']) && !empty($arguments['source_file'])){
      $items_class = new OAMMigrateItemsXML($arguments['source_file'], $item_xpath, $item_ID_xpath);
    }
    else{
      $items_class = new OAMMigrateItemsXML(" ", $item_xpath, $item_ID_xpath);
    }
    $this->source = new MigrateSourceMultiItems($items_class, $fields);
    
    // Instantiate the field mapping.
    $this->addFieldMapping(NULL, 'recipient_country_code')->xpath('recipient-country/@code');
    $this->addFieldMapping(NULL, 'recipient_country_name')->xpath('recipient-country');
  }

  /**
   * Overrides generateMachineName::DataBudget().
   */
  protected function generateMachineName($class_name = NULL) {
    $class_name = 'datacountrybudget';
    return parent::generateMachineName($class_name);
  }

  public function prepare($entity, $source_row) {
    parent::prepare($entity, $source_row);

    $organisationmaptable = str_replace('datacountrybudget', 'dataorganisation', $this->getMap()->getMapTable());
    $organisations = db_select($organisationmaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $source_row->parent_entity)
      ->execute()
      ->fetchAssoc();
    if ($organisations === FALSE) {
      drupal_set_message('You first have to load the organisations.');
      return FALSE;
    }
    $entity->field_iati_org_ref['LANGUAGE_NONE'][0]['target_id'] = $organisations['destid1'];

    $countrymaptable = str_replace('datacountrybudget' . str_replace('-', '', drupal_strtolower(pathinfo($this->arguments['source_file'], PATHINFO_FILENAME))), 'codelistlocationlevel1', $this->getMap()->getMapTable());
    $countries = db_select($countrymaptable, 'mt')
      ->fields('mt', array('sourceid1', 'destid1'))
      ->condition('sourceid1', $source_row->recipient_country_code)
      ->execute()
      ->fetchAssoc();
    if ($countries === FALSE) {
      drupal_set_message('You first have to load the admin boundaries.');
      return FALSE;
    }
    $entity->field_recipient_country['LANGUAGE_NONE'][0]['tid'] = $countries['destid1'];
  }
}
