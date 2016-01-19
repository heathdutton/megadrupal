<?php

/**
 * @file
 * Cloudwords queue management page.
 */

/**
 * Custom submit callback for marking a translatable out of date.
 */
function cloudwords_outdated_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['cloudwords_outdated'])) {
    foreach ($form_state['cloudwords_translatables'] as $translatable) {
      if ($translatable->translation_status == CLOUDWORDS_TRANSLATION_STATUS_TRANSLATION_EXISTS) {
        $translatable->translation_status = CLOUDWORDS_TRANSLATION_STATUS_OUT_OF_DATE;
        $translatable->save();
      }
    }
  }
}


function cloudwords_project_overview_scan_cancelled($form, &$form_state) {
  $projects = array();
  try {
    $projects = cloudwords_get_api_client()->get_closed_projects();
  }
  catch (CloudwordsApiException $e) {}

  $existing_ids = db_query("SELECT pid FROM {cloudwords_project} WHERE status NOT IN (:cancelled)", array(':cancelled' => cloudwords_project_closed_statuses()))->fetchCol();

  if ($projects && $existing_ids) {
    foreach ($projects as $project) {
      if (in_array($project->getId(), $existing_ids)) {
        $project->releaseContent();
        $project->setStatus($project->getStatus()->getCode());
      }
    }
  }
}

/**
 * Provides the overview page for Cloudwords.
 */
function cloudwords_project_overview_form($form, &$form_state) {

  $projects = FALSE;
  try {
    $projects = cloudwords_get_api_client()->get_open_projects();
  }
  catch (CloudwordsApiException $e) {}

  $existing_ids = db_query("SELECT pid FROM {cloudwords_project} WHERE status NOT IN (:canceled)", array(':canceled' => cloudwords_project_closed_statuses()))->fetchCol();

  $rows = array();

  if ($projects && $existing_ids) {
    foreach ($projects as $project) {
      if (in_array($project->getId(), $existing_ids)) {
        $params = $project->getParams();

        // Gather target languages. Wrap every 4 languages.
        $langs = array();
        $key = 0;
        foreach ($params['targetLanguages'] as $delta => $lang) {
          if ($delta % 4 === 0) {
            $key++;
          }
          $langs[$key][] = $lang['display'];
        }
        foreach ($langs as $delta => $lang) {
          $langs[$delta] = implode(', ', $langs[$delta]);
        }
        $target_language = implode('<br />', $langs);

        $row = array(
          'name' => l($params['name'], 'admin/structure/cloudwords/project/' . $params['id']),
          'status' => $params['status']['display'],
          'source_language' => isset($params['sourceLanguage']) ? $params['sourceLanguage']['display'] : '',
          'target_language' => $target_language,
        );

        $rows[] = $row;
      }
    }
  }

  $header = array(
    array('data' => t('Name'), 'field' => 'name'),
    array('data' => t('Status'), 'field' => 'status'),
    array('data' => t('Source language'), 'field' => 'source_language'),
    array('data' => t('Target languages'), 'field' => 'target_language', 'sort' => 'desc')    
  );


  $order = tablesort_get_order($header);
  $sort = tablesort_get_sort($header);

  if(isset($order['sql'])):
    $sql = $order['sql'];
    if($sort == 'desc') {
      usort($rows, function($a, $b) use($sql) {
          return strip_tags($a[$sql]) > strip_tags($b[$sql]) ? -1 : 1;
        }
      );
    }
    if($sort == 'asc') {
      usort($rows, function($a, $b) use ($sql) {
          return strip_tags($a[$sql]) < strip_tags($b[$sql]) ? -1 : 1;
        }
      );
    }
  endif;


  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'cloudwords-project-table'),
    '#empty' => t('No projects available.'),
  );

  return $form;
}

/**
 * Provides the overview page for Cloudwords.
 */
function cloudwords_project_closed_overview_form($form, &$form_state) {
  $form['rescan'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );

  $form['rescan']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Scan for cancelled projects in Cloudwords'),
    '#prefix' => '<div>' . t('This will check for cancelled projects and will release (make translatable) the content from those projects') . '</div>',
    '#submit' => array('cloudwords_project_overview_scan_cancelled'),
  );

  // Select table.
  $projects = db_select('cloudwords_project', 'cp')
    ->fields('cp', array('pid', 'name', 'status'))
    ->condition('status', cloudwords_project_closed_statuses())
    // ->extend('TableSort')
    ->extend('PagerDefault')
    ->limit(50)
    ->execute()
    ->fetchAll();

  $rows = array();

  foreach ($projects as $project) {
    $row = array();
    $row[] = l($project->name, 'admin/structure/cloudwords/project/' . $project->pid);

    switch ($project->status) {
      case 'cancelled':
        $row[] = t('Cancelled');
        break;

      case 'project_closed':
        $row[] = t('Closed');
        break;

      case 'drupal_cancelled':
        $row[] = t('Cancelled in Drupal');
        break;

      default:
        $row[] = t('Inactive');
        break;
    }
    $rows[] = $row;
  }

  $header = array(
    t('Name'),
    t('Status'),
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'cloudwords-project-table'),
    '#empty' => t('No closed projects.'),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
  );

  return $form;
}

