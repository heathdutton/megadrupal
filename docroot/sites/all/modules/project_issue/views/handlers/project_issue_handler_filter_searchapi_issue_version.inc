<?php

/**
 * Filter issues based on their version.
 *
 * This filter only returns values to select if it's exposed and the view has
 * a project argument (either nid or uri), and that argument is first.
 */
class ProjectIssueHandlerFilterSearchApiVersion extends SearchApiViewsHandlerFilterOptions {
  private $api_version_strings = array();

  public function has_extra_options() {
    return TRUE;
  }

  public function option_definition() {
    $options = parent::option_definition();

    $options['project_argument'] = array('default' => NULL);

    return $options;
  }

  public function extra_options_form(&$form, &$form_state) {
    // Figure out what arguments this view has defined.
    $arguments[0] = t('<None>');
    $view_args = $this->view->display_handler->get_handlers('argument');
    if (!empty($view_args)) {
      foreach ($view_args as $id => $argument) {
        $arguments[$id] = $argument->ui_name();
      }
    }

    $form['project_argument'] = array(
      '#type' => 'select',
      '#title' => t('Project argument used to determine options.'),
      '#options' => $arguments,
      '#default_value' => $this->options['project_argument'],
    );
  }

  protected function get_value_options() {
    $this->value_options = array();
    // We only want to return real options if we're building an exposed filter
    // form and we have a project argument (either nid or uri).  So, we check
    // the flag set in value_form() to see if this is an exposed filter.
    if (!empty($this->view->args)) {
      $versions = array();
      $api_tids = array();
      $releases = project_release_query_releases($this->view->argument[$this->options['project_argument']]->get_value(), NULL, NULL, FALSE, TRUE);
      foreach ($releases as $release) {
        $versions[] = $release->field_release_version[LANGUAGE_NONE][0]['value'];
        if ($api_tid = project_release_get_release_api_tid($release)) {
          $api_tids[$api_tid][] = $release->field_release_version[LANGUAGE_NONE][0]['value'];
        }
      }
      uasort($versions, '_project_release_version_compare');

      $series = array();
      foreach (taxonomy_term_load_multiple(array_keys($api_tids)) as $term) {
        $series[$term->name] = t('- @series issues -', array('@series' => $term->name));
        $this->api_version_strings[$term->name] = $api_tids[$term->tid];
      }

      $this->value_options = $series + drupal_map_assoc($versions);
    }
  }

  public function query() {
    // Expand series versions.
    foreach ($this->value as $key => $value) {
      if (isset($this->api_version_strings[$value])) {
        unset($this->value[$key]);
        $this->value += $this->api_version_strings[$value];
      }
    }
    parent::query();
  }
}
