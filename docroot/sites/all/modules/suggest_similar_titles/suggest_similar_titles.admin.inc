<?php

/**
 * @file
 * File having admin settings code.
 */

/**
 * Callback function for admin settings form.
 */
function suggest_similar_titles_admin_settings() {
  $form = array();
  $form['fieldset_suggest_title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#description' => t('Select content type to enable similar titles suggestion for.'),
  );
  $types = _node_types_build()->types;
  foreach ($types as $index => $value) {
    $form['fieldset_suggest_title'][$value->type . '_suggest_similar_titles'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($value->name),
      '#default_value' => variable_get($value->type . "_suggest_similar_titles", ""),
    );
  }
  $form['fieldset_suggest_title_d_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#description' => t('Select position where you want to show similar titles suggestion list.'),
  );
  $form['fieldset_suggest_title_d_settings']['suggest_similar_titles_settings'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#options' => array('top' => 'Top', 'bottom' => 'Bottom'),
    '#default_value' => variable_get("suggest_similar_titles_settings", "top"),
  );
  $form['fieldset_suggest_title_d_settings']['suggest_similar_titles_noof_nodes'] = array(
    '#type' => 'select',
    '#title' => t('No of nodes'),
    '#options' => array('1' => '1', '5' => '5', '10' => '10', '20' => '20'),
    '#default_value' => variable_get("suggest_similar_titles_noof_nodes", "5"),
    '#description' => t('Select maximum number of nodes to display as similar titles.'),
  );
  $form['fieldset_suggest_title_d_settings']['suggest_similar_titles_node_access'] = array(
    '#type' => 'select',
    '#title' => t('Consider node permissions'),
    '#options' => array('no' => 'No', 'yes' => 'Yes'),
    '#default_value' => variable_get("suggest_similar_titles_node_access", "no"),
    '#description' => t('Select whether system should check node view permission
      before display node in similar titles suggestion list.<br />
      Yes: System will not display restricted nodes to the user.<br />
      No: System will display all matching node titles regardless of permission settings.'),
  );
  $form['fieldset_suggest_title_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Title compare patterns'),
  );
  $form['fieldset_suggest_title_settings']['suggest_similar_titles_ignored'] = array(
    '#type' => 'textfield',
    '#title' => t('Ignore keywords'),
    '#description' => t('Enter comma separated keywords to ignore in title comparison.'),
    '#default_value' => variable_get("suggest_similar_titles_ignored", "the,is,a"),
  );
  $form['fieldset_suggest_title_settings']['suggest_similar_titles_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Percentage'),
    '#description' => t('Enter percentage how exact system should compare the title.
      For example, if you enter 75, then atleast 75% matching title will be considered similar.'),
    '#default_value' => variable_get("suggest_similar_titles_percentage", 75),
    '#size' => 4,
    '#maxlength' => 2,
    '#field_suffix' => '%',
  );
  return system_settings_form($form);
  //return $form;
}
