<?php
/**
 * @file
 * Show correct allowed values when dealing with project arguments.
 */

/**
 * Views filter handler to provide a selection dropdown for a dereference_list,
 * tied to a project argument.
 */
class project_issue_handler_filter_dereference_list extends views_handler_filter_field_list {
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
        '#value' => t('You can only select options if you expose this filter and have a project argument'),
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
      if ($project) {
        $field = field_info_field($this->definition['field_name']);
        $items = field_get_items('node', $project, $field['settings']['dereferenced_field']);
        foreach ($items as $item) {
          $this->deref_values[$item['value']] = $item['safe_value'];
        }
        $this->value_options = $this->deref_values;
      }
      else {
        $this->value_options = array();
      }
    }
    else {
      $this->value_options = array();
    }
  }

}

