<?php

class ProjectIssueHandlerFilterSearchApiDereferenceList extends SearchApiViewsHandlerFilterOptions {
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

  protected function get_value_options() {
    $this->value_options = array();
    // We only want to return real options if we're building an exposed filter
    // form and we have a project argument (either nid or uri).  So, we check
    // the flag set in value_form() to see if this is an exposed filter.
    if (!empty($this->view->args)) {
      $project = project_load($this->view->argument[$this->options['project_argument']]->get_value());
      if ($project) {
        $field = field_info_field($this->options['field']);
        foreach (field_get_items('node', $project, $field['settings']['dereferenced_field']) as $item) {
          $this->value_options[$item['safe_value']] = $item['safe_value'];
        }
      }
    }
  }
}
