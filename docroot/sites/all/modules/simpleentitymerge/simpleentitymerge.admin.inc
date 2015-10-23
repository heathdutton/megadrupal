<?php

/**
 * An array of available stages.
 *
 * @return array
 */
function _simpeentitymerge_admin_form_stages() {
  $stages = array(
    'select bundle',
    'select instances',
    'compare instances',
  );
  return $stages;
}

/**
 * A multi-step form for selecting entities to merge together.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function simpleentitymerge_admin_form($form, &$form_state) {
  $form_state['stage'] = empty($form_state['stage']) ? 'select bundle' : $form_state['stage'];
  $preserve = array('entity_bundle_source', 'entity_bundle_target', 'source', 'target');

  // Previous form state values.
  foreach ($preserve as $form_item) {
    if (!empty($form_state['values'][$form_item])) {
      $form[$form_item] = array(
        '#type' => 'hidden',
        '#value' => $form_state['values'][$form_item],
      );
      if (!empty($form_state['complete form'][$form_item]['#type']) && $form_state['complete form'][$form_item]['#type'] === 'entityreference' && !empty($form_state['complete form'][$form_item]['#era_entity_type']) && !empty($form_state['values'][$form_item][0]['entity_id']) && !empty($form_state['values'][$form_item][0]['entity_label'])) {
        $form[$form_item]['#value'] = $form_state['values'][$form_item][0]['entity_label'] . ' (' . $form_state['values'][$form_item][0]['entity_id'] . ')';
      }
    }
  }

  $function = '_simpleentitymerge_admin_form__' . str_replace(' ', '_', $form_state['stage']);
  if (function_exists($function)) {
    $function($form, $form_state);
  }

  return $form;
}

/**
 * Validate simpleentitymerge_admin_form().
 *
 * @param $form
 * @param $form_state
 */
function simpleentitymerge_admin_form_validate($form, &$form_state) {
  if ($form_state['stage'] === 'select instances') {
    if (isset($form_state['values']['source'][0]) && isset($form_state['values']['target'][0])) {
      $source = $form_state['values']['source'][0];
      $target = $form_state['values']['target'][0];
      if ($source['entity_id'] === $target['entity_id'] && $source['entity_type'] === $target['entity_type'] && $source['entity_bundle'] === $target['entity_bundle']) {
        form_set_error('target', 'Target must not be the same as source.');
      }
    }
  }
}

/**
 * Submit function for simpleentity_admin_form().
 *
 * Keep the form from ever properly submitting so we have a multistep form or
 * process the entities to be merged.
 *
 * @param array $form
 * @param array $form_state
 */
