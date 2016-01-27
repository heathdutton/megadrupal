<?php

/**
 * @file
 *   Commerce Product migration.
 *   This is a dynamic migration, reused for every product type
 *   (so that products of each type can be imported separately)
 */

class CommerceMigrateUbercartProductMigration extends Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
          'nid' => array(
            'type' => 'int',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'description' => 'Ubercart node ID',
            'alias' => 'ucp',
          ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_product', $arguments['type'])
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_ubercart_get_source_connection();
    $query = $connection->select('node', 'n');
    $query->innerJoin('uc_products', 'ucp', 'n.nid = ucp.nid AND n.vid = ucp.vid');
    $query->fields('n', array('nid', 'vid', 'type', 'title', 'created', 'changed', 'status'))
          ->fields('ucp', array('model', 'sell_price'))
          ->condition('n.type', $arguments['type'])
          ->distinct();
    if (variable_get('commerce_migrate_ubercart_user_map_ok', FALSE)) {
      $query->addField('n', 'uid', 'uid');
    }

    // Only deals with one currency, but at least trys to detect what one.
    $currency_code_setting = $connection->select('variable', 'v')
       ->fields('v', array('value'))
       ->condition('v.name', 'uc_currency_code')
       ->execute()->fetchField();

    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationEntityAPI('commerce_product', $arguments['type']);

    // Properties
    $this->addFieldMapping('sku', 'model')
         ->dedupe('commerce_product', 'sku');
    $this->addFieldMapping('type', 'type')
         ->defaultValue($arguments['type']);
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'changed');
    // Default uid to 0 if we're not mapping it.
    if (variable_get('commerce_migrate_ubercart_user_map_ok', FALSE) && variable_get('commerce_migrate_ubercart_user_migration_class', "") != "") {
      $this->addFieldMapping('uid', 'uid')->sourceMigration(variable_get('commerce_migrate_ubercart_user_migration_class', ""))->defaultValue(0);
    }
    else {
      $this->addFieldMapping('uid', 'uid')->defaultValue(0);
    }

    // Probably unnecessary but we should respect the status of the old products.
    $this->addFieldMapping('status', 'status')
         ->defaultValue(1);

    $this->addFieldMapping('language', NULL)->defaultValue(LANGUAGE_NONE);

    $this->addFieldMapping(NULL, 'vid')->issueGroup(t('DNM'));
    $this->addFieldMapping('path')->issueGroup(t('DNM'));

    $this->addFieldMapping('commerce_price', 'sell_price');

    if ($currency_code_setting == NULL) {
      $this->addFieldMapping('commerce_price:currency_code', NULL)->defaultValue('USD');
    }
    else {
      $this->addFieldMapping('commerce_price:currency_code', NULL)->defaultValue(unserialize($currency_code_setting));
    }
  }
}
