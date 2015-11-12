<?php

/**
 * @file
 * Functions/callbacks used to administer the Entity XLIFF FTP module.
 */


/**
 * Primary Drupal settings form callback.
 *
 * @see entity_xliff_ftp_menu()
 */
function entity_xliff_ftp_settings($form, &$form_state) {

  $form['sftp_details'] = array(
    '#title' => t('SFTP details'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['sftp_details']['entity_xliff_ftp_host'] = array(
    '#title' => t('SFTP hostname'),
    '#description' => t('Used to make an SFTP connection. Do not include a protocol.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('entity_xliff_ftp_host', ''),
  );

  $form['sftp_details']['entity_xliff_ftp_un'] = array(
    '#title' => t('SFTP username'),
    '#description' => t('Used when connecting to the remote server.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('entity_xliff_ftp_un', ''),
  );

  $form['sftp_details']['entity_xliff_ftp_pw'] = array(
    '#title' => t('SFTP password'),
    '#description' => t('Used when connecting to the remote server.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('entity_xliff_ftp_pw', ''),
  );

  $form['sftp_details']['entity_xliff_ftp_target_root'] = array(
    '#title' => t('XLIFF target root path'),
    '#description' => t('Used as the root directory when pushing XLIFFs to the remote server.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('entity_xliff_ftp_target_root', ''),
  );

  $form['sftp_details']['entity_xliff_ftp_source_root'] = array(
    '#title' => t('XLIFF source root path'),
    '#description' => t('Used as the root directory when pulling XLIFFs from the remote server.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('entity_xliff_ftp_source_root', ''),
  );

  $form['sftp_details']['entity_xliff_ftp_file_prefix'] = array(
    '#title' => t('XLIFF file prefix'),
    '#description' => t('Used to uniquely identify this Drupal instance from other Drupal instances when sharing the same FTP source/target roots. Note: this value should not be changed while projects are still pending.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('entity_xliff_ftp_file_prefix', ''),
  );

  return system_settings_form($form);
}

/**
 * Drupal settings form callback for Entity Xliff FTP overview page.
 *
 * @see entity_xliff_ftp_menu()
 */
function entity_xliff_ftp_overview($form, &$form_state) {
  $languages = language_list();
  if (isset($form_state['storage']['confirm_process_action'])) {
    // Define which entities are to be processed (both in storage and in the UI)
    $toBeProceessed = array();
    foreach ($form_state['input']['processable'] as $entityType => $entities) {
      foreach ($entities as $entityId => $shouldBeProcessed) {
        if ($shouldBeProcessed) {
          try {
            $wrapper = entity_metadata_wrapper($entityType, $entityId);
          }
          catch (EntityMetadataWrapperException $e) {
            watchdog('entity xliff ftp', 'Error attempting to display %entity %entity_id in the projects process confirmation form.', array(
              '%entity' => $entityType,
              '%entity_id' => $entityId,
            ), WATCHDOG_ERROR);
            continue;
          }

          $langs = $form_state['storage']['processable'][$entityType][$entityId];
          $langs = array_map(function($item) use ($languages) {
            return $languages[$item]->name;
          }, $langs);
          $toBeProceessed[] = t('Updates: !count to %label (!langs).', array(
            '!count' => count($langs),
            '%label' => $wrapper->label(),
            '!langs' => implode(', ', $langs),
          ));

          // Store the entities to be processed.
          $targetLangs = $form_state['storage']['processable'][$entityType][$entityId];
          $form_state['storage']['to_be_processed'][$entityType][$entityId] = array_map(function($item) use ($languages) {
            return $languages[$item];
          }, $targetLangs);
        }
      }
    }

    // Print out the entities to be processed.
    $form['to_be_processed'] = array(
      '#theme' => 'item_list',
      '#items' => $toBeProceessed,
      '#title' => t('Projects to be processed'),
      '#type' => 'ul',
      '#attributes' => array(),
      '#suffix' => '<br />',
    );
    $form =  confirm_form($form, t('Are you sure you want to process the following projects?'), current_path(), NULL, t('Process'));
    $form['actions']['submit']['#submit'] = array('entity_xliff_ftp_overview_process_submit');
    return $form;
  }

  drupal_set_title(t('Remote file sync overview'));

  $form['project_overview'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => t('You may also be looking to !configure_integration.', array(
      '!configure_integration' => l(t('configure remote server integration'), 'admin/config/services/entity-xliff-ftp'),
    )),
  );

  // "Pending" projects.
  $form['pending_overview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pending projects'),
    '#description' => t('An overview of pending projects. Each row represents a piece of content that is awaiting Drupal import.'),
    '#group' => 'project_overview',
  );

  // Load in pending/processable projects.
  $form_state['storage']['processable'] = entity_xliff_ftp_get_processable();

  // Prepare the table.
  $header = array('', t('Label'), t('Type'), t('Bundle'), t('Identifier'), t('Pending languages'));
  $rows = array();
  foreach ($form_state['storage']['processable'] as $entityType => $entityDetails) {
    foreach ($entityDetails as $entityId => $langs) {
      if ($entity = entity_load_single($entityType, (int) $entityId)) {
        $wrapper = entity_metadata_wrapper($entityType, $entity);
        $uri = entity_uri($entityType, $entity);
        $langs = array_map(function ($item) use ($languages) {
          return $languages[$item]->name;
        }, $langs);

        $checkbox = array(
          '#type' => 'checkbox',
          '#name' => 'processable[' . implode('][', array(
              $entityType,
              $entityId
            )) . ']',
        );

        $rows[] = array(
          'checkbox' => drupal_render($checkbox),
          'label' => l($wrapper->label(), $uri['path'], $uri['options']),
          'type' => $wrapper->type(),
          'bundle' => $wrapper->getBundle(),
          'identifier' => $entityId,
          'languages' => implode(', ', $langs),
        );
      }
      else {
        watchdog('entity xliff ftp', 'Error attempting to display %entity %entity_id in the pending projects overview pane.', array(
          '%entity' => $entityType,
          '%entity_id' => $entityId,
        ), WATCHDOG_WARNING);
      }
    }
  }

  // Add the table to be rendered.
  if (count($rows)) {
    $form['pending_overview']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  else {
    $form['pending_overview']['table'] = array(
      '#type' => 'markup',
      '#markup' => '<p><em>' . t('There are no pending projects at this time.') . '</em></p>',
    );
  }

  $form['pending_overview']['process'] = array(
    '#type' => 'submit',
    '#value' => t('Process selected projects'),
    '#submit' => array('entity_xliff_ftp_overview_process_submit'),
    // Disabled by default. Enabled when any one processable item is checked.
    '#disabled' => count($rows) <= 0,
    '#states' => array(
      'enabled' => array(
        ':input[name^="processable"]' => array('checked' => TRUE),
      ),
      'disabled' => array(
        ':input[name^="processable"]' => array('checked' => FALSE),
      ),
    ),
  );

  // "Processed" projects.
  $form['processed_overview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Processed projects'),
    '#description' => t('An overview of recently processed XLIFFs.'),
    '#group' => 'project_overview',
  );

  // Load in processed projects.
  $form_state['storage']['processed'] = entity_xliff_ftp_get_processed();

  // Prepare the table.
  $header = array(t('Label'), t('Language'), t('Processed'));
  $rows = array();

  foreach ($form_state['storage']['processed'] as $entityType => $entityDetails) {
    foreach ($entityDetails as $entityId => $languageDetails) {
      foreach ($languageDetails as $language => $fileDetails) {
        if ($entity = entity_load_single($entityType, $entityId)) {
          $wrapper = entity_metadata_wrapper($entityType, $entity);
          $uri = entity_uri($entityType, $entity);
          $processed = new DateObject($fileDetails['accessed']);

          $rows[$fileDetails['accessed'] . '-' . $wrapper->label()] = array(
            'label' => l($wrapper->label(), $uri['path'], $uri['options']),
            'language' => $languages[$language]->name,
            'processed' => date_format_date($processed),
          );
        }
        else {
          watchdog('entity xliff ftp', 'Error attempting to display %entity %entity_id in the processed projects overview pane.', array(
            '%entity' => $entityType,
            '%entity_id' => $entityId,
          ), WATCHDOG_WARNING);
        }
      }
    }
  }

  // Latest ones first, only the latest 50 though.
  krsort($rows);
  $rows = array_slice($rows, 0, 50);

  // Add the table to be rendered.
  if (count($rows)) {
    $form['processed_overview']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  else {
    $form['processed_overview']['table'] = array(
      '#type' => 'markup',
      '#markup' => '<p><em>' . t('There are no processed projects at this time.') . '</em></p>',
    );
  }

  // Add a message that indicates when processing last occurred.
  $lastProcessed = variable_get('entity_xliff_ftp_last_processed', 0);
  $lastProcessed = new DateObject($lastProcessed);
  $form['processed_overview']['table']['#prefix'] = '<p>' . t('Processing last occurred @date.', array(
      '@date' => date_format_date($lastProcessed),
    )) . '</p>';

  return $form;
}

function entity_xliff_ftp_overview_process_submit($form, &$form_state) {
  // If the form has not yet been confirmed, force a rebuild w/confirm form.
  if (!isset($form_state['storage']['confirm_process_action'])) {
    $form_state['storage']['confirm_process_action'] = TRUE;
    $form_state['rebuild'] = TRUE;
  }
  // Otherwise, process selected projects.
  else {
    $toBeProcessed = $form_state['storage']['to_be_processed'];
    foreach ($toBeProcessed as $entityType => $entities) {
      foreach ($entities as $entityId => $targetLangs) {
        entity_xliff_ftp_set_entity_xliffs($entityType, $entityId, $targetLangs);
      }
    }
    variable_set('entity_xliff_ftp_last_processed', REQUEST_TIME);
  }
}

/**
 * @see entity_xliff_ftp_form_entity_xliff_actions_alter()
 */
function _entity_xliff_ftp_form_entity_xliff_actions_alter(&$form, &$form_state) {
  // Do not show this on non-English pages for now. Currently, we only support
  // exporting XLIFFs for English. @todo
  $node = menu_get_object('node');
  if ($node->language !== 'en' && $node->language !== LANGUAGE_NONE) {
    return;
  }

  // Create a fieldset space for ourselves.
  $form['entity_xliff_ftp'] = array(
    '#title' => t('Remote file sync integration'),
    '#description' => '<p>' . t('Select languages into which you want to translate this page (defaults to all languages).') . '</p>' .
      '<p>' . t('Once selected, click %import. This will generate XLIFF files for the selected languages and push them to the remote server.', array(
        '%import' => t('Push to remote server'),
      )) . '</p>',
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -5,
  );

  $languages = language_list('language');
  unset($languages[$GLOBALS['language']->language]);
  $form['entity_xliff_ftp']['languages'] = array(
    '#title' => t('Target language(s)'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => array_map(function($lang) {return $lang->name;}, $languages),
    '#default_value' => array_keys($languages),
  );

  $form['entity_xliff_ftp']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Push to remote server'),
    '#submit' => array('entity_xliff_ftp_push_xliffs'),
  );
}

/**
 * Submit handler for pushing XLIFFs to the remote server through the UI.
 *
 * @see _entity_xliff_ftp_form_entity_xliff_actions_alter()
 */
function entity_xliff_ftp_push_xliffs($form, &$form_state) {
  if (isset($form_state['values']['languages'])) {
    // Filter down to just the selected languages.
    $selectedLangs = $form_state['values']['languages'];
    $langs = array_filter(language_list('language'), function ($lang) use ($selectedLangs) {
      return isset($selectedLangs[$lang->language]) && !empty($selectedLangs[$lang->language]);
    });

    // Get entity type and data.
    $type = $form_state['build_info']['args'][0];
    $entity = $form_state['build_info']['args'][1];

    // Push selected languages to the remote server.
    entity_xliff_ftp_put_entity_xliffs($type, $entity, $langs);
  }
}
