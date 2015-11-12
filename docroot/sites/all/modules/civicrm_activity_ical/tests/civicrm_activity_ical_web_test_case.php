<?php
/**
 * @file
 * Base class for simpletest tests for civicrm_activity_ical module
 */

class CiviCRMActivityICalTestWebTestCase extends DrupalWebTestCase {

  protected $civicrm_prefix;
  protected $tables;

  function __construct($test_id = NULL) {
    $this->profile = 'minimal';
    parent::__construct($test_id);
  }

  /**
   * Performs special setup requirements for CiviCRM
   */
  function setUpCiviCRM() {
    // If we don't clear CRM_Core_Menu::$_items, CiviCRM will preserve it across tests;
    // This wouldn't be a problem, except that CiviCRM adds the 'admin' item to the menu
    // array, and the 'admin' item isn't able to pass CiviCRM's own checking for
    // proper format of a menu item. Normally, this is ok for civicrm, but we're running
    // civicrm_enable() several times within one PHP run, so we have to clear it each time.

    // Initialize CiviCRM, including its dynamic include path.
    civicrm_initialize();
    // Require a file from CiviCRM's dynamic include path.
    require_once 'CRM/Core/Menu.php';
    CRM_Core_Menu::$_items = NULL;
  }

  /**
   * Backs up CiviCRM data to be restored after the test.
   */
  public function setUpData() {
    // default variables may be affected by whatever happens in setUp(), so set them again.
    civicrm_activity_ical_set_default_variables();

    // Backup civicrm database tables
    global $db_prefix;

    $this->civicrm_prefix = $db_prefix . '_bkp_';

    // Initialize CiviCRM, including its dynamic include path.
    civicrm_initialize();
    // Require a file from CiviCRM's dynamic include path.
    require_once 'CRM/Core/DAO.php';

    $dao = CRM_Core_DAO::executeQuery("SHOW TABLES");
    $this->tables = array();
    while ($dao->fetch()) {
      // Build list
      $this->tables[] = current($dao->toArray());
    }

    // Clone CiviCRM tables with prefix
    foreach ($this->tables as $table) {
      // Note, bracketed table names aren't a part of CiviCRM's DAO syntax.
      $new_table_name = $this->civicrm_prefix . $table;
      $query = "CREATE TABLE $new_table_name LIKE $table";
      $dao = CRM_Core_DAO::executeQuery($query);
      $query = "INSERT INTO {$new_table_name} SELECT * FROM {$table}";
      $dao = CRM_Core_DAO::executeQuery($query);
    }

    // remove all uf_match connections (new drupal users shouldn't be linked to CiviCRM contacts)
    // Note, bracketed table names aren't a part of CiviCRM's DAO syntax.
    $query = "DELETE FROM civicrm_uf_match WHERE uf_id <> 1";
    $dao = CRM_Core_DAO::executeQuery($query);
  }

  /**
   * Restores CiviCRM data to its pre-test state.
   */
  function tearDownData() {
    if (empty($this->tables)) {
      // Only revert tables if we have some that were created in setUp().
      return;
    }

    $dao = CRM_Core_DAO::executeQuery("SHOW TABLES");
    while ($dao->fetch()) {
      $existing_tables[] = current($dao->toArray());
    }

    // To preserve CiviCRM's foreign keys, we don't drop and recreate tables;
    // instead, we truncate the table, then copy all backed up data into it.
    // Since this can create foreign key errors, we turn off FK checking
    // temporarily.
    // Note, bracketed table names aren't a part of CiviCRM's DAO syntax.
    $query = "SET foreign_key_checks = 0";
    $dao = CRM_Core_DAO::executeQuery($query);
    foreach ($this->tables as $table) {
      if (in_array($this->civicrm_prefix . $table, $existing_tables)) {
        $query = "TRUNCATE TABLE " . $table;
        $dao = CRM_Core_DAO::executeQuery($query);
        $query = "INSERT INTO {$table} SELECT * FROM {$this->civicrm_prefix}{$table}";
        $dao = CRM_Core_DAO::executeQuery($query);
        $query = "DROP TABLE IF EXISTS {$this->civicrm_prefix}{$table}";
        $dao = CRM_Core_DAO::executeQuery($query);
      }
    }
    $query = "SET foreign_key_checks = 1";
    $dao = CRM_Core_DAO::executeQuery($query);

    // In case any unrecognized tables exist (e.g., CiviCRM custom data tables
    // created by testing scenarios), drop them too.
    $remaining_tables = array();
    foreach ($existing_tables as $table) {
      if (!in_array($table, $this->tables)) {
        $remaining_tables[] = $table;
      }
    }
    foreach ($remaining_tables as $table) {
      $query = "DROP TABLE IF EXISTS " . $table;
      $dao = CRM_Core_DAO::executeQuery($query);
    }
  }

  /**
   * Gets the value (if any) of the first text element identified by $id
   * @return The value, or FALSE if no matching element was found.
   */
  function getTextElementValue($id) {
    $elements = $this->xpath('//*[@id="'. $id .'"]');

    if ($elements[0]) {
      $attr = $elements[0]->attributes();
      $value = $attr['value'];
      return $value;
    }
    else {
      return FALSE;
    }
  }
}