/**
 * The project detail page.
 */
function cloudwords_project_form($form, &$form_state, CloudwordsDrupalProject $project) {
  if (is_null($project->getId())) {
    drupal_set_message(t('Project not found.'), 'warning');
    return array();
  }

  $form_state['cloudwords_project'] = $project;

  $action_statuses = array(
    'configured_project_name',
    'configured_project_details',
    'uploaded_source_materials',
    'configured_bid_options',
    'waiting_for_bid_selection',
    'bid_selection_expired',
  );

  if (in_array($project->getStatus()->getCode(), $action_statuses)) {
    drupal_set_message(t('Please finish <a href="!href" target="_blank">creating your project in Cloudwords</a>.', array('!href' => _cloudwords_ui_url() . '/cust.htm#project/' . $project->getId())), 'warning');
  }

  // Update project info.
  $project_info = array(
    'pid' => $project->getId(),
    'name' => $project->getName(),
    'status' => $project->getStatus()->getCode(),
  );
  drupal_write_record('cloudwords_project', $project_info, array('pid'));

  $client = cloudwords_get_api_client();

  // Project details.
  $form['project_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project Details'),
    '#tree' => TRUE,
  );
  $form['project_details']['metadata']['source_language'] = array(
    '#markup' => t('Source language') . ': ' . $project->getSourceLanguage()->getDisplay() . '<br />',
  );
  $form['project_details']['metadata']['status'] = array(
    '#markup' => t('Status') . ': ' . $project->getStatus()->getDisplay() . '<br />',
  );
  $form['project_details']['metadata']['description'] = array(
    '#markup' => t('Description') . ': ' . $project->getDescription() . '<br />',
  );
  $form['project_details']['metadata']['notes'] = array(
    '#markup' => t('Notes') . ': ' . $project->getNotes() . '<br />',
  );
  $form['project_details']['metadata']['delivery_due_date'] = array(
    '#markup' => t('Delivery due date') . ': ' . _cloudwords_format_display_date($project->getDeliveryDueDate()) . '<br />',
  );
  $options = array('attributes' => array('target' => '_blank'));
  $form['project_details']['metadata']['view'] = array(
    '#markup' => l(t('View Project in Cloudwords'), _cloudwords_ui_url() . '/cust.htm#project/' . $project->getId(), $options),
  );

  if ($project->isActive()) {
    $form['project_details']['metadata']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#submit' => array('cloudwords_cancel_project_redirect'),
    );
  }

  // Build language table.
  $files = array();
  try {
    $files = $client->get_project_translated_files($project->getId());
  }
  catch (CloudwordsApiException $e) {}

  $rows = array();
  foreach ($files as $file) {
    $lang_path = 'admin/structure/cloudwords/project/' . $project->getId() . '/' . $file->getLang()->getLanguageCode();
    $status_code = $file->getStatus()->getCode();

    $operations = array();
    if ($status_code != 'not_delivered') {
      $operations[] = l(t('Import'), $lang_path . '/import');
    }

    $language_status = $project->getLanguageImportStatus($file->getLang());

    $query = db_select('cloudwords_content', 'cc');
    $query->addJoin('INNER', 'cloudwords_translatable', 'ct', 'ct.ctid = cc.ctid');
    $query->condition('ct.language', cloudwords_map_cloudwords_drupal($file->getLang()->getLanguageCode()));
    $query->addExpression('COUNT(cc.ctid)', 'ncount');
    $total = $query->execute()->fetchField();
    $failed = $query->condition('cc.status', 3)->execute()->fetchField();

    if ($status_code == 'approved' && $language_status != CLOUDWORDS_LANGUAGE_FAILED) {
      $language_status = t('Imported');
    }
    elseif ($language_status == 1 && ($status_code == 'delivered' || $status_code == 'in_review')) {
      $language_status = t('Imported');
      $operations[] = l(t('Approve'), 'admin/structure/cloudwords/project/' . $project->getId() . '/' . $file->getLang()->getLanguageCode() . '/approve');
    }
    elseif ($language_status == CLOUDWORDS_LANGUAGE_APPROVED) {
      $language_status = t('Imported');
    }
    elseif ($language_status == CLOUDWORDS_LANGUAGE_FAILED) {
      $language_status = '<span class="marker">' . t('Failed') . '</span>';
    }
    else {
      $language_status = t('Not imported');
    }

    if ($failed) {
      $language_status = '<span class="marker">' . "$failed/$total failed" . '</span>';
    }

    if ($project->isDrupalCancelled()) {
      $operations = array();
    }

    $row = array(
      l($file->getLang()->getDisplay(), $lang_path),
      $file->getStatus()->getDisplay(),
      $language_status,
      implode(' | ', $operations),
    );
    $rows[] = $row;
  }
  $header = array(
    t('Name'),
    t('Status'),
    t('Import status'),
    t('Operations'),
  );

  $form['language_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Languages'),
    '#tree' => FALSE,
  );

  $form['language_wrapper']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'cloudwords-project-table'),
    '#empty' => t('No languages available.'),
  );

  // Build reference material table.
  try {
    $references = $client->get_project_references($project->getId());
  }
  catch (CloudwordsApiException $e) {
    $references = array();
  }
  $reference_rows = array();
  foreach ($references as $reference) {
    $ops = array(
      l(t('Replace'), 'admin/structure/cloudwords/file/' . $project->getId() . '/' . $reference->getId() . '/replace'),
      l(t('Download'), 'admin/structure/cloudwords/file/' . $project->getId() . '/' . $reference->getId() . '/download'),
    );

    $row = array(
      $reference->getFileName(),
      _cloudwords_format_display_date($reference->getCreatedDate()),
      implode(' | ', $ops),
    );
    $reference_rows[] = $row;
  }

  $form['reference_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project Reference Materials (Optional)'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => empty($reference_rows),
  );

  $form['reference_wrapper']['reference_table'] = array(
    '#theme' => 'table',
    '#header' => array(t('File name'), t('Date'), t('Operations')),
    '#rows' => $reference_rows,
    '#attributes' => array('id' => 'cloudwords-project-table'),
    '#empty' => t('No reference material available.'),
  );

  $form['reference_wrapper']['upload'] = array(
    '#type' => 'container',
    '#title' => t('Project reference material'),
    '#attributes' => array('class' => array('container-inline')),
    '#tree' => FALSE,
  );

  $form['reference_wrapper']['upload']['reference'] = array(
    '#type' => 'file',
    // '#title' => t('Project reference materials'),
    // '#description' => t('Upload additional reference materials.'),
    // '#theme' => 'feeds_upload',
    '#file_info' => NULL,
    '#size' => 10,
  );

  $form['reference_wrapper']['upload']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'cloudwords') . '/cloudwords.css';

  return $form;
}

