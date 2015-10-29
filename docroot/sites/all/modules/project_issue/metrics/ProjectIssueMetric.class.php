<?php

/**
 * @file
 * Base class for Project issue metrics.
 */

class ProjectIssueMetric extends SamplerMetric {

  public function projectIssueMetricInitProjects($data_types) {
    // Load options.
    $options = $this->currentSample->options;
    if (!empty($options['object_ids'])) {
      $object_ids = $options['object_ids'];
    }
    else {
      $object_ids = $this->trackObjectIDs();
    }
    foreach ($object_ids as $object_id) {
      $this->currentSample->values[$object_id] = $data_types;
    }
  }

  /**
   * Get project node IDs.
   */
  public function trackObjectIDs() {
    return db_query('SELECT nid FROM node WHERE status = 1 AND type IN (:types)', array(':types' => project_project_node_types()))->fetchCol();
  }
}
