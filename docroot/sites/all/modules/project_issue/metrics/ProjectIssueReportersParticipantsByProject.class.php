<?php

/**
 * @file
 * Class for reporters_participants_per_project metric.
 */

class ProjectIssueReportersParticipantsByProject extends ProjectIssueMetric {

  public function computeSample() {
    // Initialize the projects array.
    $this->projectIssueMetricInitProjects(array(
      'reporters' => 0,
      'participants' => 0,
    ));

    // Load options.
    $sample = $this->currentSample;

    $query = db_select('node', 'n', array('target' => 'slave'))
      ->condition('n.created', $sample->sample_startstamp, '>=')
      ->condition('n.created', $sample->sample_endstamp, '<')
      ->condition('n.type', project_issue_issue_node_types());

    $query->innerJoin('field_data_field_project', 'p', "n.nid = p.entity_id AND p.entity_type = 'node'");

    $query->addField('p', 'field_project_target_id', 'pid');
    $query->addField('n', 'uid', 'uid');
    $query->distinct();

    // Restrict to only the passed projects.
    if (!empty($sample->options['object_ids'])) {
      $query->condition('p.field_project_target_id', $sample->options['object_ids']);
    }

    // Pull the count of unique reporters per project.
    $projects = $query->execute();

    $project_participants = array();
    foreach ($projects as $project) {
      // Increment the number of reporters for the project, and also store
      // them as a participant.
      if (isset($this->currentSample->values[$project->pid]['reporters'])) {
        $this->currentSample->values[$project->pid]['reporters'] += 1;
      }
      else {
        $this->currentSample->values[$project->pid]['reporters'] = 1;
      }
      $this->currentSample->values[$project->pid]['participants'] = 0;
      $project_participants[$project->pid][$project->uid] = TRUE;
    }

    // Pull the count of unique participants per project.
    $query = db_select('comment', 'c', array('target' => 'slave'))
      ->condition('c.created', $sample->sample_startstamp, '>=')
      ->condition('c.created', $sample->sample_endstamp, '<');

    $query->innerJoin('field_data_field_project', 'p', "c.nid = p.entity_id AND p.entity_type = 'node'");

    $query->addField('p', 'field_project_target_id', 'pid');
    $query->addField('c', 'uid');
    $query->distinct();

    $projects = $query->execute();

    // Add in participants from comments.  This will overwrite the reporters if
    // they are the same uid, thus avoiding double counting.
    foreach ($projects as $project) {
      $project_participants[$project->pid][$project->uid] = TRUE;
    }

    // Store the total participants for each project.
    foreach ($project_participants as $pid => $participants) {
      if (!isset($this->currentSample->values[$pid]['reporters'])) {
        $this->currentSample->values[$pid]['reporters'] = 0;
      }
      $this->currentSample->values[$pid]['participants'] = count($participants);
    }

    unset($project_participants);
  }
}
