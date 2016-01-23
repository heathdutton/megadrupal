<?php
/**
 * @file
 * This file contains the administration page and related functions.
 */

/**
 * Personalization admin form. This form is used for the general settings tab.
 */
function personalization_admin() {
  $form = array();
  $form['#vocabularies'] = array();
  foreach (taxonomy_get_vocabularies() as $v) {
    if ($v->machine_name != PERSONALIZATION_GEO_VOCABULARY_NAME) {
      $form['#vocabularies'][$v->vid] = $v->name;
    }
  }

  $settings = variable_get('personalization_settings', array());

  drupal_add_css(drupal_get_path('module', 'personalization') . '/css/admin_form.css');
  drupal_add_js(drupal_get_path('module', 'personalization') . '/js/admin_form.js');

  $form['user_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('User settings'),
  );

  $form['user_settings']['pz_user_score_ttl'] = array(
    '#type' => 'textfield',
    '#title' => t('User profile TTL'),
    '#description' => t('Number of minutes a users personalization profile will be kept.'),
    '#default_value' => $settings['pz_user_score_ttl'],
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => TRUE,
  );

  $form['user_settings']['pz_user_content_ttl'] = array(
    '#type' => 'textfield',
    '#title' => t("User's content cache TTL"),
    '#description' => t('Number of minutes a users personalized content will be cached for.'),
    '#default_value' => $settings['pz_user_content_ttl'],
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => TRUE,
  );

  $form['taxonomony_based'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy based personalization'),
  );

  $form['taxonomony_based']['pz_vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Which vocabularies should be used?'),
    '#options' => $form['#vocabularies'],
    '#default_value' => $settings['pz_vocabularies'],
    '#description' => t("Select the vocabularies that should be used for site personalization."),
  );

  $form['taxonomony_based']['pz_vocab_weight_title'] = array(
    '#type' => 'item',
    '#title' => t('Weight to be applied to each vocabulary.'),
  );

  foreach ($form['#vocabularies'] as $k => $v) {
    $form['taxonomony_based']['pz_vocab_weight_' . $k] = array(
      '#type' => 'textfield',
      '#description' => $v,
      '#default_value' => isset($settings['pz_vocabulary_weights'][$k]) && $settings['pz_vocabulary_weights'][$k] ? $settings['pz_vocabulary_weights'][$k] : 10,
      '#size' => 6,
      '#maxlength' => 6,
    );
  }

  $form['taxonomony_based']['pz_vocab_weight_desc'] = array(
    '#type' => 'item',
    '#description' => t('The weighting sets the importance of each vocabulary when selecting content for the user. For example if the node Categories is twice as important as Tags give Categories twice the weight.'),
  );

  $form['search_based'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search keyword weighting'),
  );

  $form['search_based']['pz_search_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Search keyword weighting'),
    '#description' => t('Weight applied to search terms that match your mappings.'),
    '#default_value' => $settings['pz_search_weight'],
    '#size' => 6,
    '#maxlength' => 6,
  );

  $form['geographic_based'] = array(
    '#type' => 'fieldset',
    '#title' => t('Geographical based personalization'),
  );

  $form['geographic_based']['pz_geo'] = array(
    '#type' => 'checkboxes',
    '#title' => t("How should the user's location be ascertained"),
    '#default_value' => $settings['pz_geo'],
    '#options' => array(
      'from_api' => t('Request location from the users IP automatically from http://www.telize.com/ API'),
      'ask_user' => t('Ask the user for their location.'),
    ),
    '#description' => t("If you select both options the user will only be asked if http://www.telize.com/ cannot ascertain the location. Requesting the location from the user will require them to be using a HTML5 compliant browser and your Drupal theme to be HTML5."),
  );

  $form['geographic_based']['pz_geo_user_weight'] = array(
    '#type' => 'textfield',
    '#title' => t("User's location weighting"),
    '#description' => t("Weight applied to the user's location."),
    '#default_value' => $settings['pz_geo_user_weight'],
    '#size' => 6,
    '#maxlength' => 6,
  );

  $form['geographic_based']['pz_geo_vocab_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Location vocabulary weighting'),
    '#description' => t('Weight applied to the user accessing location tagged pages.'),
    '#default_value' => $settings['pz_geo_vocab_weight'],
    '#size' => 6,
    '#maxlength' => 6,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * The validation function for the general settings form.
 */
