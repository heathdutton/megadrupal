<?php

/**
 * @file
 * Batch create taxonomy terms.
 */

module_load_include('inc', 'opentags', 'opentags.taxonomy');

/**
 * Present a form to create terms.
 */
function opentags_batch_create_taxonomy_form($form, &$form_state) {
  $form['intro'] = array(
    '#markup' => '<p>' . t('The OpenTags module will create a set of taxonomy 
      terms which will be used by the OpenTags widgets. Please note that the 
      module will not work properly without the required terms being fully 
      created. Click the following button if you want to proceed. This process 
      will take several minutes.') . '</p>',
  );

  $form['create'] = array(
    '#type' => 'submit',
    '#value' => 'Create OpenTags Taxonomy',
  );
  return $form;
}

/**
 * Create taxonomy form submit handler.
 */
function opentags_batch_create_taxonomy_form_submit($form, &$form_state) {
  // Create fields.
  if (!field_info_field('field_opentags_term_value')) {
    field_create_field(array(
      'field_name' => 'field_opentags_term_value',
      'type' => 'text',
      'cardinality' => 1,
    ));
  }

  if (!field_info_field('field_opentags_term_disabled')) {
    field_create_field(array(
      'field_name' => 'field_opentags_term_disabled',
      'type' => 'list_boolean',
      'cardinality' => 1,
      'settings' => array(
        'allowed_values' => array(0 => 0, 1 => 1),
      ),
    ));
  }

  // Create Vocabularies.
  $vocab_names = array(
    'opentags_audience',
    'opentags_coverage',
    'opentags_function',
    'opentags_language',
    'opentags_type',
    'opentags_subject',
  );

  $total_terms_count = 0;
  $batch_operations = array();

  foreach ($vocab_names as $vocab_name) {
    $vocab_data = opentags_taxonomy_load_data_xml($vocab_name);
    $total_terms_count += count($vocab_data['terms']['term']);

    $vocab_setting = array(
      'name' => $vocab_data['name'],
      'machine_name' => $vocab_data['machine_name'],
      'description' => $vocab_data['description'],
    );
    opentags_taxonomy_create_vocabulary($vocab_setting);

    // Create field instances and attach to vocabulary.
    field_create_instance(array(
      'field_name' => 'field_opentags_term_value',
      'entity_type' => 'taxonomy_term',
      'bundle' => $vocab_data['machine_name'],
      'label' => 'Value',
      'description' => 'Specify the term value if it
        is different from the term label',
      'widget' => array(
        'type' => 'text_textfield',
      ),
    ));

    field_create_instance(array(
      'field_name' => 'field_opentags_term_disabled',
      'entity_type' => 'taxonomy_term',
      'bundle' => $vocab_data['machine_name'],
      'label' => 'Disabled',
      'description' => 'Check this to disable term from selection in OpenTags widget',
      'widget' => array(
        'type' => 'options_onoff',
        'active' => 1,
        'settings' => array(
          'display_label' => 1,
        ),
      ),
    ));

    $terms = array();
    for ($i = 0; $i < count($vocab_data['terms']['term']); $i++) {
      $terms[] = array(
        'id' => $vocab_data['terms']['term'][$i]['term_id'],
        'name' => $vocab_data['terms']['term'][$i]['term_name'],
        'value' => $vocab_data['terms']['term'][$i]['term_value'],
        'parent' => $vocab_data['terms']['term'][$i]['term_parent'],
        'disabled' => $vocab_data['terms']['term'][$i]['term_disabled'],
      );
    }

    // Create batch operation array.
    $batch_operations[] = array(
      'opentags_batch_taxonomy_create_terms',
      array($vocab_data['machine_name'], $terms),
    );
    $batch_operations[] = array(
      'opentags_batch_taxonomy_create_hierarchy',
      array($vocab_data['machine_name'], $terms),
    );
  }

  $batch = array(
    'title' => t('Create OpenTags Taxonomy'),
    'operations' => $batch_operations,
    'finished' => 'opentags_batch_taxonomy_finished',
    'init_message' => t('Start to create OpenTags taxonomy...'),
    'progress_message' => t('Creating OpenTags taxonomy...'),
    'error_message' => t('An error occurred while creating terms.'),
    'file' => OPENTAGS_MODULE_PATH . '/opentags.batch.inc',
  );

  batch_set($batch);

  batch_process('admin/modules');
}

/**
 * Batch operation function to create terms.
 *
 * @param string $vocab_name
 *   Vocabulary machine name.
 * @param array $terms
 *   An array of term definations.
 */
function opentags_batch_taxonomy_create_terms($vocab_name, array $terms, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($terms);
  }

  if (!isset($context['results']['created'])) {
    $context['results']['created'] = 0;
    $context['results']['id_to_tid'] = array();
    $context['results']['id_to_tid'][$vocab_name] = array();
  }

  $limit = ($context['sandbox']['max'] - $context['sandbox']['progress'] > 100) ? 100 : $context['sandbox']['max'] - $context['sandbox']['progress'];

  for ($i = 0; $i < $limit; $i++) {
    // Create term.
    $vocab = taxonomy_vocabulary_machine_name_load($vocab_name);
    $term_item = $terms[$context['sandbox']['progress']];
    if ($vocab) {
      $term = new stdClass();
      $term->vid = $vocab->vid;
      $term->name = $term_item['name'];
      if (isset($term_item['description'])) {
        $term->description = $term_item['description'];
      }
      $term->parent = 0;
      $term->weight = $context['sandbox']['progress'];

      $term->field_opentags_term_value[LANGUAGE_NONE][0]['value'] = !empty($term_item['value']) ? $term_item['value'] : '';
      $term->field_opentags_term_disabled[LANGUAGE_NONE][0]['value'] = !empty($term_item['disabled']) ? 1 : 0;
      taxonomy_term_save($term);
      if (!isset($context['results']['id_to_tid'][$vocab_name][$term_item['id']])) {
        $context['results']['id_to_tid'][$vocab_name][$term_item['id']] = $term->tid;
      }
    }

    $context['results']['created']++;
    $context['sandbox']['progress']++;
    $context['message'] = t('Creating @vocab terms: @processed out of @total.',
      array(
        '@vocab' => $vocab_name,
        '@processed' => $context['sandbox']['progress'],
        '@total' => $context['sandbox']['max'],
      )
    );
  }

  if ($context['sandbox']['progress'] <= ($context['sandbox']['max'])) {
    $context['finished'] = $context['sandbox']['progress'] / ($context['sandbox']['max']);
  }
}

