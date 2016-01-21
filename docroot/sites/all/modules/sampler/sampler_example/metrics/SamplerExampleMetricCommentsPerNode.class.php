<?php

/**
 * @file
 * Class for comments_per_node metric.
 */

class SamplerExampleMetricCommentsPerNode extends SamplerMetric {

  public function computeSample() {

    // Load options.
    $sample = $this->currentSample;
    $query_options = array('target' => 'slave');

    // Pull the total entries in the comment table, grouped by nid.
    // TODO: Shouldn't have to specify $select each time like this, the
    // methods are supposed to be chainable, bug in core?
    $select = db_select('node', 'n', $query_options);
    $select->leftJoin('comment', 'c', 'n.nid = c.nid');
    $select->fields('n', array('nid'));
    $select->addExpression('COUNT(c.cid)', 'count');
    $select->groupBy('n.nid');

    // Add filters on node created time if they were set.
    if (!empty($sample->options['startstamp'])) {
      $start = is_numeric($sample->options['startstamp']) ? $sample->options['startstamp'] : strtotime($sample->options['startstamp']);
      $select->condition('n.created', $start, '>');
    }
    if (!empty($sample->options['endstamp'])) {
      $end = is_numeric($sample->options['endstamp']) ? $sample->options['endstamp'] : strtotime($sample->options['endstamp']);
      $select->condition('n.created', $end, '<');
    }

    // If a node type has been passed, restrict to that type.
    if (!empty($sample->options['node_type'])) {
      $select->condition('n.type', $sample->options['node_type']);
    }

    // Restrict to only the passed nids.
    if (!empty($sample->options['object_ids'])) {
      $select->condition('n.nid', $sample->options['object_ids']);
    }

    $nodes = $select->execute();
    foreach ($nodes as $node) {
      $this->currentSample->values[$node->nid]['comments'] = $node->count ? $node->count : 0;
    }
  }

  public function trackObjectIDs() {

    // Load options -- these are the global options in the sampler object.
    $options = $this->options;

    $object_ids = array();

    // Grab nids.
    $select = db_select('node', 'n', $query_options)
      ->fields('n', array('nid'));

    // If a node type has been passed, restrict to that type.
    if (!empty($options['node_type'])) {
      $select->condition('type', $options['node_type']);
    }

    $nodes = $select->execute();
    foreach ($nodes as $node) {
      $object_ids[] = $node->nid;
    }

    return $object_ids;
  }
}

