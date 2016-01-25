<?php

/**
 * @file
 * Class for nodes_vs_users_vs_comments metric.
 */

class  SamplerExampleMetricNodesVsUsersVsComments extends SamplerMetric {

  // Since this metric only has one object ID, provide that here as a constant.
  const OBJECT_ID = 1;

  public function computeSample() {

    // Load options.
    $sample = $this->currentSample;
    $query_options = array('target' => 'slave');

    // Compute nodes, users, and comments created during the period.
    $nodes = db_query('SELECT COUNT(nid) AS count FROM {node} WHERE created >= :startstamp AND created < :endstamp', array(':startstamp' => $sample->sample_startstamp, ':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();
    $users = db_query('SELECT COUNT(uid) AS count FROM {users} WHERE created >= :startstamp AND created < :endstamp', array(':startstamp' => $sample->sample_startstamp, ':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();
    $comments = db_query('SELECT COUNT(cid) AS count FROM {comment} WHERE created >= :startstamp AND created < :endstamp', array(':startstamp' => $sample->sample_startstamp, ':endstamp' => $sample->sample_endstamp), $query_options)->fetchField();

    $this->currentSample->values = array(
      self::OBJECT_ID => array(
        'nodes' => $nodes,
        'users' => $users,
        'comments' => $comments,
      ),
    );
  }

  public function trackObjectIDs() {
    // We're tracking these metric across all nodes and users, which only ever
    // has one object_id, so we just hard code it.
    $object_ids = array(self::OBJECT_ID);
    return $object_ids;
  }
}

