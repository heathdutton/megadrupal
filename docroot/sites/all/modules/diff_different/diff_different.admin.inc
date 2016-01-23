<?php

/**
 * @file
 * diff_different.admin.inc
 */

/**
 * Global admin settings form.
 *
 * @param array $form
 *   The admin config form (stub).
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The completed $form.
 */
function diff_different_admin_settings($form, $form_state) {

  $form['diff_different_content_types'] = array(
    '#title' => t('Select where the <em>diff</em> should be inserted.'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#description' => t('The position (aka weight) of the inserted diff section is relative to an "anchor" field, selected below for each content type.') . '<br/>' .
      t('The most common choice is the <em>Body</em> field, if it exists for the selected content type.') . '<br/>' .
      t('Drop-downs reflect field ordering on the content type.'),
  );

  foreach (node_type_get_types() as $content_type => $type) {

    $form['diff_different_content_types']["diff_different_{$content_type}_weight"] = array(
      '#prefix' => '<div class="field-and-weight">',
      '#type' => 'select',
      '#title' => t('&nbsp;'),
      '#default_value' => variable_get("diff_different_{$content_type}_weight", 1),
      '#options' => array(
        -1 => t('Above'),
        1 => t('Below'),
      ),
    );

    $content_type_field_names = array_keys(field_read_fields(array('bundle' => $content_type)));
    $instances = array();
    foreach ($content_type_field_names as $field_name) {
      $instances[$field_name] = field_info_instance('node', $field_name, $content_type);
    }
    uasort($instances, '_diff_different_cmp_weight');

    $options = array('' => '--' . t('Disabled') . '--');
    foreach ($instances as $field_name => $instance) {
      $options[$field_name] = $instance['label'];
      if (variable_get('diff_different_debug', FALSE)) {
        $weight = isset($instance['display']['default']['weight']) ? (int) $instance['display']['default']['weight'] : '?';
        $options[$field_name] .= ' ' . t('(wght @weight)', array('@weight' => $weight));
      }
    }
    $form['diff_different_content_types']["diff_different_{$content_type}_field"] = array(
      '#type' => 'select',
      '#title' => $type->name,
      '#default_value' => variable_get("diff_different_{$content_type}_field"),
      '#options' => $options,
      '#suffix' => '</div>',
    );
  }

  $form['diff_different_revision_scenario'] = array(
    '#type' => 'radios',
    '#title' => t("Which pair of revisions should be <em>diffed</em>?"),
    '#default_value' => (int) variable_get('diff_different_revision_scenario'),
    '#options' => array(
      0 => t('Latest author revision on the left, latest editor revision after that on the right'),
      1 => t('Very last revision on the right, revision before that on the left'),
    ),
    '#description' => t('The first option only applies when two or more users have edited the same node. The second option does not care who made the changes.'),
  );
  $form['#attached']['css'][] = drupal_get_path('module', 'diff_different') . '/diff_different.admin.css';
  return system_settings_form($form);
}

/**
 * Compare field instances by their display weights.
 *
 * @param int|string $field1
 *   The first field instance.
 * @param int|string $field2
 *   The second field instance
 *
 * @return int
 *   -1, 0, or +1
 */
function _diff_different_cmp_weight($field1, $field2) {
  $weight1 = isset($field1['display']['default']['weight']) ? (int) $field1['display']['default']['weight'] : 0;
  $weight2 = isset($field2['display']['default']['weight']) ? (int) $field2['display']['default']['weight'] : 0;
  return $weight1 < $weight2 ? -1 : ($weight1 == $weight2 ? 0 : 1);
}
