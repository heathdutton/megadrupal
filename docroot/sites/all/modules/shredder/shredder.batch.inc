<?php

/**
 * @file
 * Makes text-data in fields unreadable.
 */

/**
 * Butch finish function.
 */
function shredder_batch_finished($success, $results, $operations) {
  if ($success) {
    $entity_count = format_plural($results['count_changed_entities'], '1 entity', '@count entities');
    $errors = '';
    if (!empty($results['errors'])) {
      $errors = format_plural($results['errors'], '1 error', '@count errors') . t(' (see watchdog).');
    }
    drupal_set_message('Changed data for ' . $entity_count . '. ' . $errors);
  }
  else {
    drupal_set_message('Finished with errors.', 'error');
  }
}

/**
 * Returns array of entities what will be changed.
 */
function shredder_get_entities_to_change($form_state) {
  $supported_entity_properties = array(
    'title' => 'node',
    'name' => 'user',
    'mail' => 'user',
    'filename' => 'file',
    'subject' => 'comment',
  );

  $entity_info = entity_get_info();
  $not_changed_entity_types = $form_state['object']['entity_type'];
  $not_changed_bundles = $form_state['object']['bundles'];

  $query = db_select('shredder_changed_entities', 'sce')->fields('sce');
  $query->condition('sce.entity_type', array_keys($not_changed_entity_types), 'IN');
  $result = $query->execute();
  $changed_entities = array();
  if (!empty($result)) {
    foreach ($result as $entity) {
      $changed_entities[$entity->entity_type][$entity->entity_id] = $entity->entity_id;
    }
  }

  $entities = array();
  // Add entities for selected properties.
  foreach ($supported_entity_properties as $entity_property => $entity_type) {
    if (array_key_exists($entity_property, $form_state['object']['fields']) && array_key_exists($entity_type, $not_changed_entity_types)) {

      if (shredder_is_entity_have_bundles($entity_info, $entity_type)) {
        foreach (array_keys($entity_info[$entity_type]['bundles']) as $bundle) {
          if (array_key_exists($bundle, $not_changed_bundles)) {
            // Used EntityFieldQuery for taxonomy, because it's involve taxonomy_entity_query_alter hook for bundle.
            if ($entity_type == 'taxonomy_term') {
              shredder_add_taxonomy_to_change($entities, $changed_entities, $entity_type, $bundle);
            }
            else {
              shredder_add_entities_to_change($entities, $changed_entities, $entity_type, $bundle);
            }
            unset($not_changed_bundles[$bundle]);
          }
        }
      }
      else {
        shredder_add_entities_to_change($entities, $changed_entities, $entity_type);
      }
      unset($not_changed_entity_types[$entity_type]);
    }
  }
  if (!empty($not_changed_entity_types) || !empty($not_changed_bundles)) {
    // Add entities for selected fields.
    foreach ($form_state['object']['fields'] as $field => $field_title) {
      if (!array_key_exists($field, $supported_entity_properties)) {

        $field_info = field_info_field($field);
        foreach ($field_info['bundles'] as $entity_type => $bundles) {
          if (array_key_exists($entity_type, $not_changed_entity_types)) {

            if (shredder_is_entity_have_bundles($entity_info, $entity_type)) {
              foreach ($bundles as $bundle) {
                if (array_key_exists($bundle, $not_changed_bundles)) {
                  if ($entity_type == 'taxonomy_term') {
                    shredder_add_taxonomy_to_change($entities, $changed_entities, $entity_type, $bundle, $field);
                  }
                  else {
                    shredder_add_entities_to_change($entities, $changed_entities, $entity_type, $bundle);
                  }
                  unset($not_changed_bundles[$bundle]);
                }
              }
            }
            else {
              shredder_add_entities_to_change($entities, $changed_entities, $entity_type);
            }
            unset($not_changed_entity_types[$entity_type]);
          }
        }
      }
    }
  }
  return $entities;
}

/**
 * Adds taxonomy terms to $entities array.
 */