function simpleentitymerge_admin_form_submit($form, &$form_state) {
  if ($form_state['stage'] === 'compare instances' && !empty($form_state['values']['op']) && $form_state['values']['op'] !== 'Back') {
    $source_type = !empty($form_state['values']['source_type']) ? $form_state['values']['source_type'] : false;
    $source_id = !empty($form_state['values']['source_id']) ? $form_state['values']['source_id'] : false;
    $target_type = !empty($form_state['values']['target_type']) ? $form_state['values']['target_type'] : false;
    $target_id = !empty($form_state['values']['target_id']) ? $form_state['values']['target_id'] : false;
    $fields = !empty($form_state['values']['fields']) ? $form_state['values']['fields'] : array();
    $properties = !empty($form_state['values']['properties']) ? $form_state['values']['properties'] : array();
    $foreign_keys = !empty($form_state['values']['foreignkeys']) ? $form_state['values']['foreignkeys'] : array();

    if ($source_type && $source_id && $target_type && $target_id) {
      $source_entity = entity_load($source_type, array($source_id));
      $target_entity = entity_load($target_type, array($target_id));
      if (!empty($source_entity[$source_id]) && !empty($target_entity[$target_id])) {

        // Filter out which fields the user selected to overwrite.
        foreach ($fields as $field_name => $info) {
          if ($info['doit'] === 0) {
            unset($fields[$field_name]);
          }
        }
        $fields = array_keys($fields);

        // Filter out which properties the user selected to overwrite.
        foreach ($properties as $property_name => $info) {
          if ($info['doit'] === 0) {
            unset($properties[$property_name]);
          }
        }
        $properties = array_keys($properties);

        // Filter out which foreign keys the user selected to overwrite.
        foreach ($foreign_keys as $foreign_key => $info) {
          if ($info['doit'] === 0) {
            unset($foreign_keys[$foreign_key]);
          }
        }

        // Merge the properties and fields.
        $merge_result = _simpleentitymerge_merge_entities($source_entity[$source_id], $source_type, $target_entity[$target_id], $target_type, $fields, $properties);
        if ($merge_result !== FALSE) {
          $target_uri = entity_uri($target_type, $target_entity[$target_id]);
          $text = !empty($target_uri['path']) && !empty($target_uri['options']) ? l(t('Complete'), $target_uri['path'], $target_uri['options']) : t('Completed item');
          drupal_set_message($text);

          $batch = array(
            'title' => t('Updating Entities'),
            'operations' => array(),
            'finished' => '_simpleentitymerge_batch_finished',
            'file' => drupal_get_path('module', 'simpleentitymerge') . '/simpleentitymerge.admin.inc',
          );

          // Update entity reference fields.
          if ($form_state['values']['update_references'] === 1) {
            $batch['operations'][] = array('_simpleentitymerge_update_references_field', array($source_entity[$source_id], $source_type, $target_entity[$target_id], $target_type));
          }

          // Update miscellaneous references.
          if (count($foreign_keys) > 0) {
            $batch['operations'][] = array('_simpleentitymerge_update_tables', array($source_entity[$source_id], $source_type, $target_entity[$target_id], $target_type, $foreign_keys));
          }

          // Delete the source entity.
          if ($form_state['values']['delete_source'] === 1) {
            $batch['operations'][] = array('_simpleentitymerge_delete_entity', array($source_type, $source_id));
          }

          if (!empty($batch['operations'])) {
            batch_set($batch);
          }
        }
      }
    }
    $form_state['stage'] = 'finished';
  }
  else {
    $stages = _simpeentitymerge_admin_form_stages();

    // Find what stage we're currently at.
    reset($stages);
    for ($i = 0; $i < count($stages); $i++) {
      if ($stages[$i] === $form_state['stage']) {
        break;
      }
      else {
        next($stages);
      }
    }

    // If the "Back" button was used, return to the previous stage.
    if (!empty($form_state['values']['op']) && $form_state['values']['op'] === 'Back') {
      $form_state['stage'] = prev($stages);

      // Copy the form_state input to values.
      $form_state['values'] = $form_state['input'];
    }
    else {
      // Set the next stage.
      $form_state['stage'] = next($stages);
    }

    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Helper function to create the select bundle form.
 *
 * @param $form
 * @param $form_state
 */
function _simpleentitymerge_admin_form__select_bundle(&$form, &$form_state) {
  $options = array();
  $entities = field_info_bundles();
  foreach ($entities as $entity => $bundles) {
    $options[$entity] = array();
    foreach ($bundles as $bundle => $info) {
      $options[$entity][$entity . '--' . $bundle] = $info['label'];
    }
  }
  $form['entity_bundle_source'] = array(
    '#title' => t('Source'),
    '#type' => 'select',
    '#options' => $options,
    '#required' => TRUE,
  );
  $form['entity_bundle_target'] = array(
    '#title' => t('Target'),
    '#type' => 'select',
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
}

/**
 * Helper function to create the select instances form.
 *
 * @param $form
 * @param $form_state
 */
function _simpleentitymerge_admin_form__select_instances(&$form, &$form_state) {
  $form_source = (!empty($form_state['values']['op']) && $form_state['values']['op'] === 'Back') ? 'input' : 'values';
  $entity_bundle_source = explode('--', $form_state[$form_source]['entity_bundle_source'], 2);
  $entity_bundle_target = explode('--', $form_state[$form_source]['entity_bundle_target'], 2);
  $form['source'] = array(
    '#type' => 'entityreference',
    '#title' => t('Source'),
    '#era_entity_type' => $entity_bundle_source[0],
    '#era_bundles' => array($entity_bundle_source[1]),
    '#era_cardinality' => 3,
    '#required' => TRUE,
  );

  $form['target'] = array(
    '#type' => 'entityreference',
    '#title' => t('Target'),
    '#era_entity_type' => $entity_bundle_target[0],
    '#era_bundles' => array($entity_bundle_target[1]),
    '#era_cardinality' => 3,
    '#required' => TRUE,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#limit_validation_errors' => array(),
    '#submit' => array('simpleentitymerge_admin_form_submit'),
    '#value' => t('Back'),
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
}

/**
 * Helper function to create the compare instances table/form.
 *
 * @param $form
 * @param $form_state
 */
function _simpleentitymerge_admin_form__compare_instances(&$form, &$form_state) {
  $source = $form_state['values']['source'][0];
  $target = $form_state['values']['target'][0];
  $source_entity = entity_load($source['entity_type'], array($source['entity_id']));
  $target_entity = entity_load($target['entity_type'], array($target['entity_id']));
  if (!empty($source_entity[$source['entity_id']]) && !empty($target_entity[$target['entity_id']])) {
    $form['source_type'] = array(
      '#type' => 'hidden',
      '#value' => $source['entity_type'],
    );
    $form['source_id'] = array(
      '#type' => 'hidden',
      '#value' => $source['entity_id'],
    );
    $form['target_type'] = array(
      '#type' => 'hidden',
      '#value' => $target['entity_type'],
    );
    $form['target_id'] = array(
      '#type' => 'hidden',
      '#value' => $target['entity_id'],
    );
    $form['fields'] = array(
      '#tree' => TRUE,
    );

    // Iterate through every field available, regardless of what it's attached to.
    $field_info_instances = field_info_instances();
    $bundle_fields = $field_info_instances[$target['entity_type']][$target['entity_bundle']];
    foreach ($bundle_fields as $field_name => $field) {

      // Pick out fields that are available on both the source and target entities.
      if (isset($field_info_instances[$target['entity_type']][$target['entity_bundle']][$field_name]) && isset($field_info_instances[$source['entity_type']][$source['entity_bundle']][$field_name])) {
        $form['fields'][$field_name]['#label'] = $field['label'];

        // Load the field items for the source and target entity.
        $source_items = field_get_items($source['entity_type'], $source_entity[$source['entity_id']], $field_name);
        $target_items = field_get_items($target['entity_type'], $target_entity[$target['entity_id']], $field_name);

        // List the field item values.
        if (is_array($source_items)) {
          foreach ($source_items as $source_item) {
            $view = field_view_value($source['entity_type'], $source_entity[$source['entity_id']], $field_name, $source_item);
            $form['fields'][$field_name]['#source_value'] = isset($view['#markup']) ? $view['#markup'] : '';
          }
        }
        if (is_array($target_items)) {
          foreach ($target_items as $target_item) {
            $view = field_view_value($target['entity_type'], $target_entity[$target['entity_id']], $field_name, $target_item);
            $form['fields'][$field_name]['#target_value'] = isset($view['#markup']) ? $view['#markup'] : '';
          }
        }

        // Allow the field to be selected for merging.
        $form['fields'][$field_name]['doit'] = array(
          '#type' => 'checkbox',
          '#default_value' => 0,
        );

      }
    }

    // Get the entity properties if they are of the same entity type.
    if ($source['entity_type'] === $target['entity_type']) {
      $form['properties'] = array(
        '#tree' => TRUE,
      );

      $entity_properties = entity_get_property_info();
      $allowed_properties = _simpleentitymerge_allowed_entity_properties();
      if (isset($allowed_properties[$source['entity_type']])) {
        foreach ($allowed_properties[$source['entity_type']] as $property) {
          if (in_array($property, array_keys($entity_properties[$source['entity_type']]['properties']))) {

            $form['properties'][$property]['#source_value'] = isset($source_entity[$source['entity_id']]->{$property}) ? $source_entity[$source['entity_id']]->{$property} : '';
            $form['properties'][$property]['#target_value'] = isset($target_entity[$target['entity_id']]->{$property}) ? $target_entity[$target['entity_id']]->{$property} : '';

            // Allow the property to be selected for merging.
            $form['properties'][$property]['doit'] = array(
              '#type' => 'checkbox',
              '#default_value' => 0,
            );
          }
        }
      }
    }

    // Show database tables with foreign keys that relate to the entity type, if the
    // source and target are of the same entity.
    if ($source['entity_type'] === $target['entity_type']) {
      $schema = drupal_get_complete_schema();
      $entity_info = entity_get_info();
      $base_table = $entity_info[$source['entity_type']]['base table'];
      $id = $entity_info[$source['entity_type']]['entity keys']['id'];
      $form['foreignkeys'] = array(
        '#tree' => TRUE,
      );
      foreach ($schema as $table => $info) {
        if (isset($info['foreign keys'])) {
          foreach ($info['foreign keys'] as $foreign_key_info) {
            if (isset($foreign_key_info['table']) && $foreign_key_info['table'] === $base_table && isset($foreign_key_info['columns']) && count($foreign_key_info['columns']) > 0) {
              foreach ($foreign_key_info['columns'] as $column => $reference) {
                if ($reference === $id) {
                  $form['foreignkeys'][$table]['#columns'][] = $column;

                  $form['foreignkeys'][$table]['doit'] = array(
                    '#type' => 'checkbox',
                    '#default_value' => 0,
                  );

                  if (isset($info['primary key']) && in_array($id, $info['primary key'])) {
                    $form['foreignkeys'][$table]['doit']['#disabled'] = TRUE;
                    $form['foreignkeys'][$table]['doit']['#default_value'] = 0;
                  }
                }
              }
            }
          }
        }
      }
    }

    $form['update_references'] = array(
      '#type' => 'checkbox',
      '#title' => 'Replace entity references (Resaves entities fully)',
      '#default_value' => 0,
    );

    $form['delete_source'] = array(
      '#type' => 'checkbox',
      '#title' => 'Delete the source item',
      '#default_value' => 0,
    );

    $form['#theme'] = 'simpleentitymerge_table';
    $form['#attached'] = array(
      'css' => array(
        drupal_get_path('module', 'simpleentitymerge') . "/simpleentitymerge.css",
      ));
  }
  $form['back'] = array(
    '#type' => 'submit',
    '#limit_validation_errors' => array(),
    '#submit' => array('simpleentitymerge_admin_form_submit'),
    '#value' => t('Back'),
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
}

/**
 * Theme function to display the field values in a table and let the user select which to keep.
 */
function theme_simpleentitymerge_table($variables) {
  drupal_add_js('misc/tableselect.js');
  drupal_add_library('system', 'drupal.collapse');
  $form = $variables['form'];
  $output = '';

  if (array_key_exists('fields', $form)) {
    $header = array(t('Field Name'), t('Source Value'), t('Target Value'), array('class' => array('select-all')));
    $rows = array();
    foreach (element_children($form['fields']) as $field_name) {
      $rows[] = array(
        0 => t('@label (@name)', array('@name' => $field_name, '@label' => $form['fields'][$field_name]['#label'])),
        1 => (!empty($form['fields'][$field_name]['#source_value']) ? var_export($form['fields'][$field_name]['#source_value'], TRUE) : ''),
        2 => (!empty($form['fields'][$field_name]['#target_value']) ? var_export($form['fields'][$field_name]['#target_value'], TRUE) : ''),
        3 => drupal_render($form['fields'][$field_name]['doit']),
      );
    }
    if (count($rows) > 0) {
      $output .= theme_fieldset(array('element' => array(
          '#attributes' => array('class' => array('collapsible')),
          '#children' => '',
          '#description' => '',
          '#id' => NULL,
          '#title' => t('Fields'),
          '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
        )));
    }
  }

  if (array_key_exists('properties', $form)) {
    $header = array(t('Property'), t('Source Value'), t('Target Value'), array('class' => array('select-all')));
    $rows = array();
    foreach (element_children($form['properties']) as $field_name) {
      $rows[] = array(
        0 => $field_name,
        1 => (!empty($form['properties'][$field_name]['#source_value']) ? var_export($form['properties'][$field_name]['#source_value'], TRUE) : ''),
        2 => (!empty($form['properties'][$field_name]['#target_value']) ? var_export($form['properties'][$field_name]['#target_value'], TRUE) : ''),
        3 => drupal_render($form['properties'][$field_name]['doit']),
      );
    }
    if (count($rows) > 0) {
      $output .= theme_fieldset(array('element' => array(
          '#attributes' => array('class' => array('collapsible')),
          '#children' => '',
          '#description' => '',
          '#id' => NULL,
          '#title' => t('Properties'),
          '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
        )));
    }
  }

  if (array_key_exists('foreignkeys', $form)) {
    $header = array(t('Table'), t('Column'), array('class' => array('select-all')));
    $rows = array();
    foreach (element_children($form['foreignkeys']) as $table) {
      if (!empty($form['foreignkeys'][$table]['#columns'])) {
        $columns = implode(', ', $form['foreignkeys'][$table]['#columns']);
        $rows[] = array(
          0 => $table,
          1 => (isset($form['foreignkeys'][$table]['doit']['#disabled']) && $form['foreignkeys'][$table]['doit']['#disabled'] === TRUE) ? $columns . ' (contains primary key)' : $columns,
          2 => drupal_render($form['foreignkeys'][$table]['doit']),
        );
      }
    }
    if (count($rows) > 0) {
      $output .= theme_fieldset(array('element' => array(
          '#attributes' => array('class' => array('collapsible')),
          '#children' => '',
          '#description' => t('IDs of entities are used in many places. For example, to retain the files associated with a user when merging.'),
          '#id' => NULL,
          '#title' => t('Database Foreign Keys'),
          '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
        )));
    }
  }

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Overwrite the fields of the target entity with the source entity.
 *
 * @param stdClass $entity_source
 * @param string $entity_source_type
 * @param stdClass $entity_target
 * @param string $entity_target_type
 * @param array $fields
 */
function _simpleentitymerge_merge_entities($entity_source, $entity_source_type, $entity_target, $entity_target_type, $fields = array(), $properties = array()) {
  // Merge fields.
  $field_info_instances = field_info_instances();
  foreach ($fields as $field) {
    if (isset($field_info_instances[$entity_source_type][$entity_source->type][$field]) && isset($field_info_instances[$entity_target_type][$entity_target->type][$field])) {
      $entity_target->{$field} = $entity_source->{$field};
    }
  }

  // Merge properties.
  if ($entity_source_type === $entity_target_type) {
    $entity_property_info = entity_get_property_info();
    $allowed_properties = _simpleentitymerge_allowed_entity_properties();
    foreach ($properties as $property) {
      if (in_array($property, $allowed_properties[$entity_source_type]) && in_array($property, array_keys($entity_property_info[$entity_source_type]['properties']))) {
        $entity_target->{$property} = $entity_source->{$property};
      }
    }
  }

  // Save the entity.
  try {
    return entity_save($entity_target_type, $entity_target);
  }
  catch (Exception $e) {
    drupal_set_message($e->errorInfo[2], 'error');
    return FALSE;
  }
}

/**
 * Update all entity reference fields across all entities to point to a
 * different entity.
 *
 * @param stdClass $entity_source
 * @param string $entity_source_type
 * @param stdClass $entity_target
 * @param string $entity_target_type
 * @param array $context
 *   A BatchAPI compatible array for storing information on the current state of processing.
 */
function _simpleentitymerge_update_references_field($entity_source, $entity_source_type, $entity_target, $entity_target_type, &$context) {
  if (empty($context['sandbox'])) {
    $sandbox['results']['references'] = array();
    $context['sandbox']['entities'] = entity_get_info();
    $context['sandbox']['entity_types'] = array_keys($context['sandbox']['entities']);
    $context['sandbox']['entity_types_total'] = count($context['sandbox']['entity_types']);
    $context['sandbox']['current_entity'] = array_shift($context['sandbox']['entity_types']);

    $context['sandbox']['fields'] = array();
    $field_info_fields = field_info_fields();
    foreach ($field_info_fields as $field_name => $field) {
      if ($field['module'] === 'entityreference' && $field['type'] === 'entityreference' && in_array($entity_source_type, array_keys($field['bundles'])) && in_array($entity_target_type, array_keys($field['bundles']))) {
        $context['sandbox']['fields'][$field_name] = $field;
      }
    }
    $context['sandbox']['field_names'] = array_keys($context['sandbox']['fields']);
    $context['sandbox']['fields_total'] = count($context['sandbox']['fields']);
    $context['sandbox']['current_field'] = array_shift($context['sandbox']['field_names']);

    $context['sandbox']['target_id_property'] = $context['sandbox']['entities'][$entity_target_type]['entity keys']['id'];
    $context['sandbox']['source_id_property'] = $context['sandbox']['entities'][$entity_source_type]['entity keys']['id'];

    $context['sandbox']['progress'] = 0;
  }

  if (empty($context['sandbox']['current_entity']) || ($context['sandbox']['entity_types_total'] * $context['sandbox']['fields_total']) === 0) {
    $context['finished'] = 1;
  }
  else {
    $context['finished'] = 1 - ($context['sandbox']['progress'] / ($context['sandbox']['entity_types_total'] * $context['sandbox']['fields_total']));
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $context['sandbox']['current_entity'])
      ->fieldCondition($context['sandbox']['current_field'], 'target_id', $entity_source->{$context['sandbox']['source_id_property']})
      ->range(0, 5);
    $result = $query->execute();
    if (isset($result[$context['sandbox']['current_entity']]) && count($result[$context['sandbox']['current_entity']]) > 0) {
      $ids = array_keys($result[$context['sandbox']['current_entity']]);
      $items = entity_load($context['sandbox']['current_entity'], $ids);
      foreach ($items as $item) {
        if (isset($item->{$context['sandbox']['current_field']})) {
          foreach ($item->{$context['sandbox']['current_field']} as $language => $value) {
            foreach ($value as $delta => $targets) {
              if (isset($targets['target_id']) && $targets['target_id'] === $entity_source->{$context['sandbox']['source_id_property']}) {
                $item->{$context['sandbox']['current_field']}[$language][$delta]['target_id'] = $entity_target->{$context['sandbox']['target_id_property']};
              }
            }
          }
        }
        try {
          $saved = entity_save($context['sandbox']['current_entity'], $item);
          if ($saved === FALSE) {
            $id = entity_extract_ids($context['sandbox']['current_entity'], $item);
            $sandbox['results']['references'][] = $context['sandbox']['current_entity'] . ' - ' . $id[0];
          }
        }
        catch (Exception $e) {
          $id = entity_extract_ids($context['sandbox']['current_entity'], $item);
          $sandbox['results']['references'][] = $context['sandbox']['current_entity'] . ' - ' . $id[0];
        }
      }
    }
    else {
      // Move on to the next field.
      $context['sandbox']['current_field'] = array_shift($context['sandbox']['field_names']);
      // If all fields have been checked, move on to the next entity and reset
      // the field array.
      if (empty($context['sandbox']['current_field'])) {
        $context['sandbox']['progress']++;
        $context['sandbox']['current_entity'] = array_shift($context['sandbox']['entity_types']);
        $context['sandbox']['field_names'] = array_keys($context['sandbox']['fields']);
        $context['sandbox']['current_field'] = array_shift($context['sandbox']['field_names']);
      }
    }
  }
}

/**
 * Update any remaining tables referencing an entity's id.
 *
 * @param stdClass $entity_source
 * @param string $entity_source_type
 * @param stdClass $entity_target
 * @param string $entity_target_type
 * @param array $selected_tables
 * @param array $context
 *   A BatchAPI compatible array for storing information on the current state of processing.
 */
function _simpleentitymerge_update_tables($entity_source, $entity_source_type, $entity_target, $entity_target_type, $selected_tables ,&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['schema'] = drupal_get_complete_schema();
    $context['sandbox']['schema_total'] = count($context['sandbox']['schema']);
    $context['sandbox']['entity_info'] = entity_get_info();
    $context['sandbox']['base_table'] = $context['sandbox']['entity_info'][$entity_source_type]['base table'];
    $context['sandbox']['id'] = $context['sandbox']['entity_info'][$entity_source_type]['entity keys']['id'];
    $context['sandbox']['source_id'] = entity_extract_ids($entity_source_type, $entity_source);
    $context['sandbox']['target_id'] = entity_extract_ids($entity_target_type, $entity_target);

    $context['results']['tables'] = array();
    $context['results']['tables_not'] = array();
  }

  if ($entity_source_type !== $entity_target_type || !isset($context['sandbox']['source_id'][0]) || !isset($context['sandbox']['target_id'][0]) || count($context['sandbox']['schema']) <= 0 || $context['sandbox']['schema_total'] === 0) {
    $context['finished'] = 1;
  }
  else {
    $context['finished'] = 1 - (count($context['sandbox']['schema']) / $context['sandbox']['schema_total']);
    reset($context['sandbox']['schema']);
    $table = key($context['sandbox']['schema']);
    $info = array_shift($context['sandbox']['schema']);
    if (isset($info['foreign keys'])) {
      foreach ($info['foreign keys'] as $foreign_key_info) {
        if (isset($foreign_key_info['table']) && $foreign_key_info['table'] === $context['sandbox']['base_table'] && isset($foreign_key_info['columns']) && count($foreign_key_info['columns']) > 0) {
          foreach ($foreign_key_info['columns'] as $column => $reference) {
            if ($reference === $context['sandbox']['id']) {
              if (!in_array($context['sandbox']['id'], $info['primary key'])) {
                if (isset($selected_tables[$table]['doit']) && $selected_tables[$table]['doit'] === 1) {
                  $num_updated = db_update($table)
                    ->fields(array(
                        $column => $context['sandbox']['target_id'][0],
                      ))
                    ->condition($column, $context['sandbox']['source_id'][0])
                    ->execute();
                  $context['results']['tables'][] = $num_updated . ' ' . $column . ' columns updated in ' . $table;
                }
              }
              else {
                $context['results']['tables_not'][] = 'Check ' . $column . ' in ' . $table . ' because this is a primary key. Please update manually if necessary.';
              }
            }
          }
        }
      }
    }
  }
}

/**
 * Delete an entity.
 *
 * @param string $source_type
 * @param int $source_id
 * @param array $context
 *   A BatchAPI compatible array for storing information on the current state of processing.
 */
function _simpleentitymerge_delete_entity($source_type, $source_id, &$context) {
  if (!isset($context['message'])) {
    $context['message'] = '';
  }
  try {
    entity_delete($source_type, $source_id);
    $context['message'] .= '<br />' . t('Source entity deleted');
    $context['finished'] = 1;
  }
  catch (Exception $e) {
    $context['message'] .= '<br />' . t('Unable to delete source entity.');
  }
}

/**
 * The list of properties for entities that's allowable to merge.
 */
function _simpleentitymerge_allowed_entity_properties() {
  $list = array(
    'node' => array(
      'uid',
      'title',
      'status',
      'comment',
      'promote',
      'sticky',
      'language',
      'created',
    ),
    'user' => array(
      'name',
      'pass',
      'mail',
      'theme',
      'signature',
      'signature_format',
      'created',
      'status',
      'timezone',
      'language',
      'picture',
      'init',
      'data',
      'roles',
    ),
  );
  drupal_alter('simpleentitymerge_entity_properties', $list);
  return $list;
}

/**
 * Display messages once the batch operation has completed.
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function _simpleentitymerge_batch_finished($success, $results, $operations) {
  if (!$success) {
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }

  $message = '';
  if (isset($results['references']) && count($results['references']) > 0) {
    $message = count($results) . ' ' . t('were unable to be processed.');
    $message .= '<br />' . theme('item_list', array('items' => $results['references']));
  }
  if (isset($results['tables_not']) && count($results['tables_not']) > 0) {
    $message .= '<br />' . theme('item_list', array('items' => $results['tables_not']));
  }
  if (isset($results['tables']) && count($results['tables']) > 0) {
    $message .= '<br />' . theme('item_list', array('items' => $results['tables']));
  }
  drupal_set_message($message);
}
