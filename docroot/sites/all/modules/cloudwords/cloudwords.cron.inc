<?php

/**
 * @file
 * Cron for cloudwords.module.
 */

function _cloudwords_cron(){
  _cloudwords_cron_release_closed_project_content();
  _cloudwords_cron_auto_import_translation_preview_bundle(); 
  _cloudwords_cron_auto_project_creation_of_out_of_date_translatables();
}

function _cloudwords_cron_auto_import_translation_preview_bundle(){

  $enabled = variable_get('cloudwords_auto_import_translation_enabled', false);
  if($enabled != true){
    return false;
  }

  $frequency_setting = variable_get('cloudwords_auto_import_translation_frequency', '180')  * 60;
  $runnable_after = variable_get('cloudwords_auto_import_translation_last_cron_run', REQUEST_TIME) + $frequency_setting;
  if($runnable_after && ($runnable_after > REQUEST_TIME)){
    return false;
  }

  module_load_include('inc', 'cloudwords', 'cloudwords.pages');
  
  $projects = cloudwords_get_api_client()->get_open_projects();

  $existing_ids = db_query("SELECT pid FROM {cloudwords_project} WHERE status NOT IN (:cancelled)", array(':cancelled' => cloudwords_project_closed_statuses()))->fetchCol();

  $importables = array();
  
  foreach ($projects as $project) {
    if (in_array($project->getId(), $existing_ids)) {
      $params = $project->getParams();
      foreach ($params['targetLanguages'] as $delta => $lang) {
        $language = new CloudwordsLanguage($lang);
        $translated_bundles = array();
        try {
          $translated_bundles = cloudwords_get_api_client()->get_translated_bundles($project->getId(), $language->getLanguageCode());
        } catch (CloudwordsApiException $e) {}
          
        if($translated_bundles){
          $valid_ctids = $project->getCtids(cloudwords_map_cloudwords_drupal($language->getLanguageCode()));
          $translatables = cloudwords_translatable_load_multiple($valid_ctids);
         
          foreach($translatables as $translatable){
            foreach($translated_bundles as $translated_bundle){
              if(!isset($translatable->last_import) || 
                (isset($translatable->last_import) && $translatable->last_import < strtotime($translated_bundle['xliff']['createdDate']))){
                  
                $importables[$project->getId().'_'.$language->getLanguageCode()] = array(
                 'project' => $project,
                 'language' => $language
                );
                
                // TODO deal with single translatable at a time  
                //_cloudwords_cron_auto_import_translation_preview_bundle_cron_import_xliff($project, $language, $translatable, $translated_bundle);
              }
            }
          }
        }
      }
    }
  }

  foreach($importables as $importable){
    $form_state = array();
    $form_state['cloudwords_project'] = $importable['project'];
    $form_state['cloudwords_language'] = $importable['language'];    
    $form_state['in_cron'] = true;
    $form_state['values']['op'] = 'Submit';
    $form_state['values']['submit'] = 'Submit';
    drupal_form_submit('cloudwords_project_language_import_form', $form_state, $importable['project'], $importable['language']);  
  }

  variable_set('cloudwords_auto_import_translation_last_cron_run', REQUEST_TIME);
}

/*
 *  Imports single document to a translatable at a time.  
 *  //TODO
 */
function _cloudwords_cron_auto_import_translation_preview_bundle_cron_import_xliff($project, $language, $translatable, $translated_bundle){
  module_load_include('inc', 'cloudwords', 'cloudwords.pages');
  $file = cloudwords_get_api_client()->get_translated_document_by_id($project->getId(), $language->getLanguageCode(), $translated_bundle['id']);     
  $temp = tempnam(file_directory_temp(), 'cloudwords-');
  file_put_contents($temp, $file);
  $serializer = new CloudwordsFileformatXLIFF();

  if (simplexml_load_string(_cloudwords_filter_xml_control_characters(file_get_contents($temp))) && 
  $serializer->validateImport($project, $language, $temp)) {
    $imported = $serializer->import($temp);
    
    foreach ($imported as $ctid => $data) {
      try {                      
        $translatable->setSetting('node_status', (int) isset($node_statuses[$ctid]));
        $translatable->saveData($data);
        $translatable->translation_status = CLOUDWORDS_TRANSLATION_STATUS_TRANSLATION_EXISTS;
        $translatable->translated_document_id = $translated_bundle['id'];
        $translatable->last_import = time();
        $translatable->save();
        $translatable->setProjectTranslationStatus($project, CLOUDWORDS_LANGUAGE_IMPORTED);
        
        _cloudwords_import_translation_preview_bundle($project, $translatable, $translated_bundle);
      }
      catch (Exception $e) {
        drupal_set_message(t($e->getMessage()), 'error');
        $translatable->setProjectTranslationStatus($project, CLOUDWORDS_LANGUAGE_FAILED);
      }
    }                  
  }  
}