function shredder_add_taxonomy_to_change(&$entities, $changed_entities, $entity_type, $bundle = NULL, $field = NULL) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type);
  if ($bundle) {
    $query->entityCondition('bundle', $bundle);
  }
  if ($field) {
    $query->fieldCondition($field, 'value', 'NULL', '!=');
  }
  $ids = $query->execute();
  if (!empty($ids)) {
    foreach ($ids[$entity_type] as $id => $entity) {
      if (empty($changed_entities[$entity_type][$id])) {
        $entities[$entity_type][] = $id;
      }
    }
  }
}

/**
 * Adds entities to $entities array.
 */
function shredder_add_entities_to_change(&$entities, $changed_entities, $entity_type, $bundle = NULL) {
  $entity_info = entity_get_info($entity_type);
  $base_table = $entity_info['base table'];
  $base_table_schema = drupal_get_schema($base_table);

  $select_query = db_select($base_table);
  $select_query->addField($base_table, $entity_info['entity keys']['id'], 'entity_id');
  $select_query->addExpression(':entity_type', 'entity_type', array(':entity_type' => $entity_type));

  if ($bundle) {
    // Handle bundles.
    if (!empty($entity_info['entity keys']['bundle'])) {
      $sql_field = $entity_info['entity keys']['bundle'];
      $having = FALSE;

      if (!empty($base_table_schema['fields'][$sql_field])) {
        $select_query->addField($base_table, $sql_field, 'bundle');
      }
    }
    else {
      $sql_field = 'bundle';
      $select_query->addExpression(':bundle', 'bundle', array(':bundle' => $entity_type));
      $having = TRUE;
    }
    if (!empty($entity_info['entity keys']['bundle'])) {
      $method = $having ? 'havingCondition' : 'condition';
      $select_query->$method($base_table . '.' . $sql_field, $bundle, '=');
    }
  }
  $ids = $select_query->execute()->fetchAllKeyed(0,0);

  if (!empty($ids)) {
    foreach ($ids as $id) {
      if (empty($changed_entities[$entity_type][$id])) {
        $entities[$entity_type][] = $id;
      }
    }
  }
}

/**
 * Changes field's and entity property's values.
 */
function shredder_shredder_values($entity_type, $entity_ids, &$context) {
  module_load_include('inc', 'shredder', 'shredder.admin');
  $supported_entity_properties = array(
    'title', 'name', 'mail', 'filename', 'subject',
  );

  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($entity_ids);
  }

  $entity_id = $entity_ids[$context['sandbox']['progress']];
  $entity = shredder_load_current_entity($entity_type, $entity_id, $context);

  // Change fields and properties and save entity.
  $object = shredder_cache_get('shredder');
  foreach (array_keys($object['fields']) as $field) {
    if (in_array($field, $supported_entity_properties)) {
      shredder_change_entity_property($entity, $field);
    }
    else {
      shredder_change_field($entity, $field);
    }
  }
  unset($object);

  $entity->revision = 0;
  $entity->nodechanges_skip = TRUE; // For nodechanges module.
  if (!empty($entity->need_save)) {
    try {
      if ($entity_type == 'field_collection_item') {
        $entity->save(TRUE); // Workaround, field_collection_item revisions don't save from entity_save. TRUE for skip host.
      }
      else {
        entity_save($entity_type, $entity);
      }
      if (empty($context['sandbox']['revisions'])) {
        shredder_add_to_result_table($entity_type, $entity_id);
        $context['results']['count_changed_entities']++;
      }
    } catch (Exception $e) {
      $error = t('Failed with ' . $entity_type . ' '. $entity_id);
      if (!empty($context['sandbox']['current_revision'])) {
        $error .= t(' (rev_id: @revision_id)', array('@revision_id' => $context['sandbox']['current_revision']));
      }
      watchdog('shredder', $error);
      $context['results']['errors']++;
    }
  }
  unset($entity);

  $context['message'] = t('Changed data for @current of @total @entity_type.',
    array('@current' => $context['sandbox']['progress'], '@total' => $context['sandbox']['max'], '@entity_type' => $entity_type));

  if (empty($context['sandbox']['revisions'])) {
    $context['sandbox']['progress']++;
  }
  else {
    $context['message'] .= t(' Changed revision: @revision_id.', array('@revision_id' => $context['sandbox']['current_revision']));
  }
  $context['finished'] = empty($context['sandbox']['max']) ? 1 : $context['sandbox']['progress'] / $context['sandbox']['max'];
}

