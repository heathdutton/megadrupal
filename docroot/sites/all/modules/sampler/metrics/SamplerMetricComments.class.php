<?php

/**
 * @file
 * Class for comments metric.
 */

class SamplerMetricComments extends SamplerSingleObjectMetric {

  public function computeSample() {

    // Load options.
    $sample = $this->currentSample;
    $query_options = array('target' => 'slave');

    // Comments created during the sample period.
    $period_comments = db_query('SELECT COUNT(cid) AS count FROM {comment} WHERE created >= :startstamp AND created < :endstamp', array(':startstamp' => $sample->sample_startstamp, ':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();
    // Total comments created through the end of the sample period.
    $total_comments = db_query('SELECT COUNT(cid) AS count FROM {comment} WHERE created < :endstamp', array(':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();

    $this->currentSample->values = array(
      parent::OBJECT_ID => array(
        'period_comments' => intval($period_comments),
        'total_comments' => intval($total_comments),
      ),
    );
  }
}

