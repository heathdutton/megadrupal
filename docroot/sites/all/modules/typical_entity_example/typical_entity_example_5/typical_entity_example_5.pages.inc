<?php

/**
 * @file
 * Callbacks for viewing entities.
 */

/**
 * Entity view callback.
 * 
 * @param object $entity
 *   Entity to view.
 * 
 * @return array
 *   Renderable array.
 */
function typical_entity_example_5_view($entity) {
  drupal_set_title(entity_label('typical_entity_example_5', $entity));

  // Return automatically generated view page.
  return entity_view('typical_entity_example_5', array(entity_id('typical_entity_example_5', $entity) => $entity), 'full');
}

/**
 * Show list of existing entity revisions.
 */
function typical_entity_example_5_revisions($entity) {
  drupal_set_title(t('Entity revisions'));

  // Get revisions.
  $resivions = db_query('SELECT r.revision_id, r.title, r.description FROM {typical_entity_example_5_revision} r WHERE r.teid = :teid ORDER BY r.revision_id DESC', array(':teid' => $entity->teid));

  // Show these revisions in table.
  $header = array(
    array('data' => t('Revision ID')),
    array('data' => t('Title')),
    array('data' => t('Description')),
  );
  foreach ($resivions as $revision) {
    $rows[] = array(
      array('data' => $revision->revision_id),
      array('data' => $revision->title),
      array('data' => $revision->description),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}
