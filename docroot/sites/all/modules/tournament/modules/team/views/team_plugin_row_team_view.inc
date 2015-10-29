<?php

/**
 * @file
 * Contains the team view row plugin.
 */

/**
 * A row plugin which renders a team via team_view.
 *
 * @ingroup views_row_plugins
 */
class team_plugin_row_team_view extends views_plugin_row {
  var $base_table = 'teams';
  var $base_field = 'tid';

  // Store the teams to be used for pre_render.
  var $teams = array();

  function option_definition() {
    $options = parent::option_definition();
    $options['view_mode'] = array('default' => 'full');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options_form_summary_options();
    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $this->options['view_mode'],
     );
    $form['help']['#markup'] = t('Display the team with standard team view.');
  }


  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    $entity_info = entity_get_info('team');
    $options = array();
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }
    if (empty($options)) {
      $options = array(
        'full' => t('Team account')
      );
    }

    return $options;
  }

  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['view_mode']]);
  }

  function pre_render($values) {
    $tids = array();
    foreach ($values as $row) {
      $tids[] = $row->{$this->field_alias};
    }
    $this->teams = team_load_multiple($tids);
  }

  function render($row) {
    $team = $this->teams[$row->{$this->field_alias}];
    $team->view = $this->view;
    $build = team_view($team, $this->options['view_mode']);

    return drupal_render($build);
  }
}