/**
 * Submit callback that redirects to project cancel page.
 */
function cloudwords_cancel_project_redirect($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/cloudwords/project/' . $form_state['cloudwords_project']->getId() . '/cancel';
}

/**cloudwords_project_cancel_form
 * Confirmation form for canceling projects.
 */
function cloudwords_project_cancel_form($form, &$form_state, CloudwordsDrupalProject $project) {
  $form_state['cloudwords_project'] = $project;

  $question = t('Are you sure you want to cancel %project?', array('%project' => $project->getName()));
  $path = 'admin/structure/cloudwords/project/' . $project->getId();
  $description = t('Canceling this project will release the content attached to it and you will not be able to import any translated content. This will not cancel the proejct in Cloudwords.');

  return confirm_form($form, $question, $path, $description);
}

/**
 * Submit callback for cloudwords_project_cancel_form().
 */
function cloudwords_project_cancel_form_submit($form, &$form_state) {
  $project = $form_state['cloudwords_project'];
  $project->cancel();
  $form_state['redirect'] = 'admin/structure/cloudwords/project/' . $project->getId();
  drupal_set_message(t('%name was canceled successfully.', array('%name' => $project->getName())));
}

/**
 * Validate callback for cloudwords_project_form().
 */
function cloudwords_project_form_validate($form, &$form_state) {
  $upload_dir = 'private://cloudwords/reference_material';

  if (!file_prepare_directory($upload_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    form_set_error('upload][reference', t('Unable to create the upload directory.'));
  }

  if (!($file = file_save_upload('reference', array('file_validate_extensions' => array('zip')), $upload_dir))) {
    // form_set_error('upload][reference', t('Please upload a zip file.'));
  }
  else {
    $form_state['reference_material'] = $file;
  }
}

/**
 * Submit callback for cloudwords_project_form().
 */
function cloudwords_project_form_submit($form, &$form_state) {
  $project = $form_state['cloudwords_project'];

  if (!empty($form_state['reference_material'])) {
    cloudwords_get_api_client()->upload_project_reference($project->getId(), drupal_realpath($form_state['reference_material']->uri));
  }
}

/**
 * The language detail page for a project.
 */
function cloudwords_project_language_form($form, &$form_state, CloudwordsDrupalProject $project, CloudwordsLanguage $language) {
  $rows = array();

  $header_row = array(
    array('data' => 'Source name', 'field' => 'ct.label'),
    array('data' => 'Group', 'field' => 'ct.textgroup'),
    array('data' => 'type', 'field' => 'ct.type'),
    array('data' => 'Translation status', 'field' => 'ct.translation_status'),
    array('data' => 'Import status', 'field' => 'cc.status'),
  );

  $query = db_select('cloudwords_translatable', 'ct')
    ->fields('ct')
    ->condition('ct.language', cloudwords_map_cloudwords_drupal($language->getLanguageCode()));

  $query->addJoin('INNER', 'cloudwords_content', 'cc', 'ct.ctid = cc.ctid');
  $query->fields('cc', array('status'));
  $query->condition('cc.pid', $project->getId());

  $query
    ->extend('TableSort')
    ->orderByHeader($header_row)
    ->extend('PagerDefault')
    ->limit(5);

  $results = $query->execute();

  foreach ($results as $result) {
    $translatable = cloudwords_translatable_load($result->ctid);
    $row = array();

    $row['label'] = $translatable->editLink();
    $row['group'] = $translatable->textGroupLabel();
    $row['type'] = $translatable->typeLabel();
    $row['translation_status'] = $translatable->translationStatusLabel();

    switch ($result->status) {
      case CLOUDWORDS_LANGUAGE_NOT_IMPORTED:
        $status = t('Not imported');
        break;

      case CLOUDWORDS_LANGUAGE_IMPORTED:
        $status = t('Imported');
        break;

      case CLOUDWORDS_LANGUAGE_FAILED:
        $status = '<span class="marker">' . t('Import failed') . '</span>';
        break;
    }

    $row['status'] = $status;
    $rows[$translatable->ctid] = $row;
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header_row,
    '#rows' => $rows,
    '#attributes' => array('id' => 'cloudwords-project-table'),
    '#empty' => t('No content available.'),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
  );


  return $form;
}

function _cloudwords_project_language_import_publish_opts(CloudwordsDrupalProject $project, CloudwordsLanguage $language) {
  $valid_ctids = $project->getCtids(cloudwords_map_cloudwords_drupal($language->getLanguageCode()));
  $translatables = cloudwords_translatable_load_multiple($valid_ctids);

  $node_status = NULL;
  $options = array();

  foreach ($translatables as $translatable) {
    $translatable->type;
    if ($translatable->type == 'node_content') {
        $options[$translatable->ctid] = t('Publish translation for @name', array('@name' => $translatable->label));
    }
  }

  if ($options) {
    $node_status = array(
      '#type'           => 'checkboxes',
      '#title'          => t('Publishing options that will be applied after the import'),
      '#options'        => $options,
    );
  }

  return $node_status;
}

/**
 * Confirmation form for importing a delived language.
 */
function cloudwords_project_language_import_form($form, &$form_state, CloudwordsDrupalProject $project, CloudwordsLanguage $language) {
  $form['#attached']['js'][] = drupal_get_path('module', 'cloudwords') . '/cloudwords.js';
    
  $node_status_opts = _cloudwords_project_language_import_publish_opts($project, $language);

  if ($node_status_opts) {
    $form['select_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Select / Deselect all for publishing'),
    );    
    $form['node_status'] = $node_status_opts;
  }

  $form_state['cloudwords_project'] = $project;
  $form_state['cloudwords_language'] = $language;

  $question = t('Are you sure you want to import content for %lang?', array('%lang' => $language->getDisplay()));
  $path = 'admin/structure/cloudwords/project/' . $project->getId();

  $language_status = db_query("SELECT status FROM {cloudwords_project_language} WHERE pid = :pid AND language = :lang",
    array(':pid' => $project->getId(), ':lang' => $language->getLanguageCode())
  )->fetchField();
  $description = NULL;
  if ($language_status == 2) {
    $description = t('You have already imported content from this project and language. If you import again it will overwrite any existing content for this language.');
  }

  return confirm_form($form, $question, $path, $description);
}

