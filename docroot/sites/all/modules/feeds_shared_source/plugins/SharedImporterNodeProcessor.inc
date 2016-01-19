<?php
/**
 * @file
 * Class definition of FeedsNodeProcessor.
 */

/**
 * Creates nodes from feed items.
 */
class SharedImporterNodeProcessor extends FeedsNodeProcessor {
  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'use_importer_id' => TRUE,
      'use_feed_nid' => FALSE,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);

    $form['use_importer_id'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use importer ID'),
      '#description' => t('If checked then the importer ID (the machine name of this importer) will be used to restrict what imported nodes are checked for matching GUIDs.'),
      '#default_value' => $this->config['use_importer_id'],
    );

    $form['use_feed_nid'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use feed NID'),
      '#description' => t('If checked then the feed importer NID will be used when restricting which existing GUIDs are checked.'),
      '#default_value' => $this->config['use_feed_nid'],
    );

    return $form;
  }

  /**
   * Get nid of an existing feed item node if available.
   *
   * Retrieve the target entity's existing id if available. Otherwise return 0.
   *
   * Copied from FeedsProcessor::existingEntityId and tweaked to not care
   * about the feed_nid
   *
   * @ingroup mappingapi
   *
   * @param FeedsSource $source
   *   The source information about this import.
   *
   * @param FeedsParserResult $result
   *   A FeedsParserResult object.
   *
   * @return sstring
   *   The serial id of an entity if found, 0 otherwise.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {

    if ($nid = parent::existingEntityId($source, $result)) {

      return $nid;
    }

    $query = db_select('feeds_item')
      ->fields('feeds_item', array('entity_id'))
      ->condition('entity_type', $this->entityType());

    if ($this->config['use_importer_id']) {

      $query->condition('id', $source->id);
    }

    if ($this->config['use_feed_nid']) {

      $query->condition('feed_nid', $source->feed_nid);
    }

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
