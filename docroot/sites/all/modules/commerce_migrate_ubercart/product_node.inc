<?php

/**
 * @file
 *   Commerce Product Display Node migration.
 *   This is a dynamic migration, reused for every product type
 *   (so that products of each type can be imported separately)
 */

class CommerceMigrateUbercartNodeMigration extends Migration {
  // An array mapping D6 format names to this D7 databases formats.
  public $filter_format_mapping = array();

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
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $connection = commerce_migrate_ubercart_get_source_connection();

    $query = $connection->select('node', 'n');

    switch (SOURCE_DATABASE_DRUPAL_VERSION) {
      case 'd7':
        $query->leftJoin('field_data_body', 'fdb', 'n.nid = entity_id AND n.vid = revision_id');
        $query->fields('n', array('nid', 'type', 'title', 'created', 'changed'))
              ->fields('fdb', array('body_value', 'body_summary', 'body_format', 'language'))
              ->condition('n.type', $arguments['type'])
              ->distinct();
        break;
      case 'd6':
        $query->leftJoin('node_revisions', 'nr', 'n.nid = nr.nid AND n.vid = nr.vid');
        $query->leftJoin('filter_formats', 'ff', 'nr.format = ff.format');
        $query->fields('n', array('nid', 'type', 'title', 'created', 'changed'));
        $query->addField('nr', 'body', 'body_value');
        $query->addField('nr', 'teaser', 'body_summary');
        $query->addField('ff', 'name', 'body_format');
        $query->condition('n.type', $arguments['type'])
          ->distinct();
        break;
    }
    if (variable_get('commerce_migrate_ubercart_user_map_ok', FALSE)) {
      $query->addField('n', 'uid', 'uid');
    }
    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

    // The product_display content type is created by commerce install profiles,
    // not the module itself, so hardcoding this might be unwise.
    $this->destination = new MigrateDestinationNode('product_display');

    // Properties
    // Default uid to 1 if we're not mapping it.
    if (variable_get('commerce_migrate_ubercart_user_map_ok', FALSE)) {
      $this->addFieldMapping('uid', 'uid')->sourceMigration(variable_get('commerce_migrate_ubercart_user_migration_class', ""))->defaultValue(1);
    }
    else {
      $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    }
        $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'changed');

    // This gets mapped later on in prepareRow as $row->path.
    $this->addFieldMapping('path', 'path')->defaultValue(NULL);

    // Fields
    // We have no idea of which type the referenced product is, so
    // we specify all (for the sourceMigration).
    $this->addFieldMapping('field_product', 'nid')
         ->sourceMigration($arguments['products']);

    $this->addFieldMapping('body', 'body_value')
         ->defaultValue('');
    $this->addFieldMapping('body:summary');
    $this->addFieldMapping('body:format', 'body_format');
    $this->addFieldMapping('body:language')->defaultValue('UND');
  }

  public function prepareRow($row) {
    // Transform body format to something we can use if it's not already.
    if (!filter_format_exists($row->body_format)) {
      $row->body_format = $this->transformFormatToMachineName($row->body_format);
    }

    // Get the original path, if any and make it available for mapping.
    switch (SOURCE_DATABASE_DRUPAL_VERSION) {
      case 'd7':
        $query_string = 'SELECT alias FROM {url_alias} WHERE source = :source';
        break;
      case 'd6':
        $query_string = 'SELECT dst AS alias FROM {url_alias} WHERE src = :source';
        break;
    }
    $connection = commerce_migrate_ubercart_get_source_connection();
    $url_alias = $connection->query($query_string, array(':source' => "node/{$row->nid}"))->fetchObject();
    if (!empty($url_alias->alias)) {
      $row->path = $url_alias->alias;
    }
  }

  // Map friendly name of format (key) to machine name (format).
  public $format_mapping = array();

  /**
   * Transform a D6-type format into a D7 format name
   *
   * @param $format
   *   Friendly name of the format, like 'Filtered HTML'.
   */
  public function transformFormatToMachineName($format) {
    // If we haven't initialized the $format_mapping, do it.
    if (empty($this->format_mapping)) {
      $result = db_query("SELECT format, name FROM {filter_format}");
      foreach ($result as $record) {
        $this->format_mapping[$record->name] = $record->format;
      }
    }
    if (!empty($this->format_mapping[$format])) {
      return $this->format_mapping[$format];
    }
    return variable_get('commerce_migrate_ubercart_default_filter_format', 'plain_text');
  }
}