/**
 * Submit callback for cloudwords_project_language_import_form().
 */
function cloudwords_project_language_import_form_submit($form, &$form_state) {
  $in_cron = (isset($form_state['in_cron'])) ? $form_state['in_cron'] : false;
  $project = $form_state['cloudwords_project'];
  $language = $form_state['cloudwords_language'];
  $node_statuses = isset($form_state['values']['node_status']) ? $form_state['values']['node_status'] : array();
  $node_statuses = array_filter($node_statuses);

  // Get translated bundles to map
  $translated_bundles = cloudwords_get_api_client()->get_translated_bundles($project->getId(), $language->getLanguageCode());

  // Download file and save as a temporary file.
  $file = cloudwords_get_api_client()->download_translated_file($project->getId(), $language->getLanguageCode());
  $temp = tempnam(file_directory_temp(), 'cloudwords-');
  file_put_contents($temp, $file);

  // Determine if tmp file is an xliff or a zip file
  if(is_resource(zip_open($temp))) {
     // Read zip file contents.
    $zip = new ArchiverZip($temp);
    $file_names_in_zip = $zip->listContents();

    $file_names = array();
    foreach($file_names_in_zip as $file_name_in_zip){
      // Three validation options:
      // Only include files, not directories or dotfiles
      // always allow 'xliff', 'xlf', 'xml' - check this only to skip running an extra valid xml test
      // validate files without extensions as some xliff clients will return valid xliff without an ext
      $file_name_parts = explode('/',$file_name_in_zip);
      $file_name = end($file_name_parts);
      
      $file_name_ext_text_arr = explode(".", strtolower($file_name));
      $file_ext = end($file_name_ext_text_arr);
      
      if(substr($file_name_in_zip, -1) == '/' || preg_match('/^([.])/', $file_name)){
        continue;
      }elseif(in_array($file_ext, array('xliff', 'xlf', 'xml'))){
        $file_names[] = $file_name_in_zip;
      }elseif(strpos($file_name,'.') == false){
        libxml_use_internal_errors(TRUE);
        if (simplexml_load_string(_cloudwords_filter_xml_control_characters(file_get_contents('zip://' . $temp . '#' . $file_name)))) {
          $file_names[] = $file_name_in_zip;
        }
        libxml_clear_errors();
        libxml_use_internal_errors(false);
      }
    }
    
    $archive = $zip->getArchive();
    $isArchive = true;
  }else if(simplexml_load_string(_cloudwords_filter_xml_control_characters(file_get_contents($temp)))){
    $file_names = array($temp);
    $isArchive = false;
  } 

  if ($file_names) {
    $batch = array(
      'title' => t('Importing files ...'),
      'operations' => array(),
      'init_message' => t('Loading files'),
      'progress_message' => t('Processed @current out of @total files.'),
      'error_message' => t('An error occurred during processing.'),
      'finished' => 'cloudwords_import_batch_finished',
      'progressive' => FALSE,
      'file' => drupal_get_path('module', 'cloudwords') . '/cloudwords.pages.inc',
    );

    foreach ($file_names as $file_name) {
      //we need to fetch the translated document based on the filename
      $translated_bundle = _cloudwords_get_bundle_by_filename($file_name, $translated_bundles);

      $batch['operations'][] = array('cloudwords_import_batch', array($isArchive, $temp, $file_name, $project, $language, $node_statuses, $translated_bundle, $in_cron));   
    }

    batch_set($batch);
  }
}

