<?php

/**
 * @file
 * Taxonomy plugin.
 *
 * This plugin automatically create link to taxonomy terms.
 * It also allows to create taxonomy terms on the fly.
 *
 * It only works with taxonomy_term_reference fields.
 */


/**
 * Callback 'available' for taxonomy plugin.
 */
function opac_plugin_taxonomy_available($server, $field) {
  if ($field['type'] == 'taxonomy_term_reference') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Callback for taxonomy plugin.
 */
function opac_plugin_taxonomy($record, $mapping) {
  module_load_include('inc', 'opac', 'includes/helper.taxonomy');
  module_load_include('inc', 'opac', 'includes/helper.field');

  $remotefield = $mapping['mapped_with'];
  $node_field_name = $mapping['node_field_name'];
  $content_type = $mapping['content_type'];
  $node_field_info = _opac_field_info($node_field_name, $content_type);
  $vocabulary_machine_name = $node_field_info['settings']['allowed_values'][0]['vocabulary'];
  $term_field = _opac_entity_has_field('taxonomy_term', $mapping['term_field'], $vocabulary_machine_name) ?
    $mapping['term_field'] : 'name';

  // Get related vocabulary id.
  $vid = _opac_taxonomy_get_vid($vocabulary_machine_name);
  if (!$vid) {
    return $field;
  }

  $field = array('und' => array());

  if (isset($record[$remotefield])) {
    foreach ($record[$remotefield] as $value) {
      // Retieves the term id depending of the field value.
      $tid = _opac_taxonomy_get_term($vid, $term_field, $value);
      if ($tid) {
        $field['und'][] = array('tid' => $tid);
      }
      // No term match the field value. So if nomatch_rule
      // is 'create', we add a new term: The remotefield value.
      elseif ($mapping['nomatch_rule'] == 'create') {
        $tid = _opac_taxonomy_new_term($vid, $value, $term_field);
        $field['und'][] = array('tid' => $tid);
      }

      // If we did not find a taxo id, and we have a default value,
      // we use it.
      else {
        // If we have a default value, we use it.
        $default_tid = $node_field_info['instance']['default_value'][0]['tid'];
        if ($default_tid) {
          $field['und'][] = array('tid' => $default_tid);
        }
      }
    }
  }
  return $field;
}

/**
 * Additional form elements for taxonomy.
 */
function opac_plugin_taxonomy_form(&$form_state, $field_settings, $mapping) {
  $new = count($mapping['mapped']) ? 0 : 1;
  $vname = $field_settings['settings']['allowed_values'][0]['vocabulary'];
  $vocabulary = taxonomy_vocabulary_machine_name_load($vname);

  $vocabulary_fields_opt = array('name' => 'Term name');
  foreach (field_info_instances('taxonomy_term', $vname) as $name => $field) {
    $vocabulary_fields_opt[$name] = $field['label'];
  }

  $default_term_field = $new ? NULL : $mapping['mapped'][0]['term_field'];
  if (!$new && !isset($vocabulary_fields_opt[$default_term_field])) {
    $default_term_field = 'name';
    $message = t(
      'Taxonomy plugin has replaced @term_field with Term name.
      Probably because the field no longer exists or the vocabulary for the node field has changed.',
      array('@term_field' => $default_term_field)
    );
    drupal_set_message($message, 'error');
  }

  $form['taxonomy_rule'] = array(
    '#type' => 'fieldset',
    '#title' => check_plain(t('Taxonomy rule for vocabulary @name', array('@name' => $vocabulary->name))),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['taxonomy_rule']['term_field'] = array(
    '#type' => 'select',
    '#title' => t('Term field to compare'),
    '#required' => TRUE,
    '#default_value' => $new ? NULL : $mapping['mapped'][0]['term_field'],
    '#options' => $vocabulary_fields_opt,
    '#ajax' => array(
      'callback' => 'opac_plugin_taxonomy_term_field_label_callback',
      'wrapper' => 'taxonomy_term_field_label_div',
    ),
  );
  $form['taxonomy_rule']['nomatch_rule'] = array(
    '#title' => 'No match rule',
    '#type' => 'select',
    '#options' => array('none' => 'Leave empty', 'create' => 'Create new term'),
    '#default_value' => $new ? 'none' : $mapping['mapped'][0]['nomatch_rule'],
  );
  return $form;
}