/**
 * Batch operation function to create term hierarchy.
 *
 * @param string $vocab_name
 *   Vocabulary machine name.
 * @param array $terms
 *   An array of term definations.
 */
function opentags_batch_taxonomy_create_hierarchy($vocab_name, array $terms, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($terms);
  }

  $limit = ($context['sandbox']['max'] - $context['sandbox']['progress'] > 100) ? 100 : $context['sandbox']['max'] - $context['sandbox']['progress'];

  for ($i = 0; $i < $limit; $i++) {
    $term_item = $terms[$context['sandbox']['progress']];

    if (isset($term_item['parent'])) {
      $tid = $context['results']['id_to_tid'][$vocab_name][$term_item['id']];
      $term = taxonomy_term_load($tid);
      if (isset($context['results']['id_to_tid'][$vocab_name][$term_item['parent']])) {
        $term->parent = $context['results']['id_to_tid'][$vocab_name][$term_item['parent']];
        taxonomy_term_save($term);
      }
    }

    $context['sandbox']['progress']++;
    $context['message'] = t('Creating @vocab term hierarchy: @processed out of @total.',
      array(
        '@vocab' => $vocab_name,
        '@processed' => $context['sandbox']['progress'],
        '@total' => $context['sandbox']['max'],
      )
    );
  }

  if ($context['sandbox']['progress'] <= ($context['sandbox']['max'])) {
    $context['finished'] = $context['sandbox']['progress'] / ($context['sandbox']['max']);
  }

}

/**
 * Batch finished operation.
 */
function opentags_batch_taxonomy_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('OpenTags has created %count terms.', array('%count' => $results['created'])));
  }
  else {
    drupal_set_message(t('There is an error while creating the terms.'), 'error');
  }
}
