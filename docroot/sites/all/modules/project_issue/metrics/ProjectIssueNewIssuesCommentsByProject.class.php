<?php

/**
 * @file
 * Class for new_issues_comments_by_project metric.
 */

class  ProjectIssueNewIssuesCommentsByProject extends ProjectIssueMetric {

  public function computeSample() {
    // Initialize the projects array.
    $this->projectIssueMetricInitProjects(array(
      'new_issues' => 0,
      'new_comments' => 0,
      'new_total' => 0,
    ));

    // Load options.
    $sample = $this->currentSample;

    $query = db_select('node', 'n', array('target' => 'slave'))
      ->condition('n.created', $sample->sample_startstamp, '>=')
      ->condition('n.created', $sample->sample_endstamp, '<')
      ->condition('n.type', project_issue_issue_node_types());

    $query->innerJoin('field_data_field_project', 'p', "n.nid = p.entity_id AND p.entity_type = 'node'");

    $query->addField('p', 'field_project_target_id', 'pid');
    $query->addExpression('COUNT(n.nid)', 'count');
    $query->groupBy('p.field_project_target_id');

    // Restrict to only the passed projects.
    if (!empty($sample->options['object_ids'])) {
      $query->condition('p.field_project_target_id', $sample->options['object_ids']);
    }

    // Pull the count of new issues per project.
    $projects = $query->execute();

    foreach ($projects as $project) {
      $this->currentSample->values[$project->pid]['new_issues'] = (int)$project->count;
      $this->currentSample->values[$project->pid]['new_comments'] = 0;
      $this->currentSample->values[$project->pid]['new_total'] = (int)$project->count;
    }

    // Pull the count of new issue comments per project.
    $query = db_select('comment', 'c', array('target' => 'slave'))
      ->condition('c.created', $sample->sample_startstamp, '>=')
      ->condition('c.created', $sample->sample_endstamp, '<');

    $query->innerJoin('field_data_field_project', 'p', "c.nid = p.entity_id AND p.entity_type = 'node'");

    $query->addField('p', 'field_project_target_id', 'pid');
    $query->addExpression('COUNT(c.cid)', 'count');
    $query->groupBy('p.field_project_target_id');

    // Restrict to only the passed projects.
    if (!empty($sample->options['object_ids'])) {
      $query->condition('p.field_project_target_id', $sample->options['object_ids']);
    }

    $projects = $query->execute();

    foreach ($projects as $project) {
      if (!isset($this->currentSample->values[$project->pid]['new_issues'])) {
        $this->currentSample->values[$project->pid]['new_issues'] = 0;
      }
      $this->currentSample->values[$project->pid]['new_comments'] = (int)$project->count;
      if (isset($this->currentSample->values[$project->pid]['new_total'])) {
        $this->currentSample->values[$project->pid]['new_total'] += (int)$project->count;
      }
      else {
        $this->currentSample->values[$project->pid]['new_total'] = (int)$project->count;
      }
    }
  }
}
