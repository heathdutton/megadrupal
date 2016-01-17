<?php

function shadow_schema_build_table($table, $columns, $filters, $indexes) {
  $definition = array(
    'fields' => array(),
    'primary key' => array(),
    'indexes' => array(),
    'module' => 'shadow',
    'name' => $table->db_name,
  );

  if (!$base_table = drupal_get_schema($table->base_table)) {
    return FALSE;
  }

  _shadow_load_classes();
  
  foreach ($columns as $column) {
    $column_parser = new ShadowColumn($column->definition);
    
    $field_schema = $column_parser->getSchema();
    $definition['fields'][$column->db_name] = $field_schema;
    
    if (($column_parser->getTableName() == $table->base_table) && in_array($column_parser->getFieldName(), $base_table['primary key'])) {
      $definition['primary key'][] = $column->db_name;
    }
    elseif ($column_parser->isMultiple()) {
      $definition['primary key'][] = $column->db_name;
    }
  }
  
  $c = 0;
  foreach ($indexes as $index) {
    ++$c;
    $index = new ShadowIndex($index->definition, $definition);
    $definition['indexes']["index$c"] = $index->getDefinition();
  }
  
  return $definition;
}
