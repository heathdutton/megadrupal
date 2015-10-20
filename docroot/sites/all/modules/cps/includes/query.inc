<?php

/**
 * @file
 * Views and Entity query alters.
 */

/**
 * Implements hook_query_alter().
 */
function cps_query_alter(QueryAlterableInterface $query) {
  // No need to alter if there's no active changeset.
  $changeset_id = cps_get_current_changeset();
  if ($changeset_id == CPS_PUBLISHED_CHANGESET) {
    return;
  }

  // The tag is per-entity-type, so need to consult entity info.
  $types = cps_get_supported();
  foreach ($types as $entity_type) {
    if ($query->hasTag($entity_type . '_load_multiple')) {
      // Don't load the changeset before here, since that could lead to infinite
      // recursion - changesets are entities but will never be CPS enabled.
      // However now we're here, protect against invalid changeset IDs.
      $changeset_id = cps_get_current_changeset(TRUE);
      if ($changeset_id == CPS_PUBLISHED_CHANGESET) {
        return;
      }
      $changeset = cps_changeset_load($changeset_id);
      $info = entity_get_info($entity_type);
      $entity_id_key = $info['entity keys']['id'];
      $revision_id_key = $info['entity keys']['revision'];
      $entity_revision_table = $info['revision table'];
      $conditions =& $query->conditions();

      // Every condition that's on the base table, needs to be on the revision
      // table.
      $add_join = FALSE;
      $table_alias = 'cps_revision';
      foreach ($conditions as &$condition) {
        if (is_array($condition)) {
          // If there's a condition on the revision key, don't need to alter at
          // all.
          if ($condition['field'] == 'revision.' . $revision_id_key) {
            return;
          }
          // Only alter conditions on the base table, and only if they aren't on
          // the entity ID key.
          if (strpos($condition['field'], 'base.') === 0 && $condition['field'] != 'base.' . $entity_id_key) {
            $add_join = TRUE;
            $condition['field'] = str_replace('base.', $table_alias . '.', $condition['field']);
          }
        }
      }
      if ($add_join) {
        // If the changeset has been published, we INNER join to exclude content
        // that did not exist when the changeset was published. If it has not, we
        // LEFT join so we can properly include live content along with the
        // changeset.
        $join_type = $changeset->published ? 'INNER' : 'LEFT';
        $query->addJoin($join_type, 'cps_entity', 'cps_entity', "cps_entity.entity_type = '$entity_type' AND cps_entity.entity_id = base.$entity_id_key AND cps_entity.changeset_id = '$changeset_id'");
        $query->join($entity_revision_table, $table_alias, "$table_alias.$revision_id_key = COALESCE(cps_entity.revision_id, base.$revision_id_key)");
      }
    }
  }
}

/**
 * Implements hook_entity_query_alter().
 *
 * @param $query
 */
function cps_entity_query_alter($query) {
  // Do not muck with EFQ queries if there is not a changeset active.
  $changeset_id = cps_get_current_changeset(TRUE);
  if ($changeset_id == CPS_PUBLISHED_CHANGESET) {
    return;
  }

  // Ignore specific queries for historical data.
  if ($query->age != FIELD_LOAD_CURRENT) {
    return;
  }

  // Broken with changesets, ignore.
  if (empty($query->entityConditions['entity_type'])) {
    return;
  }

  // Only bother if it's a supported, revisioned entity type.
  $entity_type = $query->entityConditions['entity_type']['value'];
  if (!cps_is_supported($entity_type)) {
    return;
  }

  $query->executeCallback = 'cps_field_sql_storage_field_storage_query';
}

/**
 * Replacement for field_sql_storage_field_storage_query().
 *
 * This function replaces the original so that we can take into account changesets.
 * It is a copy that has been heavily modified.
 *
 * It does the following things differently:
 * * Select on entity_base_table as top table instead of field_base_table.
 *
 * * Join the CPS table to either include live content or restrict to the content
 *   at the time of the publishing.
 *
 * * Use field revision tables based on the CPS revision to fetch the data from.
 *
 * * Check if properties exist on the entity revision table, if so use those, else use
 *   the ones on the entity base table.
 */
