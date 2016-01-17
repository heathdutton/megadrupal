<?php
/**
 * @file
 * webform_feedback.admin.inc
 * Code for webform feedback setup form,
 * moved into a separate file for efficiency.
 */

/**
 * Form settings for module.
 */
function webform_feedback_settings($form) {
  // Add the first option in the dropdown to the beginning of the array.
  $no_feedback = t("No Webform Selected");
  $field_query[0] = array("0" => $no_feedback);
  // Find all webforms with block turned on.
  $webform_nid = db_query("SELECT nid FROM {webform} WHERE block = 1")->fetchCol();
  if (!empty($webform_nid)) {
    // Get the titles of all the webforms with block turned on.
    foreach ($webform_nid as $value) {
      $field_query[$value] = db_query("SELECT title FROM {node} WHERE nid = :blocknid", array(':blocknid' => $value))->fetchCol();
    }
  }
  /**
   * Remove a level in the array.
   */
  function webform_feedback_array_level_pop($field_query) {
    foreach ($field_query as $key => $extra_level) {
      $array_clean[$key] = array_pop($extra_level);
    }
    return $array_clean;
  }
  $processed = webform_feedback_array_level_pop($field_query);
  $form = array();
  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('Select the webform you would like to use as your feedback form below. If no webforms are listed make sure you expose the webform as a block (found under form settings > advanced). The second dropdown will determine if the button shows up on the left or right side of your web site.'),
  );
  $form['webform_feedback'] = array(
    '#type' => 'select',
    '#title' => t('Choose a feedback webform'),
    '#default_value' => variable_get('webform_feedback'),
    '#options' => $processed,
    '#description' => t('Choose a webform.'),
  );
  $form['webform_feedback_position'] = array(
    '#type' => 'select',
    '#title' => t('Choose which side you want the feedback button to be located.'),
    '#default_value' => variable_get('webform_feedback_position'),
    '#options' => array(
      'left' => t('left'),
      'right' => t('right'),
    ),
  );
  return system_settings_form($form);
}
