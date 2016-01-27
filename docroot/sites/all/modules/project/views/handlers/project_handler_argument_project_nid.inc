<?php

/**
 * Argument handler for Project node IDs.
 *
 * This basically just works like a generic node ID argument handler, but it
 * has a special option for setting the breadcrumb on the view to use the
 * special project-specific breadcrumb logic.
 */
class project_handler_argument_project_nid extends views_handler_argument_node_nid {
  function option_definition() {
    $options = parent::option_definition();
    $options['breadcrumb'] = array('default' => 'none');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['breadcrumb'] = array(
      '#type' => 'radios',
      '#title' => t('Breadcrumb behavior'),
      '#default_value' => $this->options['breadcrumb'],
      '#options' => array(
        'none' => t('No override'),
        'no-project' => t('Breadcrumb for the project section'),
        'project' => t('Breadcrumb including the project itself'),
      ),
    );
  }

  function set_breadcrumb(&$breadcrumb) {
    if (empty($this->options['breadcrumb']) || $this->options['breadcrumb'] == 'none' || empty($this->argument) || !is_numeric($this->argument)) {
      return;
    }
    $project = node_load($this->argument);
    $include_project = $this->options['breadcrumb'] == 'project';
    $breadcrumb = project_project_get_breadcrumb($project, $include_project);
  }
}
