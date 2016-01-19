<?php

/**
 * The file extension to use for XLIFF files.
 */
define('CLOUDWORDS_XLIFF_EXTENSION', 'xlf');

/**
 * Project creation form.
 *
 * This acts as the configuration form for cloudwords_project_create.
 */
function cloudwords_project_create_form($form, &$form_state) {

  $client = cloudwords_get_api_client();
  $departments = $client->get_departments();

  $private_files = variable_get('file_private_path', FALSE);

  if (!$private_files) {
    drupal_set_message(t('The <a href="@url">private file system</a> path must be configured before you can create a project.', array('@url' => url('admin/config/media/file-system'))), 'error');
  }
  unset($_SESSION['cloudwords_project']);
  $default_value = '';
  if (!empty($form_state['project'])) {
    $default_value = $form_state['project']->getName();
  }
  elseif (!empty($_SESSION['cloudwords_project'])) {
    $default_value = $_SESSION['cloudwords_project']->getName();
    $form_state['project'] = $_SESSION['cloudwords_project'];
  }

  $form['project_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 50,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#description' => t('What do you want to call this project?'),
    '#default_value' => $default_value,
    '#disabled' => !$private_files,
  );

  if($departments){
    $options = array();
    foreach($departments as $department){
      $options[$department['id']] = $department['name'];
    }
    $form['department'] = array(
      '#type' => 'select',
      '#title' => t('Department'),
      '#required' => TRUE,
      '#description' => t('Select a department'),
      '#options' => $options,
    );
  }

  $form['reference'] = array(
    '#type' => 'file',
    '#title' => t('Project reference materials'),
    '#description' => t('Upload additional reference materials as a zip file.'),
    '#file_info' => NULL,
    '#size' => 10,
    '#disabled' => !$private_files,
  );

  $languages = cloudwords_language_list();
  $info = cloudwords_translatable_info();

  $rows = array();
  $map = array();
  $added = array();
  global $user;
  if ($cache = cloudwords_project_user_get($user)) {
    $translatables = db_select('cloudwords_translatable', 'ct')
      ->fields('ct', array('ctid'))
      ->condition('ctid', $cache)
      // ->extend('TableSort')
      ->extend('PagerDefault')
      ->limit(50)
      ->execute()
      ->fetchCol();

    $translatables = cloudwords_translatable_load_multiple($translatables);

    // Map each object to a list of languages.
    foreach ($translatables as $translatable) {
      $type_info = $info[$translatable->type];
      $map[$translatable->type][$translatable->objectid][] = $languages[$translatable->language]->name;
    }

    foreach ($translatables as $translatable) {
      if (!isset($added[$translatable->type][$translatable->objectid])) {
        $type_info = $info[$translatable->type];

        // Gather target languages. Wrap every 4 languages.
        $langs = array();
        $key = 0;
        foreach ($map[$translatable->type][$translatable->objectid] as $delta => $lang) {
          if ($delta % 4 === 0) {
            $key++;
          }
          $langs[$key][] =  $lang;
        }
        foreach ($langs as $delta => $lang) {
          $langs[$delta] = implode(', ', $langs[$delta]);
        }
        $targetLanguage = implode('<br />', $langs);

        // Build row.
        $rows[] = array(
          $translatable->editLink(),
          $targetLanguage,
          $translatable->textGroupLabel(),
          $translatable->typeLabel(),
          $translatable->translationStatusLabel(),
        );

        // Mark that we've added this id.
        $added[$translatable->type][$translatable->objectid] = TRUE;
      }
    }
  }

  $header = array(
    t('Source name'),
    t('Language'),
    t('Group'),
    t('Type'),
    t('Translation exists'),
  );

  if ($rows) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#disabled' => !$private_files,
    );
  }

  $form['table_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Review content'),
  );

  $form['table_wrapper']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'cloudwords-project-table'),
    '#empty' => t('Nothing added to project.'),
  );
  $form['pager'] = array(
    '#theme' => 'pager',
  );

  if ($rows) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#disabled' => !$private_files,
    );
  }

  return $form;
}

/**
 * Validate callback for cloudwords_project_create_form().
 */
