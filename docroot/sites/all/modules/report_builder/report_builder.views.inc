<?php

/**
 * @file
 * Views plugin for report display type.
 */

/**
 * Implements hook_views_plugins().
 *
 * Includes the Report Builder views diplay in views.
 */
function report_builder_views_plugins() {
  return array(
    'display' => array(
      'report_builder' => array(
        'title' => t('Report Builder'),
        'help' => t('Expose this view as a Report Builder data source'),
        'handler' => 'ReportBuilderPluginDisplayReport',
        'theme' => 'views_view',
        'admin' => t('Report Builder'),
        'use ajax' => TRUE,
      ),
    ),
  );
}

/*
 * A views plugin that sets up the display type. This allows users to specify a
 * report as report builder-ready.
 */

class ReportBuilderPluginDisplayReport extends views_plugin_display {

  /**
   * Executes the view.
   */
  function execute() {
    return $this->view->render($this->display->id);
  }

  /**
   * Validates the view.
   */
  function validate() {
    $errors = parent::validate();
    $fields = $this->get_handlers('field');
    if (sizeof($fields) > 1) {
      $errors[] = t('Display "@display" uses more than one field.  Only one field per view is allowed for Report Builder.', array('@display' => $this->display->display_title));
    }
    $title = $this->view->get_title();
    if (empty($title)) {
      $errors[] = t('Display "@display" needs a title for identification in report builder.', array('@display' => $this->display->display_title));
    }
    return $errors;
  }

}
