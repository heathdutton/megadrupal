<?php

/**
 * @file
 * Drush integration for bundle_copy
 */

function fieldhelp_drush_command() {
  $items = array();

  $items['bundle-copy-export'] = array(
    'aliases' => array('fhex'),
    'callback' => 'fieldhelp_drush_export',
    'description' => 'Export bundle configuration.',
    'arguments' => array(
      'entity_type' => 'Entity type of the bundle to exort',
      'bundle' => 'Bundle to export',
    ),
    'options' => array(
      'filter' => ''
    ),
    'required-arguments' => TRUE,
  );

  return $items;
}

function fieldhelp_drush_export($entity_type, $bundle) {
  $data = array();
  $field_instances = field_info_instances($entity_type, $bundle);
  $exclude_type_keys = array(
    'disabled',
    'disabled_changed',
    'module',
    'orig_type',
  );
  $exclude_field_keys = array(
    'active',
    'columns',
    'deleted',
    'foreign keys',
    'id',
    'indexes',
    'locked',
    'storage',
  );
  $exclude_field_instance_keys = array(
    'deleted',
    'field_id',
    'id',
  );

  $data['type'] = array_diff_key((array)node_type_load($bundle), array_flip($exclude_type_keys));
  $field_info_bundles = field_info_bundles($entity_type);
  $data['rdf_mapping'] = $field_info_bundles[$bundle]['rdf_mapping'];

  foreach ($field_instances as $field_name => $field_instance_info) {
    if (!isset($data['fields'][$field_name])) {
      $data['fields'][$field_name] = array_diff_key(field_info_field($field_name), array_flip($exclude_field_keys));
    }

    if (!isset($data['instances'][$field_name])) {
      $data['instances'][$field_name] = array_diff_key($field_instance_info, array_flip($exclude_field_instance_keys));
    }
  }

  ksort($data['instances']);

  // Field group export data.
  if (!empty($selected_fieldgroups)) {
    foreach ($selected_fieldgroups as $key => $value) {
      if ($value !== 0) {
        $data['fieldgroups'][$full_fieldgroups[$key]->identifier] = $full_fieldgroups[$key];
      }
    }
  }

  module_load_include('inc', 'ctools', 'includes/export');

  $output = "\n/**\n";
  $output .= " * Creates bundle $bundle.\n";
  $output .= " */\n";
  $output .= "function create_$bundle() {\n";
  $output .= "  \$data = " . str_replace("\n", "\n  ", ctools_var_export($data)) . ";\n";
  $output .= "  node_type_save(node_type_set_defaults(\$data['type']));\n";
  $output .= "  foreach (\$data['fields'] as \$name => \$field) {\n";
  $output .= "    if (!field_info_field(\$name) {\n";
  $output .= "      field_create_field(\$field);\n";
  $output .= "    }\n";
  $output .= "    field_create_instance(\$data['instances'][\$name]);\n";
  $output .= "  }\n";
  $output .= "  \$rdf_mapping = array(\n";
  $output .= "    'type' => $entity_type,\n";
  $output .= "    'bundle' => $bundle,\n";
  $output .= "    'mapping' => \$data['rdf_mapping'],\n";
  $output .= "  );\n";
  $output .= "  rdf_mapping_save(\$rdf_mapping);\n";
  $output .= "}\n";

  return $output;
}
