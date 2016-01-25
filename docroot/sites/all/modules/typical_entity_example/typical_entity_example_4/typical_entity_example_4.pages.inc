<?php

/**
 * @file
 * Callbacks for viewing entities.
 */

/**
 * Show list of existing entities.
 */
function typical_entity_example_4_list() {
  drupal_set_title(t('Typical Entity 4'));

  // Show link to the entity "Add" page.
  $output = '<br/>';
  $output .= theme_link(
    array(
      'text' => t('Add new entity'),
      'path' => 'typical_entity_example_4/add',
      'options' => array(
        'attributes' => array(),
        'html' => TRUE,
      ),
    )
  );

  // Show list of existing entities.
  $entities = entity_load('typical_entity_example_4');
  $items = array();
  foreach ($entities as $entity) {
    $entity_uri = entity_uri('typical_entity_example_4', $entity);
    $items[] = l(entity_label('typical_entity_example_4', $entity), $entity_uri['path']);
  }
  $output .= theme_item_list(
    array(
      'items' => $items,
      'title' => t('List of existing entities.'),
      'type' => 'ul',
      'attributes' => array(),
    )
  );

  return $output;
}

/**
 * Entity view callback.
 * 
 * @param object $entity
 *   Entity to view.
 * 
 * @return array
 *   Renderable array.
 */
function typical_entity_example_4_view($entity) {
  drupal_set_title(entity_label('typical_entity_example_4', $entity));

  // Return automatically generated view page.
  return entity_view(
    'typical_entity_example_4',
    array(entity_id('typical_entity_example_4', $entity) => $entity),
    'full'
  );
}