function cloudwords_project_create_form_validate($form, &$form_state) {
  $form_state['values']['project_name'] = trim($form_state['values']['project_name']);
  
  if (( strpos($form_state['values']['project_name'], '/') !== FALSE) 
   || (strpos($form_state['values']['project_name'], '\\') !== FALSE) ) {
    form_set_error('project_name', t('The project name cannot contain slashes.'));    
  }

  $query = db_query("SELECT name FROM {cloudwords_project} WHERE name = :name", array(':name' => $form_state['values']['project_name']));
  $result = $query->fetchObject();
  if (isset($result->name)) {
    form_set_error('project_name', t('The project name already exists.  Please create a unique project name.'));
  }   
  
  $upload_dir = 'private://cloudwords/reference_material';
  if (!file_prepare_directory($upload_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    form_set_error('reference', t('Unable to create the upload directory.'));
  }

  // If there is a file uploaded, save it.
  if (!empty($_FILES['files']['name']['reference'])) {
    if (!($file = file_save_upload('reference', array('file_validate_extensions' => array('zip')), $upload_dir))) {
      form_set_error('reference', t('Please upload a zip file.'));
    }
    else {
      $form_state['reference_material'] = $file;
    }
  }
}

/**
 * Submit callback for cloudwords_project_create_form().
 *
 * Returning the $form_state here allows us to access the form values in the
 * batch.
 */
function cloudwords_project_create_form_submit($form, &$form_state) {
  $in_cron = (isset($form_state['in_cron'])) ? $form_state['in_cron'] : false;
  
  $project = FALSE;
  $args = array('!url' => url('admin/config/services/cloudwords'));
  $client = cloudwords_get_api_client();

  try {
    if (empty($form_state['project'])) {
      $error_message = 'There was an error creating the project. Please check that your <a href="!url">settings</a> are correct.';
      $params = array(
        'name' => $form_state['values']['project_name'],
        'notes' => 'The content for this project has been generated by the Cloudwords for Drupal integration. Please read the following to understand how to properly translate this content: https://cloudwords.zendesk.com/entries/23056462',
        'sourceLanguage' => 'en',
      	'projectContentType' => 'Drupal',
      	'uiFeatures' => array(
      	   'change_source_language' => false,
      	   'change_target_languages' => false,
      	   'change_source_material' => true,
      	   'clone_project' => false,
        ),
      );
      if(isset($form_state['values']['department'])){
        $params['department'] = array('id'=> (int) $form_state['values']['department']);
      }

      $project = $client->create_project($params);
    }
    else {
      $project = $form_state['project'];
    }
    if (!empty($form_state['reference_material'])) {
      $error_message = 'There was uploading the reference material. Please try again.';

      $client->upload_project_reference($project->getId(), drupal_realpath($form_state['reference_material']->uri));
    }
  }
  catch (CloudwordsApiException $e) {
    watchdog('cloudwords', t($e->__toString()));
    drupal_set_message(t($error_message, $args), 'error');
    $form_state['rebuild'] = TRUE;

    // Stash the project so that we can re-use it.
    $form_state['project'] = $project;
    return;
  }

  $_SESSION['cloudwords_project'] = $project;

  $project_info = array(
    'pid' => $project->getId(),
    'name' => $project->getName(),
    'status' => $project->getStatus()->getCode(),
  );

  if (db_query("SELECT pid FROM {cloudwords_project} WHERE pid = :pid", array(':pid' => $project->getId()))->fetchField()) {
    drupal_write_record('cloudwords_project', $project_info, array('pid'));
  }
  else {
    drupal_write_record('cloudwords_project', $project_info);
  }

  db_delete('cloudwords_content')
    ->condition('pid', $project->getId())
    ->execute();
  db_delete('cloudwords_project_language')
    ->condition('pid', $project->getId())
    ->execute();

  $batch = array(
    'title' => t('Building project ...'),
    'operations' => array(),
    'init_message' => t('Loading items to be processed'),
    'progress_message' => t('Saving items as XML.'),
    'error_message' => t('An error occurred during processing.'),
    'finished' => 'cloudwords_project_finished',
    'file' => drupal_get_path('module', 'cloudwords') . '/cloudwords.create_project.inc',
  );

  global $user;

  if(isset($form_state['ctids'])){
    $cache = $form_state['ctids'];
  }else if($ctids = cloudwords_project_user_get($user)){
    $cache = $ctids;
  }
  
  if ($cache) {
  
    $batch['operations'][] = array('cloudwords_project_start', array($project, $in_cron));
    foreach (array_chunk($cache, CLOUDWORDS_BATCH_SIZE) as $ctids) {
      $batch['operations'][] = array('cloudwords_project_create_batch_serialize', array($ctids, $project));
    }
    $batch['operations'][] = array('cloudwords_project_create_finilize_files', array($project));
    $batch['operations'][] = array('cloudwords_project_create_update_target_languages', array($project));
    $batch['operations'][] = array('cloudwords_project_create_upload_zip', array($project));
    $batch['operations'][] = array('cloudwords_project_create_update_reviewer_instructions', array($project));
    
    if(variable_get('cloudwords_preview_bundle_enabled', TRUE) != FALSE){
      foreach (array_chunk($cache, CLOUDWORDS_BATCH_SIZE) as $ctids) {
        foreach($ctids as $ctid){
          $batch['operations'][] = array('cloudwords_project_create_source_preview_bundle', array($project, $ctid));
        }
      }
    }

    foreach (array_chunk($cache, CLOUDWORDS_BATCH_SIZE) as $ctids) {
      foreach($ctids as $ctid){
        $batch['operations'][] = array('cloudwords_project_upload_source_preview_bundle', array($project, $ctid));
      }
    }

    $batch['operations'][] = array('cloudwords_project_create_project_reference_materials', array($project));

  }

  batch_set($batch);
}

/**
 * Creates a project via rest call.
 */
function cloudwords_project_start(CloudwordsDrupalProject $project, $in_cron, &$context) {
  $context['results']['in_cron'] = $in_cron;
  // Prepare directory.
  $destination = 'public://cloudwords/' . $project->getName();
  if (!file_prepare_directory($destination, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    drupal_set_message(t('Files directory either cannot be created or is not writable.'), 'error');
    throw new Exception(t('Files directory either cannot be created or is not writable.'));
  }
}

function _cloudwords_file_name($project, $translatable) {
  return $project->getName() . '-' .
         $project->getSourceLanguage()->getLanguageCode() . '-' .
         $translatable->cloudwordsLanguage() . '-' . 
         $translatable->textgroup . '-' . 
         $translatable->objectid;
}

/**
 * Callback for the cloudwords_project_create action.
 */
function cloudwords_project_create_batch_serialize($ctids, CloudwordsDrupalProject $project, &$context) {

  $translatables = cloudwords_translatable_load_multiple($ctids);

  $serializer = new CloudwordsFileformatXLIFF();
  $destination = 'public://cloudwords/' . $project->getName();

  if (!isset($context['results']['files'])) {
    $context['results']['files'] = array();
  }
  
  if (!isset($context['results']['translable_bundles'])) {
    $context['results']['translable_bundles'] = array();
  }
  
  $file_storage =& $context['results']['files'];

  foreach ($translatables as $translatable) {
    $language = $translatable->language;

    // Find a file to append to.
    // First run.
    if (!isset($file_storage[$language][$translatable->objectid])) {
      $file = $destination . '/' . _cloudwords_file_name($project, $translatable) . '-1.' . CLOUDWORDS_XLIFF_EXTENSION;
      file_put_contents($file, $serializer->beginExport($project, $translatable));
      $file_storage[$language][$translatable->objectid][] = $file;
    }
    else {
      $i = 1;
      $existing_file = FALSE;
      foreach ($file_storage[$language][$translatable->objectid] as $file) {
        // If the file is smaller than 1 megabyte.
        if (filesize($file) < (1048576 * variable_get('cloudwords_upload_file_size', 50))) {
          $existing_file = $file;
          break;
        }
        $i++;
      }

      if (!$existing_file) {
        // The existing files are too large. We need to create a new one.
        $file = $destination . '/' . _cloudwords_file_name($project, $translatable) . '-' . $i . '.' . CLOUDWORDS_XLIFF_EXTENSION;
        file_put_contents($file, $serializer->beginExport($project));
        $file_storage[$language][$translatable->objectid][] = $file;
      }
    }

    // Save our content to project map.
    $map = (object) array(
      'pid' => $project->getId(),
      'ctid' => $translatable->ctid,
    );
    drupal_write_record('cloudwords_content', $map);

    // Indicate that this target language is in use.
    $context['results']['target_languages'][$translatable->language] = $translatable->cloudwordsLanguage();

    // Append this translatable.
    $output = $serializer->exportTranslatable($translatable);
    file_put_contents($file, $output, FILE_APPEND | LOCK_EX);
 
    // Mark this translatable as "in project".
    $translatable->status = CLOUDWORDS_QUEUE_IN_PROJECT;
    $translatable->save();

    $context['results']['translable_bundles'][] = array('file_name' => $file, 'translatable' => $translatable);

    // Count the number processed.
    if (empty($context['results']['processed'])) {
      $context['results']['processed'] = 0;
      $context['results']['project'] = $project;
    }
    $context['results']['processed']++;
  }
}

function cloudwords_project_create_finilize_files(CloudwordsDrupalProject $project, &$context) {
  $serializer = new CloudwordsFileformatXLIFF();
  $destination = 'public://cloudwords/' . $project->getName();

  // Find our xml files.
  $valid_files = array();
  if ($items = @scandir($destination)) {
    foreach ($items as $item) {
      if (is_file("$destination/$item") && strpos($item, '.') !== 0) {
        $valid_files[] = "$destination/$item";
      }
    }
  }

  foreach ($valid_files as $file) {
    file_put_contents($file, $serializer->endExport($project), FILE_APPEND | LOCK_EX);
  }
}

/**
 * Batch callback to update a project's target language.
 */
function cloudwords_project_create_update_target_languages(CloudwordsDrupalProject $project, &$context) {
  // Update the target languages.
  $params = array(
    'id' => $project->getId(),
    'name' => $project->getName(),
    'targetLanguages' => array_values($context['results']['target_languages']),
    'sourceLanguage' => 'en',
  );
  try {
    cloudwords_get_api_client()->update_project($params);
  }
  catch (Exception $e) {
    drupal_set_message(t($e->getMessage()), 'error');
    throw $e;
  }
}

function cloudwords_project_create_upload_zip(CloudwordsDrupalProject $project, &$context) {
  // Create zip archive.
  $archiver = new ZipArchive();
  $zip_file = drupal_realpath('public://cloudwords/' . $project->getName() . '.zip');
  $destination = 'public://cloudwords/' . $project->getName();

  if ($archiver->open($zip_file, ZIPARCHIVE::CREATE || ZIPARCHIVE::OVERWRITE) !== TRUE) {
    return FALSE;
  }

  // Find our xml files.
  $valid_files = array();
  if ($items = @scandir($destination)) {
    foreach ($items as $item) {
      if (is_file("$destination/$item") && strpos($item, '.') !== 0) {
        $valid_files[] = "$destination/$item";
      }
    }
  }

  foreach ($valid_files as $file) {
    $archiver->addFromString(basename($file), file_get_contents($file));
  }
  $archiver->close();

  // Upload the source materials zip archive.
  try{
    cloudwords_get_api_client()->upload_project_source($project->getId(), $zip_file);
  }
  catch (Exception $e) {
    drupal_set_message(t($e->getMessage()), 'error');
    throw $e;
  }

  $context['results']['project'] = $project;
}

/**
 * Batch callback to update a project's reviewer instructions.
 */
function cloudwords_project_create_update_reviewer_instructions(CloudwordsDrupalProject $project, &$context) {
  global $base_url;
  $language_list = language_list();

  foreach($context['results']['target_languages'] as $target_language){
    //text for target langauage
    $language = $language_list[$target_language];
    $project_url = $base_url.'/admin/structure/cloudwords/project/'.$project->getId();
    $content = 'To review the ' . $language->name . ' translation, please follow these steps:
1. Go to the project page in Drupal: ' . $project_url . '
2. For ' . $language->name . ', click Import.
3. Review the translated content in Drupal:
';
    $translatables = $project->getTranslatables($target_language);
    foreach($translatables as $translatable){
      $content .= '
';
      $content .= $base_url.'/node/'.$translatable->objectid.'/translate';
    }

    try {
       cloudwords_get_api_client()->create_reviewer_instruction($project->getId(), $target_language, $content);
    }
    catch (Exception $e) {
      drupal_set_message(t($e->getMessage()), 'error');
      throw $e;
    }
  }
}

/**
 * Batch callback to create archives for a project's in context preview.
 */
function cloudwords_project_create_source_preview_bundle(CloudwordsDrupalProject $project, $ctid, &$context) {

  if(!isset($context['results']['translable_bundles'])){
    return;
  }

  if (!isset($context['results']['source_bundle_documents'])) {
    $context['results']['source_bundle_documents'] = array();
  }  

  if (!isset($context['results']['source_bundle_archives'])) {
    $context['results']['source_bundle_archives'] = array();
  }  

  // Get source bundles to map
  $source_bundles = cloudwords_get_api_client()->get_source_bundle($project->getId());

  foreach($context['results']['translable_bundles'] as $translable_bundle){
    //get source nodes 
    if($translable_bundle['translatable']->type == 'node_content' && $translable_bundle['translatable']->ctid == $ctid){
      $file_name = basename($translable_bundle['file_name']);
      $file_name = str_replace(' ','_',$file_name);

      $source_bundle = _cloudwords_get_bundle_by_filename($file_name, $source_bundles);

      $node = node_load($translable_bundle['translatable']->objectid);
      $url = 'node/'.$node->nid;    
      $language_code = $node->language;
      
      //only create the archive once
      if(!isset($context['results']['source_bundle_documents'][$node->nid])){

        $scraper = new CloudwordsPreviewScraper($project->getName(), $language_code, $node->nid, $node->nid, $url);
        $zip_file_path = $scraper->get_zip_file();
        //$zip_file_path = cloudwords_preview_generate_archive_from_path($project->getName(), $language_code, $node->nid, $url);
        $context['results']['source_bundle_documents'][$node->nid] = $zip_file_path;
      }else{
        $zip_file_path = $context['results']['source_bundle_documents'][$node->nid];
      }
      
      $context['results']['source_bundle_archives'][$ctid] = array('document_id' => $source_bundle['id'], 'zip_file_path' => $zip_file_path);
    
    }
  }
  
  //$context['results']['project'] = $project;
}

/**
 * Batch callback to upload archives for a project's in context preview.
 */
function cloudwords_project_upload_source_preview_bundle(CloudwordsDrupalProject $project, $ctid, &$context) {

  if(isset($context['results']['source_bundle_archives'][$ctid])){
    $document_id = $context['results']['source_bundle_archives'][$ctid]['document_id'];
    $zip_file_path = $context['results']['source_bundle_archives'][$ctid]['zip_file_path'];
    try{
      cloudwords_get_api_client()->upload_source_preview_bundle($project->getId(), $document_id, $zip_file_path);
    }
    catch (Exception $e) {
      drupal_set_message(t($e->getMessage()), 'error');
      watchdog('cloudwords', t($e->__toString()));
      return;
    }
  }
  
}

/**
 * Batch callback to add automatically generated reference materials
 */
function cloudwords_project_create_project_reference_materials(CloudwordsDrupalProject $project, &$context) {
  if (!isset($context['results']['translable_bundles'])) {
    return;
  }
  $lines = array();

  foreach($context['results']['translable_bundles'] as $translable_bundle){
    $objectid = $translable_bundle['translatable']->objectid;
    $language = $translable_bundle['translatable']->language;
    $i = $objectid.$language;
    $lines[$i][] = $translable_bundle['translatable']->label;
    $file_name = explode('/', $translable_bundle['file_name']);
    $lines[$i][] = end($file_name);
    $lines[$i][] = ($translable_bundle['translatable']->textgroup == 'node') ? url('node/'.$objectid, array('absolute' => TRUE)): '';
  }

  $file_content = 'label,filename,url'. PHP_EOL;
  foreach($lines as $line){
    $file_content .= implode(',', $line). PHP_EOL;
  }
  
  $upload_dir = 'private://cloudwords/reference_material/';
  $manifest_file_path = $upload_dir.'/'.$project->getName().'-manifest';
 
  $archiver = new ZipArchive();
  $zip_file = drupal_realpath($manifest_file_path . '.zip');
  if ($archiver->open($zip_file, ZIPARCHIVE::OVERWRITE) !== TRUE) {
    return FALSE;
  }
  //$archiver->addFromString($project->getName().'-manifest.csv', file_get_contents($manifest_file_path.'.csv'));
  $archiver->addFromString($project->getName().'-manifest.csv', $file_content);
  $archiver->close();

  $client = cloudwords_get_api_client();  
  $client->upload_project_reference($project->getId(), drupal_realpath($manifest_file_path . '.zip'));
}

/**
 * Finished callback.
 */
function cloudwords_project_finished($success, $results, $operations, $time) {
  if ($success) {
    $message = format_plural($results['processed'],
      'Uploaded %count item.',
      'Uploaded %count items.',
      array('%count' => $results['processed'])
    );
    drupal_set_message($message);

    unset($_SESSION['cloudwords_project']);

    global $user;
    db_update('cloudwords_translatable')
      ->condition('uid', $user->uid)
      ->fields(array(
        'uid' => 0,
      ))
      ->execute();
      
    //negate unpublished node access granted to scraper  
    node_access_rebuild(true);
    if ($results['in_cron'] == false) {
      drupal_goto('admin/structure/cloudwords/project/' . $results['project']->getId());
    }
  }
  else {
    // drupal_goto('admin/structure/cloudwords/project/' . $results['project']->getId());
  }
}
