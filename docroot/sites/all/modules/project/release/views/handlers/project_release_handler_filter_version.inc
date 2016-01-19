<?php
/**
 * @file
 * Show correct allowed values when dealing with project arguments.
 */

/**
 * Views filter handler to show the correct set of options when using the
 * version filter.
 */
class project_release_handler_filter_version extends views_handler_filter_field_list {
  var $is_exposed = FALSE;

  var $deref_values = NULL;

  function has_extra_options() {
    return TRUE;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['project_argument'] = array('default' => NULL);

    return $options;
  }

  function extra_options_form(&$form, &$form_state) {

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

  function value_form(&$form, &$form_state) {
    if (!empty($form_state['exposed'])) {
      // Set a flag so get_value_options() knows to return real values.
      $this->is_exposed = TRUE;
      // Due to some unknown bug in views, it appears as though this filter
      // always acts as if it's a "force single" filter, even if it's not.
      // Therefore, it's (sometimes?) setting its own value to to 'All', when
      // GET is empty, which gives an invalid choice when you load the form.
      $this->value = NULL;
    }
    parent::value_form($form, $form_state);
    if (empty($form_state['exposed'])) {
      $form['warning'] = array(
        '#type' => 'markup',
        '#markup' => t('You can only select options if you expose this filter and have a project argument'),
      );
    }
  }

  function get_value_options() {
    if (isset($this->deref_values)) {
      return;
    }
    // We only want to return real options if we're building an exposed filter
    // form and we have a project argument (either nid or uri).  So, we check
    // the flag set in value_form() to see if this is an exposed filter.
    if ($this->is_exposed && !empty($this->view->args)) {
      $arg = $this->view->args[$this->view->argument[$this->options['project_argument']]->position];
      $project = project_load($arg);
      $this->value_options = array();
      if ($project) {
        // Add the enabled compatibility terms.
        $data = taxonomy_get_tree(variable_get('project_release_api_vocabulary', 0), 0, 1, TRUE);
        foreach ($data as $term) {
          if (!empty($term->field_release_recommended[LANGUAGE_NONE][0]['value'])) {
            $this->value_options['__series:' . $term->tid] = t('- @series issues -', array('@series' => $term->name));
          }
        }

        // And then the normal options.
        $this->value_options += drupal_map_assoc(project_release_get_available_versions($project->nid));
      }
      else {
        $this->value_options = array();
      }
    }
    else {
      $this->value_options = array();
    }
  }

  function query() {
    // Walk the value list and replace tid series references with lists of release version strings.
    foreach ($this->value as $key => $value) {
      // A series term was passed, we need to expand it to all the applicable releases.
      if (substr($value, 0, 9) == '__series:') {
        $tid = substr($value, 9);
        if (!$this->view->args) {
          // Skip filter, we can't do an all projects query for all releases in one series.
          return;
        }
        // Grab the project from the arguments.
        $arg = $this->view->args[$this->view->argument[$this->options['project_argument']]->position];
        $project = project_load($arg);
        // Remove the original entry in the value list.
        unset ($this->value[$key]);
        // Append the version list.
        $this->value += project_release_get_available_versions($project->nid, $tid);
      }
    }
    return parent::query();
  }

}

