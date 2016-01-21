<?php

/**
 * @file
 * A field storing arbitrary relations between entities.
 */

/**
 * Implements hook_field_info().
 */
function awesomerelationship_field_info() {
  return array(
    'awesomerelationship' => array(
      'label' => t('Awesome Relationship'),
      'description' => t('This field stores relationships between entities.'),
      'settings' => array('allowed_values' => '', 'allowed_values_function' => ''),
      'default_widget' => 'awesomerelationship_default',
      'default_formatter' => 'awesomerelationship_default',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function awesomerelationship_field_is_empty() {
  return FALSE;
}

/**
 * Implements hook_field_insert().
 */
function awesomerelationship_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $insert1 = db_insert('relationship')->fields(array('predicate'));
  $insert2 = db_insert('relationship_data')->fields(array('relationship_id', 'entity_type', 'entity_id'));
  foreach ($items as $item) {
    if (!empty($item['entity_id'])) {
      $relationship_id = $insert1->values(array($field['field_name']))->execute();
      $insert2->values(array($relationship_id, $item['entity_type'], $item['entity_id']));
      $insert2->values(array($relationship_id, $entity_type, $entity_id));
    }
  }
  $insert2->execute();
  $items = array();
}

/**
 * Implements hook_field_update().
 */
function awesomerelationship_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  awesomerelationship_field_delete($entity_type, $entity, $field, $instance, $langcode, $items);
  awesomerelationship_field_insert($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_delete().
 */
function awesomerelationship_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($entity_id) = entity_extract_ids($entity_type, $entity);
  $result = db_query('SELECT relationship_id FROM {relationship_data} WHERE entity_type = :entity_type AND entity_id = :entity_id', array(':entity_type' => $entity_type, ':entity_id' => $entity_id));
  foreach ($result as $row) {
    db_delete('relationship')->condition('relationship_id', $row->relationship_id)->execute();
    db_delete('relationship_data')->condition('relationship_id', $row->relationship_id)->execute();
  }
}

/**
 * Implements hook_field_load().
 */
function awesomerelationship_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  foreach ($entities as $entity) {
    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $entity_ids[] = $id;
  }
  $query = db_select('relationship', 'base')
    ->condition('l.entity_type', $entity_type)
    ->condition('l.entity_id', $entity_ids);
  _awesomerationships_query_helper($query);
  foreach ($query->execute() as $item) {
    $items[$item->left_entity_id][] = array(
      'relationship_id' => $item->relationship_id,
      'predicate' => $item->predicate,
      'entity_id' => $item->right_entity_id,
      'entity_type' => $item->right_entity_type,
    );
  }
}

/**
 * Implements hook_field_widget_info().
 */
function awesomerelationship_field_widget_info() {
  return array(
    'awesomerelationship_default' => array(
      'label' => t('Awesome relationship chooser'),
      'field types' => array('awesomerelationship'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function awesomerelationship_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element['#type'] = 'fieldset';
  $element['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#options' => drupal_map_assoc(array_keys(entity_get_info())),
    '#default_value' => isset($items[$delta]) ? $items[$delta]['entity_type'] : '',
  );
  $element['entity_id'] = array(
    '#title' => t('Entity ID'),
    '#type' => 'textfield',
    '#default_value' => isset($items[$delta]) ? $items[$delta]['entity_id'] : '',
  );
  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function awesomerelationship_field_formatter_info() {
  return array(
    'awesomerelationship_default' => array(
      'label' => t('Default'),
      'field types' => array('awesomerelationship'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function awesomerelationship_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $list = array();
  foreach ($items as $item) {
    $uri = entity_uri($item['entity_type'], $item['entity']);
    $list[] = l($item['entity_type'], $uri['path'], $uri['options']);
  }
  return array(
    '#theme' => 'item_list',
    '#items' => $list,
  );
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function awesomerelationship_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  $entities_to_load = array();
  foreach ($items as $key => $item) {
    foreach ($item as $delta => $value) {
      $entities_to_load[$value['entity_type']][] = $value['entity_id'];
      $lookup[$value['entity_type']][$value['entity_id']][] = array($key, $delta);
    }
  }
  foreach ($entities_to_load as $entity_type => $ids) {
    $entities = entity_load($entity_type, $ids);
    foreach ($entities as $entity_id => $entity) {
      foreach ($lookup[$entity_type][$entity_id] as $data) {
        $items[$data[0]][$data[1]]['entity'] = $entity;
      }
    }
  }
}
