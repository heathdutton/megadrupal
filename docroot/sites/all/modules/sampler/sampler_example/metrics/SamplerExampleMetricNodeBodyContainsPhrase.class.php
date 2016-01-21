<?php

/**
 * @file
 * Class for node_body_contains_phrase metric.
 */

class  SamplerExampleMetricNodeBodyContainsPhrase extends SamplerMetric {

  // Since an object type of 'all nodes' only has one object ID, provide that
  // here as a constant.
  const ALL_NODES_OBJECT_ID = 1;

  public function computeSample() {

    // Load options.
    $sample = $this->currentSample;
    $query_options = array('target' => 'slave');

    // Check for a custom phrase.
    if (isset($sample->options['node_body_search_phrase'])) {
      $phrase = '%' . $sample->options['node_body_search_phrase'] . '%';
    }
    // Fall back to a default so we can search on something.
    else {
      $phrase = '%module%';
    }

    // The 'all nodes' object type only ever has one object ID, so we don't need
    // to worry about anything passed in object_ids -- just return the single metric
    // value asked for.
    // TODO: Shouldn't have to specify $select each time like this, the
    // methods are supposed to be chainable, bug in core?
    $select = db_select('node', 'n', $query_options);
    $select->innerJoin('field_revision_body', 'frb', 'n.vid = frb.revision_id');
    $select->addExpression('COUNT(n.nid)', 'count');
    $select->condition('n.created', $sample->sample_startstamp, '>');
    $select->condition('n.created', $sample->sample_endstamp, '<');
    $select->condition('frb.body_value', $phrase, 'LIKE');
    $value = $select->execute()->fetchField();

    $this->currentSample->values = array(
      self::ALL_NODES_OBJECT_ID => array(
        'nodes' => $value,
      ),
    );
  }

  public function trackObjectIDs() {

    // We're tracking these metric across all nodes, which only ever has one
    // object_id, so we just hard code it.
    $object_ids = array(self::ALL_NODES_OBJECT_ID);
    return $object_ids;
  }
}