function personalization_admin_validate($form, &$form_state) {
  // Ensure the profile TTL is valid.
  if (!$form_state['values']['pz_user_score_ttl'] || !is_numeric($form_state['values']['pz_user_score_ttl'])) {
    form_set_error('pz_user_score_ttl', t('Please enter a valid number of minutes.'));
  }

  // Ensure the cache TTL is valid.
  if (!is_numeric($form_state['values']['pz_user_content_ttl'])) {
    form_set_error('pz_user_content_ttl', t('Please enter a valid number of minutes.'));
  }

  // Ensure the weightings are valid.
  if (isset($form['#vocabularies'])) {
    foreach ($form['#vocabularies'] as $vid => $term_name) {
      if (isset($form_state['values']['pz_vocab_weight_' . $vid])) {
        $form_state['values']['pz_vocab_weight_' . $vid] = (int) $form_state['values']['pz_vocab_weight_' . $vid];
      }
    }
  }
}

/**
 * The submit function for the general settings form.
 *
 * This stores the config in one Drupal variable as having separate values
 * resultsBoolean in many queries when tracking users, mainly around vocabulary
 * weighting.
 */
function personalization_admin_submit($form, &$form_state) {
  $settings = $form_state['values'];
  unset($settings['submit'], $settings['form_build_id'], $settings['form_token'], $settings['form_id'], $settings['op']);

  // Check geo checkboxes and if one's been set make sure the geo vocabulary is
  // available.
  if (array_filter($settings['pz_geo'])) {
    personalization_add_geo_vocabulary();
  }

  $geo_v = taxonomy_vocabulary_machine_name_load(PERSONALIZATION_GEO_VOCABULARY_NAME);

  // Loop through the vocabularies and store their weight with them rather than
  // having to link later.
  foreach ($settings['pz_vocabularies'] as $vid => $v) {
    if ($v) {
      if (isset($settings['pz_vocab_weight_' . $vid])) {
        $settings['pz_vocabulary_weights'][$vid] = $settings['pz_vocab_weight_' . $vid];
        unset($settings['pz_vocab_weight_' . $vid]);
      }
    }
    else {
      unset($settings['pz_vocabularies'][$vid]);
      if (isset($settings['pz_vocab_weight_' . $vid])) {
        unset($settings['pz_vocab_weight_' . $vid]);
      }
    }
  }
  if ($geo_v) {
    $settings['pz_vocabularies'][$geo_v->vid] = $geo_v->vid;
  }
  if (!isset($settings['pz_vocabulary_weights'])) {
    $settings['pz_vocabulary_weights'] = array();
  }

  // Grab the old setting so we can see what's changed.
  $old_settings = variable_get('personalization_settings', array());

  // Store the settings.
  variable_set('personalization_settings', $settings);

  // Re-structure the arrays as array_diff doesn't like multidimensional arrays.
  $top_level_settings = array();
  foreach ($settings as $k => $v) {
    if (!is_array($v)) {
      $top_level_settings[$k] = $v;
    }
  }

  $top_level_old_settings = array();
  foreach ($old_settings as $k => $v) {
    if (!is_array($v)) {
      $top_level_old_settings[$k] = $v;
    }
  }

  // Recalculate all scores.
  $changes = array_diff($top_level_settings, $top_level_old_settings);
  $changes['pz_vocabularies'] = array_diff($settings['pz_vocabularies'], $old_settings['pz_vocabularies']);
  $changes['pz_geo'] = array_diff($settings['pz_geo'], $old_settings['pz_geo']);
  $changes['pz_vocabulary_weights'] = array_diff($settings['pz_vocabulary_weights'], $old_settings['pz_vocabulary_weights']);
  $changes = array_filter($changes);

  // No need to recalculate scores if nothing's changed.
  if ($changes) {
    // Delete expired scores.
    personalization_expire_scores();
    cache_clear_all('*', 'cache_personalization', TRUE);

    // Delete scores on vocabs no longer in use.
    if (isset($changes['pz_vocabularies'])) {
      $active_vocabularies = array_keys($settings['pz_vocabulary_weights']);
      $active_vocabularies[] = $geo_v->vid;
      db_delete('personalization_user_scores')->condition('vid', $active_vocabularies, 'NOT IN')->execute();
    }

    // Add the geo vid to the weights.
    $settings['pz_vocabulary_weights'][$geo_v->vid] = $settings['pz_geo_vocab_weight'];

    // Get scores that need updating.
    $query = db_select('personalization_user_scores', 'pzs')->fields('pzs');

    $has_or = FALSE;

    $db_or = db_or();
    if (isset($changes['pz_search_weight'])) {
      $db_or->condition('searchdeletedes', 0, '<>');
      $has_or = TRUE;
    }
    if (isset($changes['pz_geo_user_weight'])) {
      $db_or->condition('location', 0, '<>');
      $has_or = TRUE;
    }
    if (isset($changes['pz_geo_vocab_weight']) || isset($changes['pz_vocabulary_weights'])) {
      $db_or->condition('pages', 0, '<>');
      $has_or = TRUE;
    }

    if ($has_or) {
      $query->condition($db_or);
    }
    $scores = $query->execute();
    $new_scores = array();

    // Loop through the scores and reapply the weights.
    while ($s = $scores->fetchAssoc()) {
      $weight = 0;
      // Update the page score.
      if ($s['pages'] && isset($settings['pz_vocabulary_weights'][$s['vid']])) {
        $weight += $s['pages'] * $settings['pz_vocabulary_weights'][$s['vid']];
      }
      // Update location score.
      if ($s['location']) {
        $weight += $settings['pz_geo_user_weight'];
      }
      // Update search scores.
      if ($s['searches']) {
        $weight += $s['searches'] * $settings['pz_search_weight'];
      }
      $s['score'] = $weight;
      $new_scores[$s['id']] = $s;
    }

    // Save the updated scores back into the database 100 at a time.
    if ($new_scores) {
      $job = array();
      $operations = array();

      foreach ($new_scores as $v) {
        $job[$v['id']] = $v;

        if (count($job) == 100 || (count($new_scores) < 100 && count($new_scores) == count($job))) {
          $operations[] = array(
            'personalization_admin_update_scores_batch',
            array(
              $job,
            ),
          );
          $job = array();
        }
      }

      $batch = array(
        'operations' => $operations,
        'finished' => 'personalization_admin_update_scores_batch_finished',
        'title' => t('Recalculating user scores'),
        'init_message' => t("Users profile scores need to be recalculate based on the changes you've made."),
        'progress_message' => t('Processed @current out of @total batches.'),
        'error_message' => t('An error occurred, please check your settings.'),
      );

      batch_set($batch);
    }
  }

  drupal_set_message(t('Settings updated successfully.'));
}