/**
 * Load current entity with revisions.
 */
function shredder_load_current_entity($entity_type, $entity_id, &$context) {
  if (!empty($context['sandbox']['current_revision'])) {
    unset($context['sandbox']['current_revision']);
  }
  if (empty($context['sandbox']['revisions'])) {
    $entity_info = entity_get_info($entity_type);

    // Loads revisions for entity.
    if (!empty($entity_info['revision table'])) {

      $query = db_select($entity_info['revision table'], 'revision');
      $query->fields('revision', array($entity_info['entity keys']['id'], $entity_info['entity keys']['revision']))
        ->condition('revision.' . $entity_info['entity keys']['id'], $entity_id);
      $revisions = $query
        ->execute()
        ->fetchAllAssoc($entity_info['entity keys']['revision']);
      unset($query);

      foreach ($revisions as $revision_id => $revision) {
        $context['sandbox']['revisions'][$revision_id] = $revision_id;
      }
    }
    else {
      $entity = array_shift(entity_load($entity_type, array($entity_id)));
    }
  }
  if (empty($entity)) {
    $context['sandbox']['current_revision'] = array_shift($context['sandbox']['revisions']);
    $entity = entity_revision_load($entity_type, $context['sandbox']['current_revision']);
  }
  return $entity;
}

/**
 * Changes entity property's value.
 */
function shredder_change_entity_property(&$entity, $property) {
  if (!empty($entity->{$property})) {
    switch ($property) {
      case 'filename':
        $info = pathinfo($entity->{$property});
        $filename = basename($entity->{$property}, '.' . $info['extension']);
        $value = shredder_get_random_string(strlen($filename), FALSE) . '.' . $info['extension'];
        break;
      case 'mail':
        $value = 'user' . $entity->uid . '@' . shredder_get_random_string(4, FALSE) . '.' . shredder_get_random_string(3, FALSE);
        $entity->init = $value;
        break;
      case 'name':
        $value = 'user' . $entity->uid;
        break;
      default:
        $value = shredder_get_random_string(strlen($entity->{$property}));
        break;
    }
    $entity->{$property} = $value;
    $entity->need_save = TRUE;
  }
}

/**
 * Changes field's value.
 */
function shredder_change_field(&$entity, $field) {
  if (!empty($entity->{$field}[LANGUAGE_NONE])) {
    foreach ($entity->{$field}[LANGUAGE_NONE] as &$value) {
      $value['value'] = shredder_get_random_string(strlen($value['value']));
      if (!empty($value['summary'])) {
        $value['summary'] = shredder_get_random_string(strlen($value['summary']));
      }
    }
    $entity->need_save = TRUE;
  }
}

/**
 * Adds changed entities to result table.
 */
function shredder_add_to_result_table($entity_type, $entity_id) {
  db_insert('shredder_changed_entities')
    ->fields(array(
      'entity_id' => $entity_id,
      'entity_type' => $entity_type,
    ))
    ->execute();
}

/**
 * Returns TRUE if entity can have bundles.
 */
function shredder_is_entity_have_bundles($entity_info, $entity_type) {
  //->entityCondition not support comment's bundles. see drupal.org/node/1343708.
  if (!empty($entity_info[$entity_type]['entity keys']['bundle']) && $entity_type != 'comment') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Returns random string.
 *
 * @param $length
 *   Length of returned string.
 * @param $with_space
 *   Includes spaces if TRUE.
 *
 * @return
 *   Random string.
 */
function shredder_get_random_string($length = 10, $with_space = TRUE) {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  if ($with_space) {
    $characters .= ' ';
  }
  $randomString = '';
  for ($i = 0; $i < $length; $i++) {
    $randomString .= $characters[rand(0, strlen($characters) - 1)];
  }
  return $randomString;
}