/**
 * Extracts one xml file from a zip file and imports it.
 */
function cloudwords_import_batch($isArchive, $temp_file, $file_name, $project, $language, $node_statuses, $translated_bundle, $in_cron, &$context) {
  if (!isset($context['results']['processed'])) {
    $context['results']['processed'] = 0;
    $context['results']['temp_file'] = $temp_file;
    $context['results']['project'] = $project;
    $context['results']['language'] = $language;
    $context['results']['processed_translatables'] = array();    
    $context['results']['successful'] = 0;
    $context['results']['failed'] = 0;
    $context['results']['in_cron'] = $in_cron;
  }

  $serializer = new CloudwordsFileformatXLIFF();
  if($isArchive == true){
    $file = 'zip://' . $temp_file . '#' . $file_name;
  }else{
    $file = $file_name;
  }
  if ($serializer->validateImport($project, $language, $file)) {
    $valid_ctids = $project->getCtids(cloudwords_map_cloudwords_drupal($language->getLanguageCode()));
    $imported = $serializer->import($file);
    $to_be_imported = array_intersect($valid_ctids, array_keys($imported));

    $translatables = cloudwords_translatable_load_multiple($to_be_imported);

    foreach ($imported as $ctid => $data) {

      if (!in_array($ctid, $to_be_imported)) {
        $context['results']['success'] = FALSE;
        $context['results']['failed']++;
        drupal_set_message(t('%file contains an invalid id: %id', array('%file' => $file, '%id' => $ctid)), 'error');
      }
      else {
        $translatable = $translatables[$ctid];
        try {  
          
          $translatable->setSetting('node_status', (int) isset($node_statuses[$ctid]));
          $translatable->saveData($data);
          $translatable->translation_status = CLOUDWORDS_TRANSLATION_STATUS_TRANSLATION_EXISTS;
          $translatable->translated_document_id = $translated_bundle['id'];
          $translatable->last_import = time();
          $translatable->save();
          $translatable->setProjectTranslationStatus($project, CLOUDWORDS_LANGUAGE_IMPORTED);
          
          if(variable_get('cloudwords_preview_bundle_enabled', TRUE) != FALSE){          
            _cloudwords_import_translation_preview_bundle($project, $translatable, $translated_bundle);
          }
          
          $context['results']['successful']++;
          $context['results']['processed_translatables'][] = $translatable;
          $context['message'] = t('Importing %label (%type).', array('%label' => $translatable->label, '%type' => $translatable->typeLabel()));
        }
        catch (Exception $e) {
          drupal_set_message(t($e->getMessage()), 'error');
          $context['results']['failed']++;
          $translatable->setProjectTranslationStatus($project, CLOUDWORDS_LANGUAGE_FAILED);
        }
        $context['results']['processed']++;
      }
    }
  }
  else {
    drupal_set_message(t('%file failed to validate.', array('%file' => $file)), 'error');
    $context['results']['success'] = FALSE;
    $project->setLanguageImportStatus($language, CLOUDWORDS_LANGUAGE_FAILED);
  }
}

