<?php

/**
 * Filter issues by the project they are associated with.
 *
 * Ultimately, this filters on {project_issues}.pid using project nids.
 * However, in the UI, you can either have a select box of project titles or
 * an autocomplete textfield for a comma-separated list of project titles and
 * those all get mapped back to project nids under the covers.  Also,
 * regardless of the widget, there's a setting to control the "source" where
 * the list of choices should come from, either all issue-enabled projects on
 * the side, all projects owned by a given user, or all projects from issues
 * that a user has created or commented on.
 *
 * This was based heavily on views_handler_filter_term_node_tid from views.
 */
class ProjectIssueHandlerFilterSearchApiIssueProject extends SearchApiViewsHandlerFilter {
  // TODO This is is a copy of
  // project_issue_handler_filter_searchapi_issue_project.inc changed to work
  // with SearchAPI. That file should be deleted if non-SearchAPI views become
  // unsupported. Or move shared code to a trait when PHP 5.4 is required.
  function has_extra_options() { return TRUE; }

  function get_value_options() { /* don't overwrite the value options */ }

  function operator_options() {
    return array(
      'in' => t('Is one of'),
      'not in' => t('Is not one of'),
    );
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['widget'] = array('default' => 'textfield');
    $options['project_source'] = array('default' => 'all');
    $options['project_uid_argument'] = array('default' => '');

    return $options;
  }

  function extra_options_form(&$form, &$form_state) {
    $form['widget'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array('select' => t('Select'), 'textfield' => t('Autocomplete')),
      '#default_value' => $this->options['widget'],
    );

    // Figure out what arguments this view has defined.
    $arguments[0] = t('<None>');
    $view_args = $this->view->display_handler->get_handlers('argument');
    if (!empty($view_args)) {
      foreach ($view_args as $id => $argument) {
        $arguments[$id] = $argument->ui_name();
      }
    }

    $source_options['all'] = t('All issue-enabled projects');
    if (count($arguments) > 1) {
      $source_options['owner'] = t('Projects owned by a given user');
      $source_options['participant'] = t('Projects from all issues a given user participated in');
    }
    else {
      $description = t('If you add a user argument to this view, you will also be able to restrict the choices here to projects owned by a given user, or projects from issues that a user has participated in.');
    }
    $form['project_source'] = array(
      '#type' => 'radios',
      '#title' => t('Project choices to provide'),
      '#options' => $source_options,
      '#default_value' => $this->options['project_source'],
    );
    if (!empty($description)) {
      $form['project_source']['#description'] = $description;
    }

    if (count($arguments) > 1) {
      $form['project_uid_argument'] = array(
        '#type' => 'select',
        '#title' => t('UID argument for the user to restrict projects to'),
        '#options' => $arguments,
        '#default_value' => $this->options['project_uid_argument'],
        '#dependency' => array('radio:options[project_source]' => array('owner', 'participant')),
      );
    }
  }

  function extra_options_validate($form, &$form_state) {
    if ($form_state['values']['options']['project_source'] != 'all' && empty($form_state['values']['options']['project_uid_argument'])) {
      form_error(t('If you do not specify the argument for the user, you must use the "All issue-enabled projects" option for "Project choices to provide".'));
    }
  }

