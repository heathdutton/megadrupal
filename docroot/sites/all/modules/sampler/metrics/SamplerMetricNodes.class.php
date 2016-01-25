<?php

/**
 * @file
 * Class for nodes metric.
 */

class SamplerMetricNodes extends SamplerSingleObjectMetric {

  public function computeSample() {

    // Load options.
    $sample = $this->currentSample;
    $query_options = array('target' => 'slave');

    // Nodes created during the sample period.
    $period_nodes = db_query('SELECT COUNT(nid) AS count FROM {node} WHERE created >= :startstamp AND created < :endstamp', array(':startstamp' => $sample->sample_startstamp, ':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();
    // Total nodes created through the end of the sample period.
    $total_nodes = db_query('SELECT COUNT(nid) AS count FROM {node} WHERE created < :endstamp', array(':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();

    $this->currentSample->values = array(
      parent::OBJECT_ID => array(
        'period_nodes' => intval($period_nodes),
        'total_nodes' => intval($total_nodes),
      ),
    );
  }
}