/**
 * Updates the scores of 100 records, called by the batch process.
 *
 * @param array $rows
 *   Rows to update from the database
 *
 * @return bool
 *   Whether or not the rows were updated successfully.
 */
function personalization_admin_update_scores_batch($rows) {
  if ($rows) {
    // Delete the scores we're replacing.
    db_delete('personalization_user_scores')->condition("id", array_keys($rows), 'IN')->execute();

    // Reinsert the updated scores.
    $insert = db_insert('personalization_user_scores')->fields(array_keys(current($rows)));

    foreach ($rows as $s) {
      $insert->values($s);
    }

    $insert->execute();

    return TRUE;
  }
  return FALSE;
}

/**
 * Success message for the score update batch.
 */
function personalization_admin_update_scores_batch_finished() {
  drupal_set_message(t('User scores updated successfully.'));
}

/**
 * The search keywords mapping page.
 *
 * This page contains the "Add mapping" form as well as a paginated table of
 * existing mappings which can be removed.
 *
 * @return array
 *   A render array containing a table of results and pagination.
 */
function personalization_admin_search() {
  $return = array();

  // Add mapping form.
  $form = drupal_get_form('personalization_admin_search_form');
  $return[] = $form;

  // Run the existing mappings through theme_table.
  $table = array(
    'header' => array(
      array(
        'data' => t('Keyword'),
        'field' => 'keyword',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Term'),
        'field' => 'name',
      ),
      array(
        'data' => NULL,
      ),
    ),
    'rows' => array(),
    'attributes' => array(),
    'caption' => NULL,
    'colgroups' => array(),
    'sticky' => TRUE,
    'empty' => t('No mappings added yet.'),
  );

  // Get the existing mappings.
  $query = db_select('personalization_keyword_mapping', 'pzm');
  $query->leftJoin('taxonomy_term_data', 'td', 'td.tid = pzm.tid');
  $query = $query->fields('pzm', array(
    'id',
    'keyword',
  ));
  $query = $query->fields('td', array(
    'name',
  ));
  $query = $query->extend('TableSort')->extend('PagerDefault')->limit(50);
  $result = $query->orderByHeader($table['header'])->execute();

  // Build the rows.
  foreach ($result as $mapping) {
    $table['rows'][] = array(
      filter_xss($mapping->keyword),
      filter_xss($mapping->name),
      l(t('delete'), 'admin/config/system/personalization/search/delete/' . $mapping->id),
    );
  }

  // Add the table to the return array.
  $return['mapping_table'] = array(
    '#theme' => 'table',
    '#header' => $table['header'],
    '#rows' => $table['rows'],
  );

  // Add the pager to the return array.
  $return['mapping_table_pager'] = array(
    '#theme' => 'pager',
  );

  return $return;
}

