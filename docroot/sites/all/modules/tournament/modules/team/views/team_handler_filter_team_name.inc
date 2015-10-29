<?php

/**
 * Filter handler for team names.
 *
 * @ingroup views_filter_handlers
 */
class team_handler_filter_team_name extends views_handler_filter_in_operator {
  var $always_multiple = TRUE;

  function value_form(&$form, &$form_state) {
    $values = array();
    if ($this->value) {
      $result = db_select('teams')
        ->fields('teams')
        ->condition('tid', $this->value)
        ->execute();

      foreach ($result as $record) {
        $values[] = $record->name;
      }
    }

    sort($values);
    $default_value = implode(', ', $values);
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Team names'),
      '#default_value' => $default_value,
      '#autocomplete_path' => 'team/autocomplete',
    );

    if (!empty($form_state['exposed']) && !isset($form_state['input'][$this->options['expose']['identifier']])) {
      $form_state['input'][$this->options['expose']['identifier']] = $default_value;
    }
  }

  function value_validate($form, &$form_state) {
    $values = drupal_explode_tags($form_state['values']['options']['value']);
    $tids = $this->validate_team_strings($form['value'], $values);

    if ($tids) {
      $form_state['values']['options']['value'] = $tids;
    }
  }

  function accept_exposed_input($input) {
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

    if (empty($this->options['expose']['identifier'])) {
      return;
    }

    $identifier = $this->options['expose']['identifier'];
    $values = drupal_explode_tags($form_state['values'][$identifier]);

    $tids = $this->validate_team_strings($form[$identifier], $values);

    if ($tids) {
      $this->validated_exposed_input = $tids;
    }
  }

  /**
   * Validate the team string. Since this can come from either the form
   * or the exposed filter, this is abstracted out a bit so it can
   * handle the multiple input sources.
   */
  function validate_team_strings(&$form, $values) {
    $tids = array();
    $placeholders = array();
    $args = array();
    $results = array();
    foreach ($values as $value) {
      $missing[strtolower($value)] = TRUE;
      $args[] = $value;
      $placeholders[] = "'%s'";
    }

    if (!$args) {
      return $tids;
    }

    $result = db_query("SELECT * FROM {teams} WHERE name IN (:names)", array(':names' => $args));
    foreach ($result as $record) {
      unset($missing[strtolower($record->name)]);
      $tids[] = $record->tid;
    }

    if ($missing) {
      form_error($form, format_plural(count($missing), 'Unable to find team: !teams', 'Unable to find teams: !teams', array('!teams' => implode(', ', array_keys($missing)))));
    }

    return $tids;
  }

  function value_submit($form, &$form_state) {
    // prevent array filter from removing our anonymous user.
  }

  // Override to do nothing.
  function get_value_options() { }

  function admin_summary() {
    // set up $this->value_options for the parent summary
    $this->value_options = array();

    if ($this->value) {
      $result = db_query("SELECT * FROM {teams} t WHERE tid IN (:tids)", array(':tids' => $this->value));

      foreach ($result as $record) {
        $this->value_options[$record->tid] = $record->name;
      }
    }

    return parent::admin_summary();
  }
}
