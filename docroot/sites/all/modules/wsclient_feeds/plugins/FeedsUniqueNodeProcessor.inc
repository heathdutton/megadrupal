<?php

/**
 * @file
 * Class definition of FeedsUniqueNodeProcessor.
 *
 * This is entirely a duplicate of FeedsNodeProcessor with one change -
 * it allows different feeds importers to update the same node.
 * If the GUID is the same, then *it is the same*.
 *
 * This reduces duplicates in imported data.
 *
 * It does this by removing the importer->id constraint when looking for
 * pre-existing content.
 *
 * Wrecks some Feeds assumptions by allowing different feed sources to update
 * the same target entity.
 * However it means that Feeds can now work as an aggregator API to collate
 * data about the same thing from different sources.
 *
 * FeedsProcessor::existingEntityId explicitly requires lookups to match the
 * Feeds Importer ID, even if the GUID is the same.
 * This can create duplicates if the same item is referred to in two feeds.
 *
 * I want GUID to mean something - that that item can be referred to uniquely
 * (globally) no matter how it was originaly imported.
 *
 * This DOES have potential issues with confusion over which feed 'owns' the
 * content. On the other hand, it removes the confusion of duplicates.
 *
 * EXPERIMENTAL
 *
 * author dman dan@coders.co.nz 2013-03
 */

/**
 * Creates nodes from feed items.
 */
class FeedsUniqueNodeProcessor extends FeedsNodeProcessor {

  /**
   * Retrieve the target entity's existing id if available. Otherwise return 0.
   *
   * Cloned from FeedsProcessor::existingEntityId
   * fractionally changed the query.
   *
   * @ingroup mappingapi
   *
   * @param FeedsSource $source
   *   The source information about this import.
   * @param FeedsParserResult $result
   *   A FeedsParserResult object.
   *
   * @return string
   *   The serial id of an entity if found, 0 otherwise.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    $query = db_select('feeds_item')
      ->fields('feeds_item', array('entity_id'))
      ->condition('feed_nid', $source->feed_nid)
      ->condition('entity_type', $this->entityType());

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'url':
          $entity_id = $query->condition('url', $value)->execute()->fetchField();
          break;

        case 'guid':
          $entity_id = $query->condition('guid', $value)->execute()->fetchField();
          break;

      }
      if (isset($entity_id)) {
        // Return with the content id found.
        return $entity_id;
      }
    }
    return 0;
  }

}
