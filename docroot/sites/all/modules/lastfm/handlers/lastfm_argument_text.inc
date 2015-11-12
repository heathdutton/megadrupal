<?php
// $Id;

/**
 * @file
 *   Last.fm argument handler for arguments generic text arguments.
 */

/**
 * Argument handler for generic text information. 
 *
 * Implement the argument setting for last.fm arguments, and unset a bunch
 * of form options that don't really apply to us.
 */
class lastfm_argument_text extends views_handler_argument {

  function query() {
    // Add the argument value and method. These will be included in the final
    // last.fm API call in the query string.
    if (isset($this->argument, $this->definition['lastfm'])) {
      $this->query->args[$this->definition['lastfm']['arg']] = $this->argument;
      $this->query->args['method'] = $this->definition['lastfm']['method'];
    }
  }

  /**
   * Provide a list of default behaviors for this argument if the argument
   * is not present.
   *
   * Override this method to provide additional (or fewer) default behaviors.
   */
  function default_actions($which = NULL) {
    // This is just removing a bunch of actions which don't apply because
    // the last.fm API doesn't implement them.
    $defaults = array(
      'ignore' => array(
        'title' => t('Display all values'),
        'method' => 'default_ignore',
        'breadcrumb' => TRUE, // generate a breadcrumb to here
      ),
      'not found' => array(
        'title' => t('Hide view / Page not found (404)'),
        'method' => 'default_not_found',
        'hard fail' => TRUE, // This is a hard fail condition
      ),
      'empty' => array(
        'title' => t('Display empty text'),
        'method' => 'default_empty',
        'breadcrumb' => TRUE, // generate a breadcrumb to here
      ),
      'default' => array(
        'title' => t('Provide default argument'),
        'method' => 'default_default',
        'form method' => 'default_argument_form',
        'has default argument' => TRUE,
        'default only' => TRUE, // this can only be used for missing argument, not validation failure
      ),
    );

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    unset($options['wildcard']);
    unset($options['substitution']);
    return $options;
  }

  function options_form(&$form, $form_state) {
    // We can't do wildcards at all, so just remove those options.
    parent::options_form($form, $form_state);
    unset($form['wildcard']);
    unset($form['wildcard_substitution']);
  }

}