function _cloudwords_cron_auto_project_creation_of_out_of_date_translatables(){
  module_load_include('inc', 'cloudwords', 'cloudwords.create_project');
  //auto create projects of grouped content for out of date translatables if enabled
 
  $auto_project_creation_enabled = variable_get('cloudwords_auto_project_creation_enabled', FALSE);  
  if($auto_project_creation_enabled != true){
    return false;
  }

  // Get list of translatables that are flagged as out of date and are NOT currently in a project
  $properties = array('translation_status' => 2, 'status' => 0);
  $translatables = cloudwords_get_translatables_by_property($properties);

  //this must be run as a particular user to simulate the UI addition of ctids to a project
  global $user;
  $original_user = $user;
  $old_state = drupal_save_session();
  drupal_save_session(FALSE);
  $user = user_load(1);

  // group by language
  $langCtids = array();
  foreach($translatables as $translatable){
    $langCtids[$translatable->language][] = $translatable->ctid;   
  }
  
  foreach($langCtids as $langCode => $ctids){
    $form_state = array();    
    $form_state['values']['project_name'] = t('auto_!langCode_!date', 
      array('!langCode' => $langCode, 
            '!date' => date('Y_m_d_H_i_s',time()
            )
           )
      );

    $form_state['in_cron'] = true;
    $form_state['ctids'] = $ctids;
    $form_state['values']['op'] = 'Submit';
    $form_state['values']['submit'] = 'Submit';   

    drupal_form_submit('cloudwords_project_create_form', $form_state);  
  }

  $user = $original_user;
  drupal_save_session($old_state);
}

function _cloudwords_cron_release_closed_project_content(){
  $last = REQUEST_TIME - variable_get('cloudwords_last_cron_run', REQUEST_TIME);

  $last = $last / 3600;

  if ($last && $last < 24) {
    return;
  }

  $projects = cloudwords_get_api_client()->get_open_projects();

  $existing_ids = db_query("SELECT pid FROM {cloudwords_project} WHERE status NOT IN (:cancelled)", array(':cancelled' => cloudwords_project_closed_statuses()))->fetchCol();

  foreach ($projects as $project) {
    if (in_array($project->getId(), $existing_ids)) {

      $project_info = array(
        'pid' => $project->getId(),
        'name' => $project->getName(),
        'status' => $project->getStatus()->getCode(),
      );
      drupal_write_record('cloudwords_project', $project_info, array('pid'));

      foreach ($project->getTargetLanguages() as $language) {
        try {
          $files = cloudwords_get_api_client()->get_project_translated_files($project->getId());

          // Approve languages that were not previously approved.
          foreach ($files as $file) {
            if ($file->getStatus()->getCode() == 'approved' &&
                $project->getLanguageImportStatus($language) != CLOUDWORDS_LANGUAGE_APPROVED) {
              $project->approve($language);
            }
          }
        }

        catch (CloudwordsApiException $e) {}
      }
    }
  }

  // release the content for closed projects
  $closed_projects = array();
  try {
    $closed_projects = cloudwords_get_api_client()->get_closed_projects();
  }
  catch (CloudwordsApiException $e) {}

  if ($closed_projects && $existing_ids) {
    foreach ($closed_projects as $project) {
      if (in_array($project->getId(), $existing_ids)) {
        $project->releaseContent();
        $project->setStatus($project->getStatus()->getCode());
      }
    }
  }

  variable_set('cloudwords_last_cron_run', REQUEST_TIME);
}