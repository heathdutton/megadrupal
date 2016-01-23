<?php
/**
 * @file
 * Page callbacks for entity_collections.
 */

/**
 * Render an entity collection page.
 */
function entity_collection_page(EntityCollection $entity_collection) {
  $contexts = $entity_collection->contexts = _entity_collection_get_contexts($entity_collection);
  if (count($contexts) != count($entity_collection->requiredcontexts)) {
    return drupal_get_form('entity_collection_context_input_form', $entity_collection);
  }
  else {
    drupal_set_title($entity_collection->title);
    $content = $entity_collection->buildContent('full');
    $content['#contextual_links'] = array(
      'entity-collection' => array('contextual-links', array($entity_collection->eid)),
    );
    return $content;
  }
}