/**
 * The "Add mapping" form.
 *
 * @return array
 *   Form declaration for the add mapping form.
 */
function personalization_admin_search_form() {
  $form = array();

  $form['pz_sm_new_mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add new search keyword mapping'),
  );

  $form['pz_sm_new_mapping']['pz_sm_keyword'] = array(
    '#type' => 'textfield',
    '#title' => t('Search keyword'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['pz_sm_new_mapping']['pz_sm_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Taxonomy term'),
    '#required' => TRUE,
    '#autocomplete_path' => 'personalization/ajax/admin/search/terms',
    '#maxlength' => 255,
  );

  $form['pz_sm_new_mapping']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add mapping'),
  );

  return $form;
}

/**
 * Validates the new mapping.
 */
function personalization_admin_search_form_validate($form, &$form_state) {
  $tid = explode('(tid=', rtrim($form_state['values']['pz_sm_term'], ')'));

  $vocabularies = array();
  $settings = variable_get('personalization_settings', array());

  foreach ($settings['pz_vocabularies'] as $v) {
    if ($v) {
      $vocabularies[] = $v;
    }
  }

  // Check the term is from the allowed vocabularies.
  if (isset($tid[1]) && is_numeric($tid[1]) && $form_state['values']['pz_sm_keyword']) {
    $tid = db_select('taxonomy_term_data', 't')->condition('vid', $vocabularies, 'IN')->fields('t', array(
      'tid',
    ))->condition('tid', (int) $tid[1], '=')->range(0, 1)->execute()->fetchField();

    if ($tid) {
      $form_state['values']['pz_sm_term'] = $tid;

      // Check if this mapping has already been added.
      $map = db_query('SELECT id FROM {personalization_keyword_mapping} WHERE keyword = :keyword AND tid = :tid LIMIT 1', array(':keyword' => $form_state['values']['pz_sm_keyword'], ':tid' => $tid))->fetchField();

      if ($map) {
        form_set_error('pz_sm_keyword', t('That mapping has already been added.'));
      }
    }
    else {
      form_set_error('pz_sm_term', t('Please enter a valid term.'));
    }
  }
  else {
    form_set_error('pz_sm_term', t('Please enter a valid term.'));
  }
}

/**
 * Inserts the new search mapping into the database.
 */
function personalization_admin_search_form_submit($form, &$form_state) {
  $term = taxonomy_term_load((int) $form_state['values']['pz_sm_term']);

  $id = db_insert('personalization_keyword_mapping')->fields(array(
    'keyword' => $form_state['values']['pz_sm_keyword'],
    'vid' => $term->vid,
    'tid' => $term->tid,
    'created' => REQUEST_TIME,
  ))->execute();

  if ($id) {
    drupal_set_message(t('Mapping created successfully.'));
  }
}

/**
 * Form constructor for the search mapping deletion confirmation form.
 *
 * @see personalization_search_delete_confirm_submit()
 */
function personalization_search_delete_confirm($form, &$form_state, $mapping_id) {
  $form['mapping_id'] = array('#type' => 'value', '#value' => $mapping_id);

  $mapping = db_query('SELECT km.keyword, td.name FROM {personalization_keyword_mapping} km LEFT JOIN {taxonomy_term_data} td ON km.tid = td.tid WHERE km.id = :mapping_id LIMIT 1', array(':mapping_id' => $mapping_id))->fetchAssoc();

  if ($mapping) {
    return confirm_form($form,
      t('Are you sure you want to delete this mapping: %keyword = %term?', array('%keyword' => $mapping['keyword'], '%term' => $mapping['name'])),
      'admin/config/system/personalization/search',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
  else {
    return drupal_not_found();
  }
}

 /**
  * Form submission handler for node_revision_delete_confirm().
  */
function personalization_search_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $mapping = db_query('SELECT km.keyword, td.name FROM {personalization_keyword_mapping} km LEFT JOIN {taxonomy_term_data} td ON km.tid = td.tid WHERE km.id = :mapping_id LIMIT 1', array(':mapping_id' => $form_state['values']['mapping_id']))->fetchAssoc();
    if ($mapping) {
      db_delete('personalization_keyword_mapping')->condition('id', $form_state['values']['mapping_id'])->execute();
      watchdog('personalization', 'deleted mapping: %keyword = %term', array('%keyword' => $mapping['keyword'], '%term' => $mapping['name']));
      drupal_set_message(t('Mapping removed successfully.'));
    }
  }

  $form_state['redirect'] = 'admin/config/system/personalization/search';
}