function _cloudwords_import_translation_preview_bundle($project, $translatable, $translated_bundle){

  if($translatable->type == 'node_content' && isset($translated_bundle['id'])){
    $source_path = 'node/'.$translatable->objectid;
    //$translation_paths = translation_path_get_translations($source_path);
    //$url = $translation_paths[$translatable->language];  
    $url = drupal_get_path_alias('node/'.$translatable->objectid, $translatable->language);   
    $pathObjectId = $translatable->objectid;
    
    //Initial import path availability is flakey with source nid so query for exact new entity id
    $result = db_query('SELECT n.nid FROM {node} n WHERE n.tnid = :tnid and n.language = :language', 
                      array(':tnid' => $translatable->objectid,
                            ':language' => $translatable->language,
                      ));    
    $record = $result->fetchObject(); 
    if(isset($record->nid)){
      $url = drupal_get_path_alias('node/'.$record->nid, $translatable->language);
      $pathObjectId = $record->nid;
    }

    $node = node_load($translatable->objectid);
     
    $language_code = $translatable->language;
  
    $scraper = new CloudwordsPreviewScraper($project->getName(), $language_code, $node->nid, $pathObjectId, $url);
    if($zip_file_path = $scraper->get_zip_file()){
      // Upload the translation preview zip archive.
      try{
        cloudwords_get_api_client()->upload_translation_preview_bundle($project->getId(), $language_code, $translated_bundle['id'], $zip_file_path);
      }
      catch (Exception $e) {
        drupal_set_message(t($e->getMessage()), 'error');
        watchdog('cloudwords', t($e->__toString()));
        return;
      }
    }
  }
}

/**
 * Finished callback for batch importing.
 *
 * @todo Cleanup after error.
 */
function cloudwords_import_batch_finished($success, $results, $operations, $time) {
  $results += array('success' => TRUE);

  if ($success) {
    if ($results['successful']) {
      $message = format_plural($results['successful'],
        'Saved %count item.',
        'Saved %count items.',
        array('%count' => $results['successful'])
      );
      drupal_set_message($message);
    }

    if ($results['failed']) {
      $message = format_plural($results['failed'],
        'Failed saving %count item.',
        'Failed saving %count items.',
        array('%count' => $results['failed'])
      );
      drupal_set_message($message, 'error');
    }
    
    //negate unpublished node access granted to scraper
    node_access_rebuild(true);
  }

  // This means something blew up. bail.
  else {
    return;
  }

  if ($results['success']) {
    $results['project']->setLanguageImportStatus($results['language'], CLOUDWORDS_LANGUAGE_IMPORTED);
  }
  else {
    $results['project']->setLanguageImportStatus($results['language'], CLOUDWORDS_LANGUAGE_FAILED);
  }

  // Delete the temporary zip file.
  if (!empty($results['temp_file'])) {
    file_unmanaged_delete($results['temp_file']);
  }
  
  if ($results['in_cron'] == false) {
    drupal_goto('admin/structure/cloudwords/project/' . $results['project']->getId());
  }
}

/**
 * Project language approval form.
 */
function cloudwords_project_language_approve_form($form, &$form_state, CloudwordsDrupalProject $project, CloudwordsLanguage $language) {
  $form_state['cloudwords_project'] = $project;
  $form_state['cloudwords_language'] = $language;

  $question = t('Are you sure you want to approve %lang?', array('%lang' => $language->getDisplay()));
  $path = 'admin/structure/cloudwords/project/' . $project->getId();
  $description = t('Approving a language tells your translation vendor the content has been accepted and does not need further work. This action cannot be undone.');
  return confirm_form($form, $question, $path, $description);
}

/**
 * Submit callback for cloudwords_project_language_approve_form().
 */
function cloudwords_project_language_approve_form_submit($form, &$form_state) {
  $project = $form_state['cloudwords_project'];
  $language = $form_state['cloudwords_language'];

  cloudwords_get_api_client()->approve_project_language($project->getId(), $language->getLanguageCode());

  $project->approve($language);

  drupal_set_message(t('%name was successfully approved.', array('%name' => $project->getName())));

  $form_state['redirect'] = 'admin/structure/cloudwords/project/' . $project->getId();
}

/**
 * Replaces an existing reference file with a new one.
 */
