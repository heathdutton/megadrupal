<?php

/**
 * @file
 * Hooks provided by the MongoDb Indexer.
 */

/**
 * Build the documents before sending them to mongodb.
 *
 * @param object $document
 *   The document object to be sent to MongoDB.
 * @param object $entity
 *   The entity being processed.
 * @param string $entity_type
 *   The entity type being processed.
 */
function hook_mongodb_indexer_document_build($document, $entity, $entity_type) {
  // Add your custom data to document.
}
