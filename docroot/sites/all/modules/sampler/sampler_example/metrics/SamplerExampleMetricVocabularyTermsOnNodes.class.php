<?php

/**
 * @file
 * Class for the vocabulary_terms_on_nodes metric.
 */

class SamplerExampleMetricVocabularyTermsOnNodes extends SamplerMetric {

  public function computeSample() {
    if (module_exists('taxonomy')) {
      // Load options.
      $sample = $this->currentSample;
      $query_options = array('target' => 'slave');

      // If a list of vocabulary IDs has been passed, restrict the sample to
      // those in the list.
      $object_ids = $sample->options['object_ids'];

      // The query below doesn't return rows when a vocabulary isn't on any
      // nodes, so fill in starting values of 0 for all relevant vocabularies
      // here so something will always be reported back.
      if (empty($object_ids)) {
        $vids = $this->trackObjectIDs();
      }
      else {
        $vids = $object_ids;
      }
      foreach ($vids as $vid) {
        $this->currentSample->values[$vid]['nodes'] = 0;
      }

      // TODO: Shouldn't have to specify $select each time like this, the
      // methods are supposed to be chainable, bug in core?
      $select = db_select('taxonomy_term_data', 'td', $query_options);
      $select->innerJoin('field_data_field_tags', 'fdft', 'td.tid = fdft.field_tags_tid');
      $select->innerJoin('node', 'n', 'fdft.revision_id = n.vid');
      $select->fields('td', array('vid'));
      $select->addExpression('COUNT(fdft.revision_id)', 'count');
      $select->condition('n.created', $sample->sample_startstamp, '>');
      $select->condition('n.created', $sample->sample_endstamp, '<');
      $select->groupBy('td.vid');
      if (!empty($object_ids)) {
        $select->condition('td.vid', $object_ids);
      }
      $result = $select->execute();

      foreach ($result as $data) {
        $this->currentSample->values[$data->vid]['nodes'] = $data->count;
      }
    }
  }

  public function trackObjectIDs() {
    $vids = array();
    if (module_exists('taxonomy')) {
      // Each vocabulary is an object to take samples for.
      $vids = array_keys(taxonomy_get_vocabularies());
    }
    return $vids;
  }
}