function cloudwords_file_replace_form($form, &$form_state, CloudwordsDrupalProject $project, CloudwordsFile $file) {
  drupal_set_title(t('Replace %file', array('%file' => $file->getFilename())), PASS_THROUGH);
  $form_state['cloudwords_project'] = $project;
  $form_state['cloudwords_file'] = $file;

  $form['reference'] = array(
    '#type' => 'file',
    '#title' => t('Project reference materials'),
    '#description' => t('Upload additional reference materials as a zip file.'),
    // '#theme' => 'feeds_upload',
    '#file_info' => NULL,
    '#size' => 10,
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/cloudwords/project/' . $project->getId()),
    '#weight' => '1000',
  );

  return $form;
}

/**
 * Validate callback for cloudwords_file_replace_form().
 */
function cloudwords_file_replace_form_validate($form, &$form_state) {
  $upload_dir = 'private://cloudwords/reference_material';

  if (!file_prepare_directory($upload_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    form_set_error('upload][reference', t('Unable to create the upload directory.'));
  }

  if (!($file = file_save_upload('reference', array('file_validate_extensions' => array('zip')), $upload_dir))) {
    // form_set_error('upload][reference', t('Please upload a zip file.'));
  }
  else {
    $form_state['reference_material'] = $file;
  }
}

/**
 * Submit callback for cloudwords_file_replace_form().
 */
function cloudwords_file_replace_form_submit($form, &$form_state) {
  if (!empty($form_state['reference_material'])) {
    $project = $form_state['cloudwords_project'];
    $file = $form_state['cloudwords_file'];
    $path = drupal_realpath($form_state['reference_material']->uri);
    $return = cloudwords_get_api_client()->update_project_reference($project->getId(), $file->getId(), $path);
    drupal_set_message(t('File %old was replaced with %new.', array('%old' => $file->getFilename(), '%new' => $return->getFilename())));
    $form_state['redirect'] = 'admin/structure/cloudwords/project/' . $project->getId();
  }
}

/**
 * Page callback that serves a file downloaded from Cloudwords.
 */
function cloudwords_file_download(CloudwordsDrupalProject $project, CloudwordsFile $file) {
  $filename = $file->getFilename();
  $filedata = cloudwords_get_api_client()->download_reference_file($project->getId(), $file->getId());
  header('Content-Disposition: attachment; filename=' . basename($filename));
  header('Content-Type: application/force-download');
  header('Content-Type: application/octet-stream');
  header('Content-Type: application/download');
  header('Content-Description: File Transfer');
  header('Content-Length: ' . strlen($filedata));
  echo $filedata;
  die();
}

/**
 * Formats a Cloudwords date string.
 *
 * @param string $date
 *   A date string from the Cloudwords REST API.
 *
 * @return string
 *   A date string formatted m/d/Y.
 */
function _cloudwords_format_display_date($date) {
  try {
    $datetime = new DateTime($date);
    $date = format_date($datetime->format('U'), 'custom', 'm/d/Y', 'UTC');
  }
  catch (Exception $exc) {
    $date = '';
  }

  return $date;
}

/**
 * Content - change language form.
 *
 * @param string $nids_list
 *   A string containing node ids separated by commas.
 *
 */
function cloudwords_nodes_set_language_form($form, &$form_state, $nids_list) {
  $nids = array_map('_cloudwords_nodes_set_language_filter_nid', explode(',', $nids_list));

  $node_titles = array();
  $nodes = array();
  foreach ($nids as $key => $nid) {
    $node = node_load($nid);
    if ($node) {
      $nodes[] = $node;
      $node_titles[] = check_plain($node->title);
    }
    else {
      unset($nids[$key]);
    }
  }

  $languages = language_list();
  $lang_opts = array(LANGUAGE_NONE => 'Language neutral');

  foreach ($languages as $key => $lang) {
    $lang_opts[$key] = $lang->name;
  }

  $form['language'] = array(
    '#title' => t('Language'),
    '#description' => t('Select the language for<br /><i>' . implode("<br />", $node_titles) . '</i>'),
    '#type' => 'select',
    '#options' => $lang_opts,
  );

  $question = t('Select a language.');
  $path = 'admin/content';

  $form_state['nodes_to_update'] = $nodes;
  return confirm_form($form, $question, $path);
}

function _cloudwords_nodes_set_language_filter_nid($nid) {
  $nid = trim($nid);
  if (!is_numeric($nid)) {
    unset($nid);
  }
  else {
    return $nid;
  }
}

/**
 * Saving the updates for each node that meets the language requirements.
 */
function cloudwords_nodes_set_language_form_submit($form, &$form_state) {
  $nodes = $form_state['nodes_to_update'];
  $language = $form_state['values']['language'];

  foreach ($nodes as $node) {
    _cloudwords_nodes_set_language_single_node_operation($node, $language);
  }

  $form_state['redirect'] = 'admin/content';
}

/**
 * Operation to be performed on node
 */
function _cloudwords_nodes_set_language_single_node_operation($node, $language){
  $skip = FALSE;
  $translated = FALSE;
  $in_cloudwords = FALSE;

  if (!empty($node->tnid)) {
    if ($language == LANGUAGE_NONE) {
      $skip = TRUE;
      $translated = TRUE;
    } else {
      $translations = translation_node_get_translations($node->tnid);
      foreach ($translations as $langcode => $translation) {
        if ($translation->nid == $node->nid) {
          $skip = TRUE;
          $translated = TRUE;
          break;
        }
      }
    }
  }

  $translatables = cloudwords_get_translatables_by_property(array(
    'type' => 'node_content',
    'textgroup' => 'node',
    'objectid' => !empty($node->tnid) ? $node->tnid : $node->nid,
  ), 'language');

  $disabled_langs = array();
  foreach ($translatables as $langcode => $translatable) {
    if ($translatable->status != CLOUDWORDS_QUEUE_NOT_IN_QUEUE || $translatable->uid != 0) {
      $disabled_langs[] = $langcode;
      $in_cloudwords = TRUE;
    }
  }

  if ($in_cloudwords && ($language == LANGUAGE_NONE || in_array($language, $disabled_langs))) {
    $skip = TRUE;
  } else {
    $in_cloudwords = FALSE;
  }

  if ($skip) {
    if ($language == LANGUAGE_NONE) {
      $reason = ($translated ? t('it has translations') : '') . ($translated && $in_cloudwords ? t('and') : '') . ($in_cloudwords ? t('it is active in cloudwords') : '');
      drupal_set_message(t('Could not set <i>@title</i> to <i>Language neutral</i> because @reason', array('@title' => $node->title, '@reason' => $reason)), 'warning');
    } else {
      $reason = ($translated ? t('it is already translated in chosen language') : '') . ($translated && $in_cloudwords ? t('and') : '') . ($in_cloudwords ? t('it is marked for translation in cloudwords') : '');
      drupal_set_message(t('Could not update <i>@title</i> because @reason', array('@title' => $node->title, '@reason' => $reason)), 'warning');
    }
    return;
  }

  $node->language = $language;
  node_save($node);
}

/*
 * Implementation of node_operations form
 */
function cloudwords_translation_status_mass_update_form($form, &$form_state, $nids_list) {
  $nids = array_map('_cloudwords_nodes_set_status_nid', explode(',', $nids_list));

  $node_titles = array();
  $nodes = array();
  foreach ($nids as $key => $nid) {
    $node = node_load($nid);
    if ($node) {
      $nodes[] = $node;
      $node_titles[] = check_plain($node->title);
    }
    else {
      unset($nids[$key]);
    }
  }

  $statuses = cloudwords_exists_options_list();
  $status_opts = array(null => t('Select one'));

  foreach ($statuses as $key => $value) {
    $status_opts[$key] = $value;
  }

  $form['status'] = array(
    '#title' => t('status'),
    '#description' => t('Select the status for<br /><i>' . implode("<br />", $node_titles) . '</i>'),
    '#type' => 'select',
    '#options' => $status_opts,
  );

  $question = t('Select a status.');
  $path = 'admin/content';

  $form_state['nodes_to_update'] = $nodes;
  return confirm_form($form, $question, $path);

}

function _cloudwords_nodes_set_status_nid($nid) {
  $nid = trim($nid);
  if (!is_numeric($nid)) {
    unset($nid);
  }
  else {
    return $nid;
  }
}

/**
 * node_operation form submit
 */
function cloudwords_translation_status_mass_update_form_submit($form, &$form_state) {
  $nodes = $form_state['nodes_to_update'];
  $status = $form_state['values']['status'];

  foreach ($nodes as $node) {
    _cloudwords_translation_status_mass_update_single_node_operation($node, $status);
  }

  $form_state['redirect'] = 'admin/content';
}

/**
 * Operation to be performed on node
 * Saving the manually set translation status.
 */
function _cloudwords_translation_status_mass_update_single_node_operation($node, $status){
  $not_applicable = TRUE;

  $translatables = cloudwords_get_translatables_by_property(array(
    'type' => 'node_content',
    'textgroup' => 'node',
    'objectid' => !empty($node->tnid) ? $node->tnid : $node->nid,
    'language' => $node->language
  ), 'language');

  foreach ($translatables as $langcode => $translatable) {
    $translatable->translation_status = $status;
    $translatable->save();
    $not_applicable = FALSE;
  }

  if ($not_applicable) {
    $reason = t('is not in translatable table');
    drupal_set_message(t('Could not change translation status on <i>@title</i> because @reason', array('@title' => $node->title, '@reason' => $reason)), 'warning');
  }
  return;
}