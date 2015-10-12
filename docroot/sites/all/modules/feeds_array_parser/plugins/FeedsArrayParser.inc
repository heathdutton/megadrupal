<?php

/**
 * Parses a given file as a CSV file.
 */
class FeedsArrayParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $source_config = $source->getConfigFor($this);
    $state = $source->state(FEEDS_PARSE);
    
    $array = unserialize($fetcher_result->getRaw());
    if(!$array) {
      return;
    }

    // Report progress.
    $state->total = count($array);
    $state->pointer = key($array);
    $progress = $state->total;
    $state->progress($state->total, $progress);

    // Create a result object and return it.
    return new FeedsParserResult($array, $source->feed_nid);
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources() {
    return FALSE;
  }

  /**
   * Override parent::getSourceElement() to use only lower keys.
   */
  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key) {
    return parent::getSourceElement($source, $result, drupal_strtolower($element_key));
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return array();
  }

  /**
   * Source form.
   *
   * Show mapping configuration as a guidance for import form users.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['#weight'] = -10;

    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    $sources = $uniques = array();
    foreach ($mappings as $mapping) {
      $sources[] = check_plain($mapping['source']);
      if ($mapping['unique']) {
        $uniques[] = check_plain($mapping['source']);
      }
    }

    return $form;
  }

  /**
   * Define default configuration.
   */
  public function configDefaults() {
    return array();
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state) {
    return array();
  }

  public function getTemplate() {
    print serialize(array(array('item1' => 1, 'item2' => 2)));
    return;
  }
}
