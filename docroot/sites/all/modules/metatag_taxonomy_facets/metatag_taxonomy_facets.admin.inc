<?php

/**
 * @file
 * Administration page callbacks for the metatag_taxonomy_facets module.
 */

/**
 * Form for selecting facets to use in combinations.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Created metatag_taxonomy_facets_configure_facets_form.
 */
function metatag_taxonomy_facets_configure_facets_form(array $form, array &$form_state) {
  $facets_selected = variable_get('metatag_taxonomy_facets_selected');
  $single_as_combination = variable_get('metatag_taxonomy_facets_single_facets');
  $ignore_unsupported_facets = variable_get('metatag_taxonomy_facets_ignore_unsupported_facets');

  $searchers = facetapi_get_searcher_info();
  if (empty($searchers)) {
    drupal_set_message(t('You have no active searchers.'), 'warning');
    return array();
  }

  $searcher_taxonomy_facets = array();
  foreach ($searchers as $searcher_name => $searcher_info) {
    $fieldset = array();
    $searcher_taxonomy_facets[$searcher_name] = 0;
    $facets = facetapi_get_enabled_facets($searcher_name);
    foreach ($facets as $facet_name => $facet_info) {
      if (metatag_taxonomy_facets_is_taxonomy_facet($facet_info)) {
        $fieldset["{$searcher_info['name']}:{$facet_name}"] = array(
          '#type' => 'checkbox',
          '#title' => $facet_info['label'],
          '#default_value' => isset($facets_selected[$searcher_info['name']][$facet_name]),
        );
        $searcher_taxonomy_facets[$searcher_name]++;
      }
    }

    if (!empty($fieldset)) {
      $fieldset += array(
        '#type' => 'fieldset',
        '#title' => $searcher_info['label'],
      );

      $fieldset["{$searcher_info['name']}:_single_as_combination_"] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable metatags for this searcher\'s single facets'),
        '#default_value' => isset($single_as_combination[$searcher_name]),
        '#prefix' => '<hr/><br/>',
      );
      $fieldset["{$searcher_info['name']}:_ignore_unsupported_facets_"] = array(
        '#type' => 'checkbox',
        '#title' => t('Ignore non-taxonomy facets'),
        '#default_value' => isset($ignore_unsupported_facets[$searcher_name]),
        '#prefix' => '<hr/><br/>',
      );

      $form[$searcher_info['name']] = $fieldset;
    }
  }

  $applicable_searchers = array_filter($searcher_taxonomy_facets, function ($searcher_facets) {
    return $searcher_facets > 0;
  });

  if (empty($applicable_searchers)) {
    drupal_set_message(t('You need to have at least one taxonomy facet configured in a searcher for this module two make sense.'), 'warning');
    return array();
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Selecting facets form submit function.
 *
 * @param array $form
 *   Form being submitted.
 * @param array $form_state
 *   Form state being submitted.
 */
function metatag_taxonomy_facets_configure_facets_form_submit(array $form, array &$form_state) {
  $single_as_combination = array();
  $ignore_unsupported_facets = array();
  $form_selected_facets = array();
  $form_unselected_facets = array();
  foreach ($form_state['values'] as $key => $value) {
    $key_exploded = explode(':', $key);
    if (count($key_exploded) <= 1) {
      continue;
    }
    $searcher_name = array_shift($key_exploded);
    if (count($key_exploded) == 1 && $key_exploded[0] == '_single_as_combination_') {
      if ($value) {
        $single_as_combination[$searcher_name] = TRUE;
      }
    }
    else if (count($key_exploded) == 1 && $key_exploded[0] == '_ignore_unsupported_facets_') {
      if ($value) {
        $ignore_unsupported_facets[$searcher_name] = TRUE;
      }
    } else {
      if ($value) {
        $form_selected_facets[$searcher_name][implode(':', $key_exploded)] = 1;
      } else {
        $form_unselected_facets[$searcher_name][implode(':', $key_exploded)] = 1;
      }
    }
  }

  variable_set('metatag_taxonomy_facets_selected', $form_selected_facets);
  variable_set('metatag_taxonomy_facets_unselected', $form_unselected_facets);
  variable_set('metatag_taxonomy_facets_single_facets', $single_as_combination);
  variable_set('metatag_taxonomy_facets_ignore_unsupported_facets', $ignore_unsupported_facets);

  $searchers = facetapi_get_searcher_info();
  $new_combinations = array();
  foreach ($searchers as $searcher_name => $searcher_info) {
    $facets = facetapi_get_facet_info($searcher_name);
    $selected_facets = array();
    foreach ($facets as $facet_name => $facet_info) {
      if (metatag_taxonomy_facets_is_taxonomy_facet($facet_info) && isset($form_selected_facets[$searcher_name][$facet_name])) {
        $selected_facets[$facet_name] = $facet_info;
      }
    }
    if (empty($selected_facets)) {
      continue;
    }
    $combinations = metatag_taxonomy_facets_get_combinations_info($searcher_info, $selected_facets, isset($single_as_combination[$searcher_name]));
    foreach ($combinations as $name => $label) {
      $new_combinations[$searcher_name][] = array(
        'searcher' => $searcher_name,
        'facets' => metatag_taxonomy_facets_get_facets_part($name, $searcher_name),
      );
    }
  }

  _metatag_taxonomy_facets_save_combinations($new_combinations);

  $num_new_combinations = array_reduce($new_combinations, function ($carry, $searcher_combinations) {
    return $carry + count($searcher_combinations);
  });
  if ($num_new_combinations) {
    drupal_set_message(t('!num taxonomy facets combinations enabled.', array('!num' => $num_new_combinations)));
  }
  else {
    drupal_set_message(t('No taxonomy facets combinations enabled.'), 'warning');
  }

  metatag_config_cache_clear();
  token_clear_cache();
}

/**
 * Facets combination instance metatags edit form.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param object $taxonomy_facet
 *   Taxonomy facets entity being edited.
 *
 * @return array
 *   Created form.
 */
function metatag_taxonomy_facets_instance_edit_form(array $form, array &$form_state, $taxonomy_facet) {
  $form['tfid'] = array(
    '#type' => 'hidden',
    '#value' => $taxonomy_facet->tfid,
  );

  field_attach_form('taxonomy_facets', $taxonomy_facet, $form, $form_state, language_default('language'));

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('metatag_taxonomy_facets_instance_edit_form_delete_submit'),
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function metatag_taxonomy_facets_form_metatag_taxonomy_facets_instance_edit_form_alter(array &$form, array &$form_state) {
  $form['metatags']['#collapsible'] = FALSE;
  $form['metatags']['#collapsed'] = FALSE;
  $form['actions']['#weight'] = 100;
}

/**
 * Facets combination instance metatags edit form submit.
 *
 * @param array $form
 *   Form being submitted.
 * @param array $form_state
 *   Form state being submitted.
 */
function metatag_taxonomy_facets_instance_edit_form_submit(array $form, array &$form_state) {
  $entity = entity_load_single('taxonomy_facets', $form_state['values']['tfid']);
  $entity->metatags = $form_state['values']['metatags'];
  $result = entity_save('taxonomy_facets', $entity);
  if ($result === FALSE) {
    drupal_set_message(t('Error occurred while saving metatags for this instance'), 'error');
  }
}

/**
 * Facets combination instance metatags deleting submit.
 *
 * @param array $form
 *   Form being submitted.
 * @param array $form_state
 *   Form state being submitted.
 */
function metatag_taxonomy_facets_instance_edit_form_delete_submit(array $form, array &$form_state) {
  $result = entity_delete('taxonomy_facets', $form_state['values']['tfid']);
  if ($result === FALSE) {
    drupal_set_message(t('Error occurred while deleting metatags for this instance'), 'error');
  }
  else {
    drupal_set_message(t('Metatags for this instance successfully removed'));
    drupal_goto('admin/config/search/metatags');
  }
}

/**
 * Facets combination instance metatags add form.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param int $combination_id
 *   Combination entity id.
 * @param string $tids
 *   String of taxonomy terms ids delimited by commas.
 *
 * @return array
 *   Created form.
 */
function metatag_taxonomy_facets_instance_add_form(array $form, array &$form_state, $combination_id, $tids) {
  $form['combination_id'] = array(
    '#type' => 'hidden',
    '#value' => $combination_id,
  );
  $form['tids'] = array(
    '#type' => 'hidden',
    '#value' => urldecode($tids),
  );

  $instance = metatag_taxonomy_facets_get_combination_instance($combination_id);
  $metatags = metatag_config_load($instance);
  $taxonomy_facet = entity_create('taxonomy_facets', array(
    'combination_id' => $combination_id,
    'tids' => $tids,
    'metatags' => $metatags->config,
  ));

  metatag_field_attach_form('taxonomy_facets', $taxonomy_facet, $form, $form_state, language_default('language'));

  if (!isset($form['#metatags'])) {
    drupal_set_message(t('Metatag denies support for taxonomy facets.'), 'warning');
    $is_suitable = (int) metatag_entity_type_is_suitable('taxonomy_facets', entity_get_info('taxonomy_facets'));
    drupal_set_message(t('Is entity type suitable? ') . "({$is_suitable})", 'status');
    $does_support = (int) metatag_entity_supports_metatags('taxonomy_facets', 'taxonomy_facets');
    drupal_set_message(t('Does entity support metatags? ') . "({$does_support})", 'status');
  } else {
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function metatag_taxonomy_facets_form_metatag_taxonomy_facets_instance_add_form_alter(array &$form, array &$form_state) {
  $form['metatags']['#collapsible'] = FALSE;
  $form['metatags']['#collapsed'] = FALSE;
  $form['actions']['#weight'] = 100;
}

/**
 * Facets combination instance metatags add form submit.
 *
 * @param array $form
 *   Form being submitted.
 * @param array $form_state
 *   Form state being submitted.
 */
function metatag_taxonomy_facets_instance_add_form_submit(array $form, array &$form_state) {
  $values = array(
    'combination_id' => $form_state['values']['combination_id'],
    'tids' => $form_state['values']['tids'],
    'metatags' => $form_state['values']['metatags'],
  );
  $entity = entity_create('taxonomy_facets', $values);
  $result = entity_save('taxonomy_facets', $entity);

  if ($entity && $result !== FALSE) {
    $form_state['redirect'] = "admin/config/search/metatags/taxonomy_facets_item/{$entity->tfid}/edit";
  }
  else {
    drupal_set_message(t('Error occurred while saving metatags for this instance'), 'error');
  }
}
