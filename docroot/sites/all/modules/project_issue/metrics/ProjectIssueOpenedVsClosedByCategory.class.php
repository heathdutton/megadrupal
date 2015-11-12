<?php

/**
 * @file
 * Class for opened_vs_closed_by_category metric.
 */

class ProjectIssueOpenedVsClosedByCategory extends ProjectIssueMetric {

  public function computeSample() {
    // Load options.
    $options = $this->currentSample->options;

    // Initialize the projects array.
    $data_types = $this->newProjectOpenedVsClosed();
    $this->projectIssueMetricInitProjects($data_types);

    // The initial arguments are the 'open' project issue status options.
    $open_states = project_issue_open_states();

    // @todo Until http://drupal.org/node/214347 is resolved, we don't want
    // the 'fixed' status in the list of open statuses, so unset it here.
    $open_states = array_diff($open_states, array(PROJECT_ISSUE_STATE_FIXED));

    // Pull last possible issue nid for the end of the period being measured.
    $max_nid = db_query('SELECT MAX(nid) FROM {node} WHERE created <= :created', array(':created' => $this->currentSample->sample_endstamp), array('target' => 'slave'))->fetchField();

    if (!empty($max_nid)) {
      // Subquery to calculate the timestamp of the applicable revision
      $tsquery = db_select('node_revision', 'sv', array('target' => 'slave'));
      $tsquery->condition('sv.timestamp', $this->currentSample->sample_endstamp, '<=');
      $tsquery->addField('sv', 'nid', 'nid');
      $tsquery->addExpression('MAX(sv.vid)', 'vid');
      $tsquery->groupBy('sv.nid');
      $tsquery->orderBy('NULL');

      $query = db_select($tsquery, 'r', array('target' => 'slave'));
      $query->innerJoin('field_revision_field_project',        'p', "r.nid = p.entity_id AND r.vid = p.revision_id AND p.entity_type = 'node'");
      $query->innerJoin('field_revision_field_issue_status',   's', "r.nid = s.entity_id AND r.vid = s.revision_id AND s.entity_type = 'node'");
      $query->innerJoin('field_revision_field_issue_category', 'c', "r.nid = c.entity_id AND r.vid = c.revision_id AND c.entity_type = 'node'");
      $query->innerJoin('node', 'n', 'n.nid = r.nid');
      $query->condition('n.type', project_issue_issue_node_types());
      $query->condition('n.nid', $max_nid, '<=');
      $query->condition('n.status', NODE_PUBLISHED);
      $query->addField('p', 'field_project_target_id', 'pid');
      $query->addField('s', 'field_issue_status_value', 'sid');
      $query->addField('c', 'field_issue_category_value', 'category');

      // Restrict to only the passed projects.
      if (!empty($options['object_ids'])) {
        $query->condition('p.field_project_target_id', $options['object_ids']);
      }

      $issues = $query->execute();
      // @todo When http://drupal.org/node/115553 lands, this hard-coded list of
      // categories will need to be handled differently.
      $categories = array(1 => 'bug', 3 => 'feature', 2 => 'task', 4 => 'support');
      foreach ($issues as $issue) {
        // Add to the total count for the category in the values array.
        if (isset($categories[$issue->category])) {
          if (!isset($this->currentSample->values[$issue->pid])) {
            $this->currentSample->values[$issue->pid] = array(
              'bug_open' => 0,
              'bug_closed' => 0,
              'feature_open' => 0,
              'feature_closed' => 0,
              'task_open' => 0,
              'task_closed' => 0,
              'support_open' => 0,
              'support_closed' => 0,
            );
          }
          $this->currentSample->values[$issue->pid][$categories[$issue->category] . '_' . (in_array($issue->sid, $open_states) ? 'open' : 'closed')] += 1;
        }
      }
    }

    // Add in total counts across all categories.
    foreach ($this->currentSample->values as $project_id => $values_array) {
      $this->currentSample->values[$project_id]['total_open'] = $values_array['bug_open'] + $values_array['feature_open'] + $values_array['task_open'] + $values_array['support_open'];
      $this->currentSample->values[$project_id]['total_closed'] = $values_array['bug_closed'] + $values_array['feature_closed'] + $values_array['task_closed'] + $values_array['support_closed'];
    }
  }

  /**
   * Builds the starting values for a project for this metric.
   *
   * @return
   *   An associative array, keys are metric values, values are the default
   *   starting count for the metric values.
   */
  protected function newProjectOpenedVsClosed() {
    return array(
      'bug_open' => 0,
      'feature_open' => 0,
      'task_open' => 0,
      'support_open' => 0,
      'bug_closed' => 0,
      'feature_closed' => 0,
      'task_closed' => 0,
      'support_closed' => 0,
      'total_open' => 0,
      'total_closed' => 0,
    );
  }
}
