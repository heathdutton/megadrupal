<?php

/**
 * Build project browsing navigation form.
 *
 * @param object $project_type
 *   A node type object.
 */
function project_solr_browse_projects_form($form, &$form_state, $project_type) {
  $form['#method'] = 'get';
  if (user_is_logged_in()) {
    $form['#token'] = FALSE;
  }

  // Pass along project type with the form.
  $form['#project_type'] = $project_type;

  // Create a drop-down for each related vocabulary.
  $vocabs = array();
  $fields_info = field_info_instances('node', $project_type->type);
  foreach ($fields_info as $field_name => $value) {
    $field_info = field_info_field($field_name);
    if ($field_info['type'] == 'taxonomy_term_reference' && $field_name != 'taxonomyextra') {
      foreach ($field_info['settings']['allowed_values'] as $allowed_vocab) {
        $vocabs[$allowed_vocab['vocabulary']] = TRUE;
      }
    }
  }

  $env_id = apachesolr_default_environment();

  // Use the Apachesolr query string 'f'.
  $form['f'] = array('#tree' => TRUE);

  // Add the API compatibilty option, if applicable.
  $api_vocab = FALSE;
  if (module_exists('project_release')) {
    $api_vocab = taxonomy_vocabulary_load(variable_get('project_release_api_vocabulary', ''));
    $vocabs[$api_vocab->machine_name] = TRUE;
  }

  $vocab_names = taxonomy_vocabulary_get_names();
  foreach (array_keys($vocabs) as $vocab) {
    $vocab = taxonomy_vocabulary_machine_name_load($vocab);
    $terms = taxonomy_get_tree($vocab->vid);
    $options = array('' => t(' - Any -'));

    // Filter values take the form of facet_name:facet_value.
    $facet_machine_name = 'im_vid_' . $vocab->vid;
    // Special facet machine name if this is the API vocab.
    if ($api_vocab && $api_vocab->vid === $vocab->vid) {
      // Load facet definition.
      $facet_definition = facetapi_facet_load('im_project_release_api_tids', 'apachesolr@' . $env_id);
      $facet_machine_name = $facet_definition['field alias'];
    }
    foreach ($terms as $term) {
      $options[$facet_machine_name . ':' . $term->tid] = $term->name;
    }

    $form['f'][] = array(
      '#type' => 'select',
      '#title' => check_plain($vocab->name),
      '#options' => $options,
    );
  }

  // Add the project sandbox fields.
  $form['f'][] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => array(
      'sm_field_project_type:full' => t('Full projects'),
      'sm_field_project_type:[* TO *]' => t('All projects'),
      'sm_field_project_type:sandbox' => t('Only sandbox projects'),
    ),
  );

  // Fill in default values.
  if (isset($_GET['f'])) {
    foreach (array_filter($_GET['f']) as $n => $value) {
      if (isset($form['f'][$n])) {
        $form['f'][$n]['#default_value'] = $value;
      }
    }
  }

  $form['text'] = array(
    '#title' => t('Search @project_type', array('@project_type' => $project_type->name)),
    '#type' => 'textfield',
    '#size' => 20,
  );

  $form['solrsort'] = array(
    '#type' => 'select',
    '#title' => t('Sort by'),
    '#options' => array(
      'score desc' => t('Relevancy'),
      'sort_label asc' => t('Title'),
      'ss_name asc' => t('Author'),
      'ds_created desc' => t('Created date'),
      'ds_project_latest_release desc' => t('Last release'),
      'ds_project_latest_activity desc' => t('Last build'),
    ),
    '#default_value' => 'score desc',
  );
  if (module_exists('project_usage')) {
    $form['solrsort']['#options']['iss_project_release_usage desc'] = t('Most installed');
    $form['solrsort']['#default_value'] = 'iss_project_release_usage desc';
  }
  if (isset($_GET['solrsort'])) {
    $form['solrsort']['#default_value'] = $_GET['solrsort'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#weight' => 101,
  );
  return $form;
}

/**
 * This generates a form containing version selection and a submit button.
 *
 * Generate a form with a version selection to allow filtering page content
 * based on the API compatibility version. Also includes path to allow the
 * form to potentially submit to other urls if desired.
 *
 * @param string $path
 *   The base path to which the version form will redirect.
 * @param string $label
 *   An optional label for the form element.
 */
function project_solr_version_form($form, &$form_state, $path, $label = NULL) {
  $env_id = apachesolr_default_environment();
  $query = apachesolr_current_query($env_id);

  $form = array(
    '#attributes' => array('class' => 'clearfix'),
  );

  // Add version select field to our form.
  if (module_exists('project_release')) {
    $current_tid = '';
    $terms = array();

    if ($query) {
      $term_query = new EntityFieldQuery();
      $result = $term_query->entityCondition('entity_type', 'taxonomy_term')
        ->fieldCondition('field_release_recommended', 'value', 1)
        ->execute();

      foreach (array_map('taxonomy_term_load', array_keys($result['taxonomy_term'])) as $term) {
        $active = $query->hasFilter('im_project_release_api_tids', $term->tid);
        if ($active) {
          $current_tid = $term->tid;
        }
        $terms[$term->tid] = $term->name;
      }
    }
    if (!empty($terms)) {
      $terms = array('' => t('- Any -')) + $terms;
      if (!isset($label)) {
        $label = t('Filter by compatibility');
      }
      $form[variable_get('project_solr_project_release_api_tids_alias', 'drupal_core')] = array(
        '#title' => $label,
        '#type' => 'select',
        '#options' => $terms,
        '#default_value' => $current_tid,
      );
    }
  }

  $form['path'] = array(
    '#type' => 'value',
    '#value' => $path,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  return $form;
}

/**
 * Create a query with the right version filter and redirect to the right page.
 *
 * Create a new query, add any version filtering if it was selected in the
 * form, and redirect back to the relevant page with the appropriate filter
 * string.
 */
function project_solr_version_form_submit($form, &$form_state) {
  // We create a new query with our base path so that we don't need to remove
  // any existing drupal_core selection, and so that the implict type and
  // module tid filters don't end up in the url string.
  $query = apachesolr_drupal_query('project_solr_version_form_submit', array(), '', $form_state['values']['path']);
  $filters = array();

  $version_alias = variable_get('project_solr_project_release_api_tids_alias', 'drupal_core');

  if (!empty($form_state['values'][$version_alias])) {
    $filters = array('f' => array($version_alias . ':' . $form_state['values'][$version_alias]));
  }
  $form_state['redirect'] = array('path' => $query->getPath(), array('query' => $filters));
}
