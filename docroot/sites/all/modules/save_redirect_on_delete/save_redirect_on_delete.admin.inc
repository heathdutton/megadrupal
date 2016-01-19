<?php

/**
 * @file
 * This file contains the config form for save_redirect_on_delete module.
 */

/**
 * Config form for redirect patterns after delete.
 */
function save_redirect_on_delete_pattern() {
  $form = array();
  $all_settings = module_invoke_all('pathauto', 'settings');
  foreach ($all_settings as $settings) {
    $groupheader = $settings->groupheader;
    $module = $settings->module;
    $patterndescr = t('Default Pattern After Delete (applied to all with blank pattern below)');
    $patterndefault = '[site:url]';
    $form[$module] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($groupheader),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $variable = 'redirect_' . $module . '_delete_pattern';
    // Text box to accept values for the path after deletion of entity.
    $form[$module]['redirect_' . $module . '_delete_pattern'] = array(
      '#type' => 'textfield',
      '#title' => $patterndescr,
      '#size' => 65,
      '#default_value' => variable_get($variable, $patterndefault),
      '#maxlength' => 1280,
      '#element_validate' => array('token_element_validate'),
      '#after_build' => array('token_element_validate'),
      '#token_types' => array($settings->token_type),
      '#min_tokens' => 1,
    );

    // If the module supports a set of specialized patterns, set them up here.
    if (isset($settings->patternitems)) {
      foreach ($settings->patternitems as $itemname => $itemlabel) {
        $variable = 'redirect_' . $module . '_' . $itemname . '_delete_pattern';
        $form[$module][$variable] = array(
          '#type' => 'textfield',
          '#title' => t('After Delete Path Redirect !itemlabel', array('!itemlabel' => $itemlabel)),
          '#size' => 65,
          '#maxlength' => 1280,
          '#default_value' => variable_get($variable, ''),
          '#element_validate' => array('token_element_validate'),
          '#after_build' => array('token_element_validate'),
          '#token_types' => array($settings->token_type),
        );
      }
    }

    // Display the user documentation of placeholders supported by
    // this module, as a description on the last pattern.
    $form[$module]['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$module]['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array($settings->token_type),
    );
  }
  return system_settings_form($form);
}
