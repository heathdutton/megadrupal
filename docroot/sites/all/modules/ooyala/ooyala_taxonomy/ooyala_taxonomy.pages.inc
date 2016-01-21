<?php

/**
 * @file
 * Menu callbacks and forms provided by the Ooyala taxonomy module.
 */

/**
 * Menu callback; Batch process new vocabulary syncing.
 */
function ooyala_taxonomy_sync_confirm($form, $form_state, $vids = array()) {
  $form = array();

  if (empty($vids) && isset($_GET['vids']) && is_array($_GET['vids'])) {
    $vids = $_GET['vids'];
  }

  $vocabularies = taxonomy_get_vocabularies();
  $vocabulary_list = array();
  foreach ($vocabularies as $vocabulary) {
    if (isset($vids[$vocabulary->vid]['olid'])) {
      $vocabulary_list[] = t('The %vocabulary vocabulary will be copied to Ooyala Backlot as labels.', array('%vocabulary' => $vocabulary->name));
    }
  }

  $form['vocabularies'] = array(
    '#type' => 'value',
    '#value' => $vids,
  );

  $question = t('Sync existing terms with Ooyala?');
  $description = '<p>' . t("The vocabularies you've chosen to sync need to be populated into Ooyala Backlot:") . '</p>';
  $description .= theme('item_list', array('items' => $vocabulary_list));
  $description .= '<p>' . t("This does not affect existing videos. If you cancel this action, syncing will not be enabled.") . '</p>';
  $path = 'admin/config/media/ooyala/settings';
  return confirm_form($form, $question, $path, $description);
}

function ooyala_taxonomy_sync_confirm_submit($form, &$form_state) {
  $batch = array(
    'operations' => array(),
    'finished' => 'ooyala_taxonomy_term_sync_push_finished',
    'title' => t('Processing'),
    'error_message' => t('The sync has encountered an error.'),
    'file' => drupal_get_path('module', 'ooyala_taxonomy') .'/ooyala_taxonomy.inc',
  );

  // Push taxonomy terms to Ooyala one vocabulary at a time.
  foreach ($form_state['values']['vocabularies'] as $vid => $sync_settings) {
    if (isset($sync_settings['olid'])) {
      $batch['operations'][] = array('ooyala_taxonomy_term_sync_push_labels', array($vid, $sync_settings['olid']));
    }
  }

  batch_set($batch);
  $form_state['redirect'] = 'admin/config/media/ooyala/settings';
}


/**
 * Form alter code moved from ooyala_taxonomy.module for efficiency.
 */
function _ooyala_taxonomy_settings_form(&$form, &$form_state) {
  $form['ooyala_taxonomy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy vocabulary syncing'),
    '#description' => t('These options allow you to synchronize <a href="!taxonomy_url">Taxonomy vocabularies</a> with <a href="!ooyala_labels">Ooyala labels</a> or <a href="!ooyala_metadata">metadata</a>. This can be useful to maintain information within both Ooyala Backlot and Drupal. In order to synchronize labels, you must create a top-level label in Ooyala Backlot. Typically, vocabularies that have a limited number of items sync well with labels. Vocabularies that have a large number of items (such as a "Tags" vocabulary), are more appropriate to sync as metadata.', array('!taxonomy_url' => url('admin/structure/taxonomy'), '!ooyala_labels' => 'http://www.ooyala.com/www3/support_content_management#labels', '!ooyala_metadata' => 'http://www.ooyala.com/www3/support_content_management#metadata')),
    '#tree' => TRUE,
  );

  $vocabularies = (array) module_invoke('taxonomy', 'get_vocabularies');
  $api_active = ooyala_api_available();
  $problems = array();

  if (empty($vocabularies)) {
    $problems[] = t('There are currently no Taxonomy vocabularies available to synchronize.');
  }
  if (!$api_active) {
    $problems[] = t('The Ooyala API is currently not available. Check that your server can make HTTP requests.');
  }
  else {
    $labels = ooyala_api_label_list('<root>');
    if ($labels === FALSE) {
      $problems[] = t('The Ooyala Parter and Secret code could not be validated (you may need to save this page).');
    }
    elseif (count($labels) == 0) {
      $problems[] = t('Currently there are no labels defined within <a href="!ooyala_backlot">Ooyala Backlot</a>. At least one label needs to be created in order to synchronize it with a Taxonomy vocabulary.', array('!ooyala_backlot' => 'https://backlot.ooyala.com/backlot/web'));
    }
  }

  if (empty($problems)) {
    $options = array();
    foreach ($labels as $olid => $label) {
      $label = ltrim($label, '/');
      $options[$olid] = $label;
    }

    $form['ooyala_taxonomy']['vocabularies'] = array(
      '#theme' => 'ooyala_taxonomy_settings_table',
      '#tree' => TRUE,
    );
    $default_values = variable_get('ooyala_taxonomy', array());
    foreach ($vocabularies as $vocabulary) {
      $form['ooyala_taxonomy']['vocabularies'][$vocabulary->vid] = array(
        '#title' => check_plain($vocabulary->name),
      );
      $form['ooyala_taxonomy']['vocabularies'][$vocabulary->vid]['type'] = array(
        '#title' => t('Sync type'),
        '#type' => 'select',
        '#options' => array(
          '' => t('-- No syncing -- '),
          'labels' => t('Labels'),
          'metadata' => t('Metadata'),
        ),
        '#default_value' => isset($default_values[$vocabulary->vid]['type']) ? $default_values[$vocabulary->vid]['type'] : '',
      );
      $form['ooyala_taxonomy']['vocabularies'][$vocabulary->vid]['olid'] = array(
        '#title' => t('Sync under the top-level label') . ' ',
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($default_values[$vocabulary->vid]['olid']) ? $default_values[$vocabulary->vid]['olid'] : '',
        '#states' => array(
          'visible' => array(
            ':input[name="ooyala_taxonomy[vocabularies][' . $vocabulary->vid . '][type]"]' => array('value' => 'labels'),
          ),
        ),
      );
      $form['ooyala_taxonomy']['vocabularies'][$vocabulary->vid]['metadata_key'] = array(
        '#title' => t('Sync to metadata with the key') . ' ',
        '#type' => 'textfield',
        '#size' => 30,
        '#default_value' => isset($default_values[$vocabulary->vid]['metadata_key']) ? $default_values[$vocabulary->vid]['metadata_key'] : '',
        '#attributes' => array('placeholder' => 'tags, category, etc.'),
        '#states' => array(
          'visible' => array(
            ':input[name="ooyala_taxonomy[vocabularies][' . $vocabulary->vid . '][type]"]' => array('value' => 'metadata'),
          ),
        ),
      );
    }
    $form['ooyala_taxonomy_current'] = array(
      '#type' => 'value',
      '#value' => $default_values,
    );
  }
  else {
    $form['ooyala_taxonomy']['#description'] .= ' <strong>' . t('Taxonomy syncing cannot be enabled for the following reasons:') . '</strong>' . theme('item_list', array('items' => $problems));
  }

  // Add the additional submit handler needed for Taxonomy syncing.
  if (empty($problems)) {
    $form['#submit'][] = '_ooyala_taxonomy_settings_form_submit';
  }
}
/**
 * Theme the list of vocabularies to be synced as a table.
 */