function cps_field_sql_storage_field_storage_query(EntityFieldQuery $query) {
  $changeset_id = cps_get_current_changeset();
  $changeset = cps_changeset_load($changeset_id);

  $entity_type = $query->entityConditions['entity_type']['value'];
  $entity_info = entity_get_info($entity_type);
  $entity_id_key = $entity_info['entity keys']['id'];
  $revision_id_key = $entity_info['entity keys']['revision'];
  // The $entity_info['entity keys']['bundle'] property provides the name of
  // the key on the entity object that stores the bundle name. Normally, this
  // will be the same as the database column that stores the bundle (which is
  // what this function actually needs), but it doesn't have to be; for
  // example, the two are different for taxonomy terms. For those cases, allow
  // the module to override the default by providing a dedicated key for this
  // purpose.
  $bundle_key = isset($entity_info['entity keys']['bundle_database_column']) ? $entity_info['entity keys']['bundle_database_column'] : $entity_info['entity keys']['bundle'];
  $entity_base_table = $entity_info['base table'];
  $entity_revision_table = $entity_info['revision table'];

  // Unlike the original, we always use the entity table as the base table.
  // We can do this because we have verified that there is an entity type
  // involved in the query prior to this function being run.
  $select_query = db_select($entity_base_table, $entity_base_table);

  // Allow queries internal to the Field API to opt out of the access
  // check, for situations where the query's results should not depend on
  // the access grants for the current user.
  if (!isset($query->tags['DANGEROUS_ACCESS_CHECK_OPT_OUT'])) {
    $select_query->addTag('entity_field_access');
  }

  // If the changeset has been published, we INNER join to exclude content that did not exist when
  // the changeset was published. If it has not, we LEFT join so we can properly include live
  // content along with the changeset.
  $join_type = $changeset->published ? 'INNER' : 'LEFT';
  $select_query->addJoin($join_type, 'cps_entity', 'cps_entity', "cps_entity.entity_type = '$entity_type' AND cps_entity.entity_id = $entity_base_table.$entity_id_key AND cps_entity.changeset_id = '$changeset_id'");

  // Add a join on the revision table, so that the stub revision ID returned in
  // results is the one in the changeset, not whatever is in the base table.
  $entity_revision_table = $entity_info['revision table'];
  $select_query->join($entity_revision_table, $entity_revision_table, "$entity_revision_table.$revision_id_key = COALESCE(cps_entity.revision_id, $entity_base_table.$revision_id_key)");

  // This adds all of the base fields that are expected so that it can create
  // a pseudo entity when reading the results.
  $select_query->addMetaData('base_table', $entity_base_table);
  $select_query->addMetaData('revision_table', $entity_revision_table);
  $select_query->addExpression("'$entity_type'", 'entity_type');
  $select_query->addField($entity_base_table, $entity_id_key, 'entity_id');
  $select_query->addField($entity_revision_table, $revision_id_key, 'revision_id');
  // The bundle key is optional in the case where the entity type only has one
  // bundle.
  if ($bundle_key) {
    $select_query->addField($entity_base_table, $bundle_key, 'bundle');
  }


  $table_aliases = array();
  // Add tables for the fields used.
  foreach ($query->fields as $key => $field) {
    // The original used $tablename_function here, which was checked based on $query->age
    // Because we're going through cps_entity and looking at revisions, we always join
    // the field revision table instead of the current table.
    $tablename = _field_sql_storage_revision_tablename($field);
    // Every field needs a new table.
    $table_alias = $tablename . $key;

    // Set the first field as the field_base_table which is still used for some bits.
    if ($key == 0) {
      $field_base_table = $table_alias;
    }
    $table_aliases[$key] = $table_alias;
    // This coalesce allows us to join the specific revision for the changeset, or the
    // current revision if no changeset exists.
    $select_query->join($tablename, $table_alias, "$table_alias.entity_type = '$entity_type' AND $table_alias.revision_id = COALESCE(cps_entity.revision_id, $entity_base_table.$revision_id_key)");
    if ($field['cardinality'] != 1 || $field['translatable']) {
      $select_query->distinct();
    }
  }

  // Add field conditions. We need a fresh grouping cache.
  drupal_static_reset('_field_sql_storage_query_field_conditions');
  _field_sql_storage_query_field_conditions($query, $select_query, $query->fieldConditions, $table_aliases, '_field_sql_storage_columnname');

  // Add field meta conditions.
  _field_sql_storage_query_field_conditions($query, $select_query, $query->fieldMetaConditions, $table_aliases, '_field_sql_storage_query_columnname');

  // This still uses the $field_base_table because entity_base_table does not have this field.
  if (isset($query->deleted) && isset($field_base_table)) {
    $select_query->condition("$field_base_table.deleted", (int) $query->deleted);
  }

  // Is there a need to sort the query by property?
  $has_property_order = FALSE;
  foreach ($query->order as $order) {
    if ($order['type'] == 'property') {
      $has_property_order = TRUE;
    }
  }

  if ($query->propertyConditions || $has_property_order) {
    $query->entityConditions['entity_type']['operator'] = '=';

    // Check to see if the property exists on the revision table. If it does, use that instead
    // so that we can properly see the revision. If not, default to the entity base table.
    foreach ($query->propertyConditions as $property_condition) {
      if (in_array($property_condition['column'], $entity_info['schema_fields_sql']['revision table'])) {
        $query->addCondition($select_query, "$entity_revision_table." . $property_condition['column'], $property_condition);
      }
      else {
        $query->addCondition($select_query, "$entity_base_table." . $property_condition['column'], $property_condition);
      }
    }
  }

  foreach ($query->entityConditions as $key => $condition) {
    switch ($key) {
      case 'entity_id':
        $field = $entity_id_key;
        break;
      case 'revision_id':
        $field = $revision_id_key;
        break;
      case 'bundle':
        $field = $bundle_key;
        break;
      default:
        $field = NULL;
    }
    if ($field) {
      $query->addCondition($select_query, "$entity_base_table.$field", $condition);
    }
  }

  // Order the query.
  foreach ($query->order as $order) {
    if ($order['type'] == 'entity') {
      // Entity ordering is similar to entity conditions above.
      $key = $order['specifier'];
      switch ($key) {
        case 'entity_id':
          $field = $entity_id_key;
          break;
        case 'revision_id':
          $field = $revision_id_key;
          break;
        case 'bundle':
          $field = $bundle_key;
          break;
        default:
          $field = NULL;
      }
      $select_query->orderBy("$entity_base_table.$field", $order['direction']);
    }
    elseif ($order['type'] == 'field') {
      $specifier = $order['specifier'];
      $field = $specifier['field'];
      $table_alias = $table_aliases[$specifier['index']];
      $sql_field = "$table_alias." . _field_sql_storage_columnname($field['field_name'], $specifier['column']);
      $select_query->orderBy($sql_field, $order['direction']);
    }
    elseif ($order['type'] == 'property') {
      // Like property conditions above, check if it is in the revision table.
      if (in_array($order['specifier'], $entity_info['schema_fields_sql']['revision table'])) {
        $select_query->orderBy("$entity_revision_table." . $order['specifier'], $order['direction']);
      }
      else {
        $select_query->orderBy("$entity_base_table." . $order['specifier'], $order['direction']);
      }
    }
  }

  return $query->finishQuery($select_query, 'entity_id');
}

