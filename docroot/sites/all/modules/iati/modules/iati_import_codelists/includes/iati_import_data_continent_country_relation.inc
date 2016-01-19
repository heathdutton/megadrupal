<?php
/**
 * @file.
 * CodeLists for Location for continent - country relation
 */
class ContCountRelation extends XMLMigration {
	/**
	 * Invoke the class constructor.
	 */
	public function __construct() {
		parent::__construct ();
		
		// Do some general administration.
		$this->description = t ( 'Imports data for the continent-country relations.' );
		$this->softDependencies = array('CountryOutlines');
		// Instantiate the map.
		$fields = array (
				'continent' => 'continent',
				'country' => 'country',
				'tid' => 'tid',
		);
		$items_url = str_replace('includes', 'data', dirname(__FILE__)) . '/countrymapping.xml';
		$item_xpath = '/resultset/row'; // relative to document
		$item_ID_xpath = 'field[@name="b"]'; // relative to item_xpath
		$this->source = new MigrateSourceXML ( $items_url, $item_xpath, $item_ID_xpath, $fields );
		$this->destination = new MigrateDestinationTerm ( 'iati_admin_boundaries' );
		// Instantiate the map.
		$this->map = new MigrateSQLMap ( $this->machineName, array (
				'country' => array (
						'type' => 'varchar',
						'length' => 255,
						'not null' => TRUE,
						'description' => 'country',
						'alias' => 'c' 
				) 
		), MigrateDestinationTerm::getKeySchema () );
		// Instantiate the field mapping.
		$this->addFieldMapping ( 'tid','tid')->xpath( 'field[@name="b"]' )->sourceMigration('LocationLevel1');
		//$this->addFieldMapping ( 'field_iati_code', 'country' )->xpath ( 'field[@name="b"]' );
		$this->addFieldMapping ( 'parent', 'parent' )->xpath( 'field[@name="a"]' )->sourceMigration('LocationLevel0');
		$this->addFieldMapping ('name');
		
		//$this->addUnmigratedDestinations(array('name','description'));
		
		
		/*
		 * $this->addFieldMapping('description', 'description')->xpath('description'); $this->addFieldMapping('parent', 'parent');
		 */
	}
	
     public function prepareRow($row) {
     	//dpm($row);
     }
     
     public function prepare($term, $row) {
     	$term->name = taxonomy_term_load($term->tid)->name;
     }
	
}
