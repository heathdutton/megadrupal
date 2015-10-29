<?php

/**
 * @file
 *   Base class for Project release metrics.
 */

class ProjectReleaseMetric extends SamplerMetric {

  public function projectReleaseMetricInitProjects($data_types) {
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

  public function trackObjectIDs() {
    $nids = array();
    // Project nodes with releases.
    $projects = db_query("SELECT DISTINCT(nn.nid) FROM {node} n INNER JOIN {field_data_field_release_project} p ON n.nid = p.entity_id AND p.entity_type = 'node' INNER JOIN {node} nn ON nn.nid = p.field_release_project_target_id WHERE n.type IN (:types)", array(':types' => project_release_release_node_types()));
    foreach ($projects as $project) {
      $nids[] = $project->nid;
    }
    return $nids;
  }
}
