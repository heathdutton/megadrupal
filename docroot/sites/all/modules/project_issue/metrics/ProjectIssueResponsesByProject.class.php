<?php

/**
 * @file
 * Class for responses_by_project metric.
 */

class ProjectIssueResponsesByProject extends ProjectIssueMetric {

  public function computeSample() {
    // Initialize the projects array.
    $this->projectIssueMetricInitProjects(array(
      'response_rate' => 0,
      'response_time' => 0,
    ));

    // Query for issues created in the previosu time period, so there is a full time period for responses.
    $query = db_select('node', 'n', array('target' => 'slave'))
      ->condition('n.created', $this->currentSample->sample_startstamp - ($this->currentSample->sample_endstamp - $this->currentSample->sample_startstamp), '>=')
      ->condition('n.created', $this->currentSample->sample_startstamp, '<')
      ->condition('n.type', project_issue_issue_node_types())
      ->condition('n.status', NODE_PUBLISHED)
      ->groupBy('n.nid');

    $query->innerJoin('field_data_field_project', 'p', "n.nid = p.entity_id AND p.entity_type = 'node'");
    $query->addField('p', 'field_project_target_id');

    // Restrict to only the passed projects.
    if (!empty($this->currentSample->options['object_ids'])) {
      $query->condition('p.field_project_target_id', $this->currentSample->options['object_ids']);
    }

    // Issues not posted by the maintainer themself.
    $maintainers = db_select('project_maintainer', 'pm')
      ->where('pm.nid = p.field_project_target_id')
      ->fields('pm', array('uid'));
    $query->condition('n.uid', $maintainers, 'NOT IN');

    $query->leftJoin('comment', 'c', 'c.nid = n.nid AND c.status = :status AND c.uid <> n.uid AND c.uid <> :robot AND c.created < :end', array(
      ':status' => COMMENT_PUBLISHED,
      ':robot' => variable_get('project_issue_followup_user', 0),
      ':end' => $this->currentSample->sample_endstamp,
    ));

    $query->addExpression('c.cid IS NOT NULL', 'responded');
    $query->addExpression('min(c.created) - n.created', 'time');

    $projects = array();
    $issues = $query->execute();
    foreach ($issues as $issue) {
      if (!isset($projects[$issue->field_project_target_id])) {
        $projects[$issue->field_project_target_id] = array(
          'count' => 0,
          'responded' => 0,
          'time' => 0,
        );
      }
      $projects[$issue->field_project_target_id]['count'] += 1;
      $projects[$issue->field_project_target_id]['responded'] += $issue->responded;
      $projects[$issue->field_project_target_id]['time'] += $issue->time;
    }

    foreach ($projects as $id => $project) {
      $this->currentSample->values[$id]['response_rate'] = round($project['responded'] / $project['count'] * 100);
      $this->currentSample->values[$id]['response_time'] = round($project['time'] / $project['count']);
    }
  }
}
