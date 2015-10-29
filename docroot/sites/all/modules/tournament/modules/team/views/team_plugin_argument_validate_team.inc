<?php

/**
 * Validate whether an argument is a valid team.
 *
 * This supports either numeric arguments (TID) or strings (team names) and
 * converts either one into the team's TID.
 */
class team_plugin_argument_validate_team extends views_plugin_argument_validate {
  function option_definition() {
    $options = parent::option_definition();
    $options['type'] = array('default' => 'tid');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Type of team filter value to allow'),
      '#options' => array(
        'tid' => t('Only allow numeric TIDs'),
        'name' => t('Only allow string team names'),
        'either' => t('Allow both numeric TIDs and string team names'),
      ),
      '#default_value' => $this->options['type'],
    );
  }

  function convert_options(&$options) {
    if (!isset($options['type']) && isset($this->argument->options['validate_team_argument_type'])) {
      $options['type'] = $this->argument->options['validate_team_argument_type'];
    }
  }

  function validate_argument($argument) {
    $type = $this->options['type'];
    // is_numeric() can return false positives, so we ensure it's an integer.
    // However, is_integer() will always fail, since $argument is a string.
    if (is_numeric($argument) && $argument == (int)$argument) {
      if ($type == 'tid' || $type == 'either') {
        $where = 'tid = :argument';
      }
    }
    else {
      if ($type == 'name' || $type == 'either') {
        $where = "name = :argument";
      }
    }

    // If we don't have a WHERE clause, the argument is invalid.
    if (empty($where)) {
      return FALSE;
    }

    $query = "SELECT tid, name FROM {teams} WHERE $where";
    $team = db_query($query, array(':argument' => $argument))->fetchObject();

    if (empty($team)) {
      // Team not found.
      return FALSE;
    }

    $this->argument->argument = $team->tid;
    $this->argument->validated_title = check_plain($team->name);
    return TRUE;
  }

  function process_summary_arguments(&$args) {
    // If the validation says the input is a team name, we should reverse the
    // argument so it works for example for generation summary urls.
    $tids_arg_keys = array_flip($args);
    if ($this->options['type'] == 'name') {
      $teams = team_load_multiple($args);
      foreach ($teams as $tid => $team) {
        $args[$tids_arg_keys[$tid]] = $team->name;
      }
    }
  }
}
