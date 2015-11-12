<?php
/**
 * @file
 * iati_feature.relation_type_default.inc
 */

/**
 * Implements hook_relation_default_relation_types().
 */
function iati_feature_relation_default_relation_types() {
  $export = array();

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'iati_organisation_role';
  $relation_type->label = 'IATI Organisation Role';
  $relation_type->reverse_label = 'IATI Involved Organisations';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'node:iati_activity',
  );
  $relation_type->target_bundles = array(
    0 => 'node:iati_organisation',
  );
  $export['iati_organisation_role'] = $relation_type;

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'iati_parent_activity';
  $relation_type->label = 'IATI Parent Activity';
  $relation_type->reverse_label = 'IATI Parent Activity';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 1;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'node:iati_activity',
  );
  $relation_type->target_bundles = array(
    0 => 'node:iati_activity',
  );
  $export['iati_parent_activity'] = $relation_type;

  return $export;
}