/**
 * Implements hook_views_query_alter().
 */
function cps_views_query_alter($view, $query) {
  $changeset = cps_get_current_changeset(TRUE);

  // Do not modify queries while viewing the published changeset.
  if ($changeset == CPS_PUBLISHED_CHANGESET) {
    return;
  }

  // Find out what entity types are represented in this query.
  $entity_types = array();
  foreach ($query->relationships as $info) {
    $table_data = views_fetch_data($info['base']);
    if (empty($table_data['table']['entity type'])) {
      continue;
    }
    $entity_type = $table_data['table']['entity type'];
    // This construct ensures each entity_type exists only once.
    $entity_types[$entity_type] = $entity_type;
  }

  foreach ($entity_types as $entity_type) {
    if (cps_is_supported($entity_type)) {
      cps_views_query_alter_entity($view, $query, $entity_type);
    }
  }
}

/**
 * Helper for cps_views_queryAlter().
 */
function cps_views_query_alter_entity($view, $query, $entity_type) {
  // This is only called if the system has already determined that this entity type
  // is involved in the query, and that a changeset is in use.

  $move_cps_entities = array();
  // Start $index at -1 so it will immediately be 0.
  $index = -1;
  $entity_info = entity_get_info($entity_type);
  foreach ($query->table_queue as $alias => &$table_info) {
    // Update $index early because we might continue later.
    $index++;

    // If we reach the cps_entity array item before any candidates, then we
    // do not need to move it.
    if ($table_info['table'] == 'cps_entity') {
      break;
    }

    // Any table named field_data_* is a candidate.
    if (strpos($table_info['table'], 'field_data_') === 0) {
      $relationship = $table_info['relationship'];
      // See if it is our entity type.
      // While it may seem inefficient to check entity type individually,
      // non-revisioned entities might have field_data_* fields and we do NOT want
      // to muck with those by accident!
      $table_data = views_fetch_data($query->relationships[$relationship]['base']);
      if (empty($table_data['table']['entity type']) || $table_data['table']['entity type'] != $entity_type) {
        continue;
      }

      // There can be reverse relationships used; if so, CPS can't do anything with them.
      // Detect this and skip:
      if ($table_info['join']->field != 'entity_id') {
        continue;
      }

      // Calculate the revision table name. field_data_ is 11 characters long.
      $new_table_name = 'field_revision_' . substr($table_info['table'], 11);

      // Now add the cps_entity table.
      $cps_entity_table = cps_ensure_cps_table($entity_type, $query, $relationship);

      // Update the join to use our coalesce
      $revision_field = $entity_info['entity keys']['revision'];
      $table_info['join']->left_table = NULL;
      $table_info['join']->left_field = "COALESCE($cps_entity_table.revision_id, $relationship.$revision_field)";

      // Update the join and the table info to our new table name, and to join on the revision key.
      $table_info['table'] = $new_table_name;
      $table_info['join']->table = $new_table_name;
      $table_info['join']->field = 'revision_id';

      // Finally, if we added the cps entity table we have to move it in the table_queue so that it
      // comes before this field.
      if (empty($move_cps_entities[$cps_entity_table])) {
        $move_cps_entities[$cps_entity_table] = $alias;
      }
    }
  }

  // JOINs must be in order. i.e, any tables you mention in the ON clause of a JOIN
  // must appear prior to that JOIN. Since we're modifying a JOIN in place,
  // and adding a new table, we must ensure that new table appears prior
  // to this one. So we recorded at what index we saw that table, and then
  // use array_splice() to move the cps_entity table join to the right position.
  foreach ($move_cps_entities as $cps_entity_table => $alias) {
    cps_move_entity_table($query, $cps_entity_table, $alias);
  }

  $entity_table = $entity_info['base table'];
  $test_fields = cps_get_revision_fields($entity_type);
  // Go through and look to see if we have to modify fields and filters
  // for $entity_table.status, $entity_table.sticky.
  // but we currently do not use those fields.
  foreach ($query->fields as &$field_info) {
    // Some fields don't actually have tables, meaning they're formulae and
    // whatnot. At this time we are going to ignore those.
    if (empty($field_info['table'])) {
      continue;
    }

    // Dereference the alias into the actual table.
    $table = $query->table_queue[$field_info['table']]['table'];
    if ($table == $entity_table && in_array($field_info['field'], $test_fields)) {
      $relationship = $query->table_queue[$field_info['table']]['alias'];
      $alias = cps_ensure_revision_table($entity_type, $query, $relationship);
      if ($alias) {
        // Change the base table to use the revision table instead.
        $field_info['table'] = $alias;
      }
    }
  }

  $relationships = array();
  // Build a list of all relationships that might be for our table.
  foreach ($query->relationships as $relationship => $info) {
    if ($info['base'] == $entity_table) {
      $relationships[] = $relationship;
    }
  }

  // Now we have to go through our where clauses and modify any of our fields.
  foreach ($query->where as &$clauses) {
    foreach ($clauses['conditions'] as &$where_info) {
      // Build a matrix of our possible relationships against fields we need to
      // switch.
      foreach ($relationships as $relationship) {
        foreach ($test_fields as $field) {
          if (is_string($where_info['field']) && $where_info['field'] == "$relationship.$field") {
            $alias = cps_ensure_revision_table($entity_type, $query, $relationship);
            if ($alias) {
              // Change the base table to use the revision table instead.
              $where_info['field'] = "$alias.$field";
            }
          }
        }
      }
    }
  }
}

