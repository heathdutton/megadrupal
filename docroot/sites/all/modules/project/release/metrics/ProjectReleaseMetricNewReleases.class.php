<?php

/**
 * @file
 * Class for new_releases metric.
 */

class ProjectReleaseMetricNewReleases extends ProjectReleaseMetric {

  /**
   * Counts new releases made for each project during any given sample period.
   *
   * @return
   *   An associative array keyed by project nid, where the value is an array
   *   containing the number of new releases created for that project in the
   *   current sample period.
   */
  public function computeSample() {

    $sample = $this->currentSample;
    $options = $sample->options;

    // Initialize the projects array.
    $data_types = array(
      'releases' => 0,
    );
    $this->projectReleaseMetricInitProjects($data_types);

    $query = db_select('node', 'n', array('target' => 'slave'));
    $query->innerJoin('field_data_field_release_project', 'p', "n.nid = p.entity_id AND p.entity_type = 'node'");
    // Ignore releases from non-existent projects.
    $query->innerJoin('node', 'nn', 'p.field_release_project_target_id = nn.nid');
    $query->condition('n.created', $sample->sample_startstamp, '>=');
    $query->condition('n.created', $sample->sample_endstamp, '<');
    $query->groupBy('p.field_release_project_target_id');
    $query->addField('p', 'field_release_project_target_id', 'pid');
    $query->addExpression('COUNT(n.nid)', 'releases');

    // Restrict to only the passed project nids.
    if (!empty($options['object_ids'])) {
      $query->condition('p', 'field_release_project_target_id', $options['object_ids']);
    }

    // Pull all release nodes created during the specified time.
    $nodes = $query->execute();
    foreach ($nodes as $node) {
      $this->currentSample->values[$node->pid]['releases'] = (int)$node->releases;
    }
  }
}