  function value_form(&$form, &$form_state) {
    // If the current project source depends on a UID, figure it out.
    $uid = NULL;
    if (!empty($form_state['exposed']) && $this->options['project_source'] != 'all') {
      $arg_name = $this->options['project_uid_argument'];
      if (empty($this->view->argument[$arg_name])) {
        drupal_set_message(t('Invalid argument to restrict projects by user: falling back to all projects.'), 'error');
        $this->options['project_source'] = 'all';
      }
      else {
        $uid = $this->view->argument[$arg_name]->get_value();
      }
    }

    if ($this->options['widget'] == 'textfield') {
      // If there are already values, we have them as project nids, so map
      // those back into titles for the textfield.
      $default = '';
      if ($this->value) {
        $result = db_query('SELECT n.nid, n.title FROM {node} n WHERE n.nid IN (:nids)', array(':nids' => $this->value));
        foreach ($result as $project) {
          if ($default) {
            $default .= ', ';
          }
          $default .= $project->title;
        }
      }

      switch ($this->options['project_source']) {
        case 'all':
          $autocomplete_path = 'project/autocomplete/issue/project';
          break;

        case 'owner':
          $autocomplete_path = "project/autocomplete/issue/owner/$uid";
          break;

        case 'participant':
          $autocomplete_path = "project/autocomplete/issue/user/$uid";
          break;
      }

      $form['value'] = array(
        '#title' => t('Select projects'),
        '#type' => 'textfield',
        '#autocomplete_path' => $autocomplete_path,
        '#default_value' => $default,
        '#size' => 30,
        '#maxlength' => NULL,
        '#description' => t('Enter a comma separated list of projects.'),
      );

    }
    else { // widget == select
      $options = project_issue_get_projects($this->options['project_source'], $uid);

      $default_value = (array) $this->value;
      if (!empty($form_state['exposed'])) {
        $identifier = $this->options['expose']['identifier'];
        if (empty($this->options['expose']['single']) && empty($this->options['expose']['required'])) {
          $default_value = array();
        }
        if (!empty($this->options['expose']['single'])) {
          if (empty($this->options['expose']['required']) && (empty($default_value) || !array_filter($default_value))) {
            $default_value = 'All';
          }
          elseif (empty($default_value) || !array_filter($default_value)) {
            $keys = array_keys($options);
            $default_value = array_shift($keys);
          }
          else {
            $copy = $default_value;
            $default_value = array_shift($copy);
          }
        }
        if (!isset($form_state['input'][$identifier])) {
          $form_state['input'][$identifier] = $default_value;
        }
      }

      $form['value'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => t('Select project'),
        '#options' => $options,
        '#size' => min(9, count($options)),
        '#default_value' => $default_value,
      );
    }
  }

  function value_validate($form, &$form_state) {
    // We only validate if they've chosen the text field style.
    if ($this->options['widget'] != 'textfield') {
      return;
    }

    $values = drupal_explode_tags($form_state['values']['options']['value']);
    $nids = $this->validate_project_strings($form['value'], $values);

    if ($nids) {
      $form_state['values']['options']['value'] = $nids;
    }
  }

  function accept_exposed_input($input) {
    // If filter is not exposed, do not change the behavior
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // If it's not required and there's no value don't bother filtering.
    if (empty($this->options['expose']['required']) && empty($this->validated_exposed_input)) {
      return FALSE;
    }

    $rc = parent::accept_exposed_input($input);
    if ($rc) {
      // If we have previously validated input, override.
      if (isset($this->validated_exposed_input)) {
        $this->value = $this->validated_exposed_input;
      }
    }

    return $rc;
  }

  function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    $identifier = $this->options['expose']['identifier'];

    // We only validate if they've chosen the text field style.
    if ($this->options['widget'] != 'textfield') {
      if ($form_state['values'][$identifier] != 'All')  {
        $this->validated_exposed_input = (array) $form_state['values'][$identifier];
      }
      return;
    }

    if (empty($this->options['expose']['identifier'])) {
      return;
    }

    $values = drupal_explode_tags($form_state['values'][$identifier]);

    $nids = $this->validate_project_strings($form[$identifier], $values);
    if ($nids) {
      $this->validated_exposed_input = $nids;
    }
  }

  /**
   * Validate the user string. Since this can come from either the form
   * or the exposed filter, this is abstracted out a bit so it can
   * handle the multiple input sources.
   */
  function validate_project_strings(&$form, $values) {
    if (empty($values)) {
      return array();
    }

    $nids = array();
    $results = array();

    foreach ($values as $value) {
      $query = new EntityFieldQuery();
      $query
        ->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', project_project_node_types())
        ->fieldCondition('field_project_has_issue_queue', 'value', 1)
        ->propertyCondition('title', $value);

      $result = $query->execute();
      if (empty($result['node'])) {
        $missing[] = $value;
      }
      else {
        foreach ($result['node'] as $entity_id => $entity) {
          $nids[] = $entity_id;
        }
      }
    }

    if (!empty($missing)) {
      form_error($form, format_plural(count($missing), 'Unable to find project: @projects', 'Unable to find projects: @projects', array('@projects' => implode(', ', $missing))));
    }

    return $nids;
  }

  /**
   * Add this filter to the query.
   */
  public function query() {
    $this->query->condition($this->real_field, $this->value, $this->operator, $this->options['group']);
  }

  function value_submit($form, &$form_state) {
    // prevent array_filter from messing up our arrays in parent submit.
  }

  function admin_summary() {
    switch ($this->options['project_source']) {
      case 'all':
        return t('All projects with issue tracking');

      case 'owner':
        return t('Projects a user owns');

      case 'participant':
        return t('Projects from issues a user participated in');
    }
  }
}
