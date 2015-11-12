<?php

class SearchApiViewsHandlerFilterIssueStatus extends SearchApiViewsHandlerFilterOptions {
  protected function get_value_options() {
    parent::get_value_options();
    $this->value_options = array('Open' => t('- Open issues -')) + $this->value_options;
  }

  public function query() {
    if (isset($this->value['Open'])) {
      unset($this->value['Open']);
      $this->value += drupal_map_assoc(project_issue_open_states());
    }
    elseif (($key = array_search('Open', $this->value)) !== FALSE) {
      unset($this->value[$key]);
      $this->value += drupal_map_assoc(project_issue_open_states());
    }
    parent::query();
  }
}
