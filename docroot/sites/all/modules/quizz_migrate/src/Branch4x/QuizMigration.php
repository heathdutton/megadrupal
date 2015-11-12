<?php

namespace Drupal\quizz_migrate\Branch4x;

use Drupal\quizz_migrate\Destination\QuizDestination;
use MigrateSourceSQL;
use MigrateSQLMap;
use Migration;
use SelectQuery;

class QuizMigration extends Migration {

  protected $description = 'Migrate quiz node to entity.';
  protected $entityType = 'quiz_entity';
  protected $bundle = 'quiz';
  protected $bodyFieldName = 'quiz_body';
  protected $sourcePK = array(
      'nid' => array(
          'type'        => 'int',
          'not null'    => TRUE,
          'description' => 'Node',
          'alias'       => 'n',
      )
  );

  public function __construct($arguments = array()) {
    parent::__construct($arguments);
    $this->source = $this->setupMigrateSource();
    $this->destination = $this->setupMigrateDestination();
    $this->map = new MigrateSQLMap($this->machineName, $this->sourcePK, QuizDestination::getKeySchema());
    $this->setupFieldMapping();
  }

  protected function setupMigrateDestination() {
    return new QuizDestination();
  }

  protected function setupMigrateSource() {
    $query = db_select('node', 'n');
    $query->innerJoin('quiz_node_properties', 'properties', 'n.vid = properties.vid');
    $query
      ->fields('n', array('nid', 'vid', 'uid', 'language', 'title', 'created', 'status', 'changed'))
      ->orderBy('n.nid', 'ASC')
    ;
    $this->setUpMigateSourceFields($query);
    return new MigrateSourceSQL($query);
  }

  /**
   * @param SelectQuery $query
   */
  protected function setUpMigateSourceFields($query) {
    foreach (field_info_instances('node', $this->bundle) as $field_name => $field_instance) {
      $columns = array();
      $field = field_info_field($field_name);

      if (!empty($field['columns'])) {
        foreach (array_keys($field['columns']) as $column) {
          $columns[] = _field_sql_storage_columnname($field_name, $column);
        }
      }

      $table = _field_sql_storage_tablename($field_instance);
      $query->leftJoin($table, $table, "n.vid = {$table}.revision_id AND {$table}.delta = 0 AND {$table}.entity_type = 'node'");
      $query->fields($table, $columns);
    }
  }

  protected function setupFieldMapping() {
    $this->addFieldMapping('type')->defaultValue($this->bundle);
    $this->addSimpleMappings(array('uid', 'title', 'created', 'changed', 'status', 'language'));
    $this->addUnmigratedSources(array('nid', 'vid'));
    $this->addUnmigratedDestinations(array('qid', 'vid', 'path'));
    if ($this->bundle) {
      $this->setupFieldMappingEntityFields();
    }
  }

  protected function setupFieldMappingEntityFields() {
    foreach (array_keys(field_info_instances('node', $this->bundle)) as $source_field_name) {
      $dest_field_name = 'body' === $source_field_name ? $this->bodyFieldName : $source_field_name;

      if (field_info_instance($this->entityType, $dest_field_name, $this->bundle)) {
        $this->addUnmigratedDestinations(array($dest_field_name));
        $field = field_info_field($dest_field_name);
        $this->addFieldMapping("{$dest_field_name}:language", 'language');
        foreach (array_keys($field['columns']) as $column) {
          $src = _field_sql_storage_columnname($source_field_name, $column);
          $dest = 'value' === $column ? $dest_field_name : "{$dest_field_name}:{$column}";
          $this->addFieldMapping($dest, $src);
        }
      }
      else {
        $warnings[] = $dest_field_name;
      }
    }

    if (!empty($warnings) && 'admin' === arg(0)) {
      $msg = "Fields not found on {$this->entityType}.{$this->bundle}: " . implode(', ', $warnings);
      drupal_set_message($msg, 'error');
    }
  }

}
