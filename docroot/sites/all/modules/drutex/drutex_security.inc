<?php

/**
 * @file
 * Provide security features for DruTeX.
 *
 * This module covers functions to restrict the set of allowed
 * LaTeX commands and environments.
 */

/**
 * Implements hook_info().
 */
function drutex_security_info($filter) {
  return (object) array(
    'title' => t('Security restrictions'),
    'description' => t('Restricts the set of allowed LaTeX commands.'),
    'toggle' => TRUE,
    'weight' => 20,
  );
}

/**
 * Implementation of subhook_filter_tips().
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drutex_security_filter_tips($delta, $filter, $long = FALSE) {

  if ($filter->settings['drutex_submodules']['security']){
    $allowed_commands = $filter->settings['drutex_security']['drutex_allowedcommands'];
    $allowed_environments = $filter->settings['drutex_security']['drutex_allowedenvironments'];
    $text = t('Allowed commands include: %cmd', array('%cmd' => $allowed_commands));
    $text .= '<br />' . t('Allowed environments include: %env', array('%env' => $allowed_environments));
  }
  else {
    $text = '';
  }
  return $text;
}

/**
 * Implements hook_defaults().
 */
function drutex_security_defaults() {
  $D['drutex_security_active'] = FALSE;
  $D['drutex_allowedcommands'] = '\atop \binom \cdot \cfrac \choose \frac \int \ln \over \sum \to';
  $D['drutex_allowedenvironments'] = 'align array equation equations gather matrix split';

  return $D;
}

/**
 * Checks if $text only includes allowed commands/environments.
 */
function drutex_security($text, $filter) {
  static $allowed_commands = array();

  if (empty($allowed_commands)) {
    $allowed_commands = explode(' ', $filter->settings['drutex_security']['drutex_allowedcommands']);
    $allowed_environments = explode(' ', $filter->settings['drutex_security']['drutex_allowedenvironments']);

    foreach ($allowed_environments as $env) {
      $allowed_commands[$filter->format] = array_merge($allowed_commands,
        array("\\begin\{$env}", "\\begin\{$env*}", "\\end\{$env}", "\\end\{$env*}"));
    }
  }

  $matches = array();
  preg_match_all('/(\\\\begin\{[a-zA-Z*]+\})|(\\\\end\{[a-zA-Z*]+\})|(\\\\[a-zA-Z]+)/s', $text, $matches);

  $commands = $matches[0];

  foreach ($commands as $command) {
    if (array_search($command, $allowed_commands[$filter->format]) === FALSE) {
      watchdog('DruTeX', "Unallowed command (by security submodule): %cmd", array('%cmd' => $command), WATCHDOG_WARNING);
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Implements hook_filter_settings().
 */
function drutex_security_filter_settings($filter) {
  $form["drutex_allowedcommands"] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed commands'),
    '#rows' => 4,
    '#cols' => 50,
    '#default_value' => $filter->settings['drutex_security']['drutex_allowedcommands'],
    '#description' => t('Commands are seperated by single space.'),
  );

  $form["drutex_allowedenvironments"] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed environments'),
    '#rows' => 4,
    '#cols' => 50,
    '#default_value' => $filter->settings['drutex_security']['drutex_allowedenvironments'],
    '#description' => t('Environments are seperated by single space.'),
  );

  return $form;
}