/**
 * Helper for hook_views_query_alter().
 */
function cps_ensure_cps_table($entity_type, $query, $relationship) {
  $changeset_id = cps_get_current_changeset();
  $changeset = cps_changeset_load($changeset_id);

  if (isset($query->tables[$relationship]['cps_entity'])) {
    return $query->tables[$relationship]['cps_entity']['alias'];
  }

  $table_data = views_fetch_data($query->relationships[$relationship]['base']);

  // Construct the join.
  $join = new views_join();
  $join->definition = array(
    'table' => 'cps_entity',
    'field' => 'entity_id',
    'left_table' => $relationship,
    'left_field' => $table_data['table']['base']['field'],
    'extra' => array(
      array(
        'field' => 'entity_type',
        'value' => $entity_type,
      ),
      array(
        'field' => 'changeset_id',
        'value' => $changeset_id,
      )
    ),
    'type' => $changeset->published ? 'INNER' : 'LEFT',
  );

  $join->construct();
  $join->adjusted = TRUE;

  return $query->queue_table('cps_entity', $relationship, $join);
}

/**
 * Add the revision table for the entity type for the current changeset to a query.
 *
 * @param views_plugin_query $query
 *   The views query plugin.
 * @param string $relationship
 *   The name of the relationship.
 *
 * @return string $alias
 *   The alias of the relationship.
 */
