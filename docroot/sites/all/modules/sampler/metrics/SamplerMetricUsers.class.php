<?php

/**
 * @file
 * Class for users metric.
 */

class SamplerMetricUsers extends SamplerSingleObjectMetric {

  public function computeSample() {

    // Load options.
    $sample = $this->currentSample;
    $query_options = array('target' => 'slave');

    // Users created during the sample period.
    $period_users = db_query('SELECT COUNT(uid) AS count FROM {users} WHERE created >= :startstamp AND created < :endstamp', array(':startstamp' => $sample->sample_startstamp, ':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();
    // Total nodes created through the end of the sample period.
    $total_users = db_query('SELECT COUNT(uid) AS count FROM {users} WHERE created < :endstamp', array(':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();

    $this->currentSample->values = array(
      parent::OBJECT_ID => array(
        'period_users' => intval($period_users),
        'total_users' => intval($total_users),
      ),
    );
  }
}