function theme_ooyala_taxonomy_settings_table($variables) {
  $element = $variables['element'];

  $header = array(t('Vocabulary'), t('Sync type'), t('Ooyala Backlot target'));
  $rows = array();
  foreach (element_children($element) as $key) {
    $row = array();
    $row[] = $element[$key]['#title'];
    $element[$key]['type']['#title_display'] = 'invisible';
    $row[] = drupal_render($element[$key]['type']);
    $sync_row = '';
    $sync_row .= drupal_render($element[$key]['olid']);
    $sync_row .= drupal_render($element[$key]['metadata_key']);
    $row[] = array(
      'class' => array('container-inline'),
      'data' => $sync_row,
    );
    $rows[] = $row;
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Submit handler for ooyala_settings_form().
 *
 * Taxonomies need special form handling. All other settings are saved by the
 * standard system_settings_form_submit() function.
 */
function _ooyala_taxonomy_settings_form_submit($form, &$form_state) {
  $existing_vocabularies = $form_state['values']['ooyala_taxonomy_current'];
  $pending_vocabularies = array();
  $new_vocabularies = array();

  foreach ($form_state['values']['ooyala_taxonomy']['vocabularies'] as $vid => $vocabulary_values) {
    $olid = $vocabulary_values['olid'];
    $metadata_key = $vocabulary_values['metadata_key'];
    $label_sync_enabled = ($vocabulary_values['type'] === 'labels' && $olid);
    $metadata_sync_enabled = ($vocabulary_values['type'] === 'metadata' && $metadata_key);

    if ($label_sync_enabled || $metadata_sync_enabled) {
      // If syncing is already enabled, keep it enabled.
      if ($existing_vocabularies[$vid] == $vocabulary_values) {
        $new_vocabularies[$vid] = $vocabulary_values;
      }
      // Syncing is being enabled for the first time.
      else {
        // If this vocabulary has terms, add it to the list for batch operation.
        if ($label_sync_enabled && count(taxonomy_get_tree($vid, 0, NULL, FALSE))) {
          $pending_vocabularies[$vid]['olid'] = $olid;
        }
        // If the vocabulary is empty, just enable syncing immediately.
        else {
          $new_vocabularies[$vid] = $vocabulary_values;
        }
      }
    }
  }

  // Set the taxonomy variable immediately for existing or empty vocabularies.
  variable_set('ooyala_taxonomy', $new_vocabularies);

  // Set the redirect to enable vocabularies that have existing terms.
  if (!empty($pending_vocabularies)) {
    $form_state['redirect'] = array('admin/config/media/ooyala/sync-taxonomy', array('query' => array('vids' => $pending_vocabularies)));
  }
}