function cps_ensure_revision_table($entity_type, $query, $relationship) {
  // Get the alias for the {cps_entity} table we chain off of in the COALESCE
  $cps_entity_table = cps_ensure_cps_table($entity_type, $query, $relationship);

  // Get the name of the revision table and revision key.
  $entity_info = entity_get_info($entity_type);
  $table = $entity_info['revision table'];
  $field = $entity_info['entity keys']['revision'];

  // Add this table only once.
  if (isset($query->tables[$relationship][$table])) {
    $alias = $query->tables[$relationship][$table]['alias'];
    // If this table previously existed, but was not added by us, we need
    // to modify the join and make sure that $cps_entity_table comes first.
    if (empty($query->table_queue[$alias]['join']->cps_adjusted)) {
      $query->table_queue[$alias]['join'] = cps_get_revision_table_join($relationship, $table, $field, $cps_entity_table);
      // We also have to ensure that our cps_entity_table comes before this.
      cps_move_entity_table($query, $cps_entity_table, $alias);
    }

    return $alias;
  }

  // Construct the join.
  $join = cps_get_revision_table_join($relationship, $table, $field, $cps_entity_table);
  return $query->queue_table($table, $relationship, $join);
}

/**
 * Fetch a join object for a revision table, constructed to use cps_entity
 *
 * @param string $relationship
 *   The $relationship to use in the view.
 * @param $table
 *   The table name.
 * @param $field
 *   The field to join on.
 * @param $cps_entity_table
 *   The alias of the cps_entity table joined to the main entity table.
 *
 * @return views_join
 *   An adjusted views join object to add to the query.
 */
function cps_get_revision_table_join($relationship, $table, $field, $cps_entity_table) {
  $join = new views_join();
  $join->definition = array(
    'table' => $table,
    'field' => $field,
    // Making this explicitly null allows $left_table to be a formula
    'left_table' => NULL,
    'left_field' => "COALESCE($cps_entity_table.revision_id, $relationship.$field)",
  );

  $join->construct();
  $join->adjusted = TRUE;
  $join->cps_adjusted = TRUE;

  return $join;
}

/**
 * Move a cps_entity table to appear before $alias
 *
 * Because CPS chains possibly pre-existing tables onto the cps_entity table,
 * we have to ensure that the cps_entity table appears in the query before
 * the alias it's chained on or the SQL is invalid. This uses array_slice
 * to reconstruct the query table_queue object.
 *
 * @param views_plugin_query_default $query
 *   The SQL query object.
 * @param $cps_entity_table
 *   The alias of the CPS entity table
 * @param $alias
 *   The alias of the table it needs to appear before.
 */
function cps_move_entity_table($query, $cps_entity_table, $alias) {
  $keys = array_keys($query->table_queue);
  $current_index = array_search($cps_entity_table, $keys);
  $index = array_search($alias, $keys);

  // If it's already before our table, we don't need to move it, as we
  // could accidentally move it forward.
  if ($current_index < $index) {
    return;
  }
  $splice = array($cps_entity_table => $query->table_queue[$cps_entity_table]);
  unset($query->table_queue[$cps_entity_table]);

  // Now move the item to the proper location in the array. Don't use array_splice
  // because that breaks indices.
  $query->table_queue = array_slice($query->table_queue, 0, $index, TRUE) +
    $splice +
    array_slice($query->table_queue, $index, NULL, TRUE);
}

/**
 * Fetch a list of fields that exist on both the revision and the main table.
 * @return array
 */
function cps_get_revision_fields($entity_type) {
  $entity_info = entity_get_info($entity_type);
  $base_schema = drupal_get_schema($entity_info['base table']);
  $revision_schema = drupal_get_schema($entity_info['revision table']);

  $fields = array();
  foreach ($revision_schema['fields'] as $name => $info) {
    if (isset($base_schema['fields'][$name]) && $name != $entity_info['entity keys']['id'] && $name != $entity_info['entity keys']['revision']) {
      $fields[] = $name;
    }
  }

  return $fields;
}
