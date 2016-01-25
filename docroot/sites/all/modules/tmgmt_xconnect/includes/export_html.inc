<?php
/**
 * @file
 * Exporter to export the Job to HTML.
 */

/**
 * Exporter to export the Job and its Items to HTML.
 */
class TmgmtXConnectExportHtml implements TmgmtXConnectExportInterface {
  /**
   * {@inheritdoc}
   */
  public function exportJob(TMGMTJob $job) {
    $data = array();
    foreach ($job->getItems() as $item) {
      $data = array_merge($data, $this->extractItemData($item));
    }
    return $this->createHtml($job, $data);
  }

  /**
   * {@inheritdoc}
   */
  public function exportJobItem(TMGMTJobItem $item) {
    $job = $item->getJob();
    $data = $this->extractItemData($item);
    return $this->createHtml($job, $data);
  }

  /**
   * Extract the data from a single Job Item.
   *
   * @param TMGMTJobItem $item
   *   The item to extract from.
   *
   * @return array
   *   The extracted data.
   */
  protected function extractItemData(TMGMTJobItem $item) {
    $item_data = $this->sortItemData($item, $item->getData());

    $raw_data = array_filter(
      tmgmt_flatten_data($item_data),
      '_tmgmt_filter_data'
    );

    $data = array();
    foreach ($raw_data as $key => $value) {
      $key_encoded = $this->encodeId($item->tjiid . '][' . $key);
      $data[$item->tjiid][$key_encoded] = $value;
    }

    return $data;
  }

  /**
   * Helper to sort the fields of the content to translate.
   *
   * NOTE: This will only sort the fields of a node entity.
   *
   * @param TMGMTJobItem $item
   *   The entity we need to translate.
   * @param array $data
   *   The id for the export.
   *
   * @return array
   *   Sorted id.
   */
  protected function sortItemData(TMGMTJobItem $item, array $data) {
    // TODO: Only for nodes, can we make this better?
    if ($item->item_type !== 'node') {
      return $data;
    }

    // Load the field information.
    $entity = entity_load_single($item->item_type, $item->item_id);
    if (!$entity) {
      return $data;
    }
    $entity_wrapper = entity_metadata_wrapper($item->item_type, $entity);
    $instances = field_info_instances($item->item_type, $entity_wrapper->getBundle());

    // Sort fields according to form widget weights.
    $sort = array();
    $i = 0;
    foreach ($data as $field_name => $field_data) {
      // Put fields without weight first.
      $weight = isset($instances[$field_name]['widget']['weight'])
        ? $instances[$field_name]['widget']['weight']
        : -100;

      // Make each weight value unique to avoid field to be overwritten by
      // fields with the same weight.
      $sort[($weight * 10000) + $i] = $field_name;

      $i++;
    }

    ksort($sort);
    $fields = array_flip($sort);

    return array_merge($fields, $data);
  }

  /**
   * Helper to create the actual HTML output.
   *
   * @param TMGMTJob $job
   *   The job to create the HTML for.
   * @param array $data
   *   The id to add to the HTML.
   *
   * @return string
   *   The rendered HTML.
   */
  protected function createHtml(TMGMTJob $job, array $data) {
    $source_language = $job->getTranslator()
      ->mapToRemoteLanguage($job->source_language);
    $target_language = $job->getTranslator()
      ->mapToRemoteLanguage($job->target_language);

    // Add the item id only if there is no more then one item in the data.
    $job_item_id = NULL;
    if (1 === count($data)) {
      $keys = array_keys($data);
      $job_item_id = array_shift($keys);
    }

    return theme(
      'tmgmt_xconnect_export_html',
      array(
        'job_id' => $job->tjid,
        'job_item_id' => $job_item_id,
        'source_language' => $source_language,
        'target_language' => $target_language,
        'data' => $data,
      )
    );
  }

  /**
   * Helper to encode a field id so it is safe to use as XML id.
   *
   * @param string $id
   *   The id to encode.
   *
   * @return string
   *   The encoded id.
   */
  protected function encodeId($id) {
    $raw = base64_encode($id);
    $clean = rtrim(strtr($raw, '+/', '-_'), '=');

    // Prefix with a b to enforce that the first character is a letter.
    return 'b' . $clean;
  }

}
