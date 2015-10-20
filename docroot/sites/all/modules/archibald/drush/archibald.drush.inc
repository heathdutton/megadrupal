<?php

/**
 * @file
 *   drush integration for Archibald 2
 */

require_once dirname(__FILE__) . "/../includes/Lom.class.php";
require_once dirname(__FILE__) . "/../includes/LomSaveHandler.class.php";

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @see drush_parse_command()
 *   for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function archibald_drush_command() {
  $items = array();
  $items['archibald_test'] = array(
    'description' => "test archibald lom class",
    'arguments' => array(
      'action' => 'the test action (' .
      'import_demo, ' .
      'json_export, ' .
      'json_export_100, ' .
      'load, ' .
      'insert, ' .
      'set_env_api)',
      'id' => 'if loading then use lomid',
    ),
    'callback' => 'drush_archibald_test',
  );

  $items['archibald_sync_taxonomy'] = array(
    'description' => 'sync taxonomys with csv or ontology Server file. ' .
    "paramters (" .
    "csv, " .
    "lom-v1, " .
    "lom-v1-installation, " .
    "educa_standard_curriculum)",
    'arguments' => array(),
    'callback' => 'drush_archibald_taxonomy_sync',
  );

  $items['archibald_reset_upload_err'] = array(
    'description' => 'files with are marked as faulty while upload, ' .
    'would be resettet, so that these can be retried ',
    'arguments' => array(),
    'callback' => 'drush_archibald_reset_upload_errors',
  );

  $items['archibald_reset_curriculum'] = array(
    'description' => "re run setup of curriculum scripts ",
    'arguments' => array(
      'curriculum' => 'enter here curriculum machine_name, ' .
      'by default it will process all',
    ),
    'callback' => 'drush_archibald_resetup_curriculum',
  );

  $items['archibald_sync_coverages'] = array(
    'description' => "synchronize coverages with thesaurie systems. " .
    "swd or rameau",
    'arguments' => array(
      'type' => 'Coverage type (swd|rameau)',
      'charset' => 'The charset (iso5426|utf-8)',
      'file' => 'The path to the coverage file',
      'nocleanup' => 'provide 1 to not truncate previous keywords',
    ),
    'callback' => 'drush_archibald_import_coverages',
  );

  $items['archibald_solr_index'] = array(
    'description' => "index outstanding lom objectsa in solr",
    'callback' => 'drush_archibald_solr_index',
  );

  $items['archibald_fix_server_type'] = array(
    'description' => "fix server type multiple entries",
    'callback' => 'drush_archibald_fix_server_type',
  );

  $items['archibald_purge_deleted'] = array(
    'description' => "Delete marked items permanently",
    'callback' => 'drush_archibald_purge_deleted',
  );

  $items['archibald_delete_from_solr'] = array(
    'description' => "Delete given lom_id from SOLR",
    'arguments' => array(
      'lom_id' => 'LOM ID'
    ),
    'callback' => 'drush_archibald_delete_from_solr',
  );

  $items['archibald_test_mmi'] = array(
    'description' => "Test MetaMetadataIdentifiers",
    'callback' => 'drush_archibald_test_mmi',
  );

  $items['archibald_set_api'] = array(
    'description' => "Decide wich API to use",
    'callback' => 'drush_archibald_set_api',
  );

  $items['archibald_get_api'] = array(
    'description' => "Show which API is in use",
    'callback' => 'drush_archibald_get_api',
  );

  return $items;
}

define('ARCHIBALD_DRUSH_JSON_PATH', dirname(__FILE__) . '/../data/lom_demo_data/');

/**
 * kind of unit test and other developer stuff

 *
 * @param string $action
 *  import_demo: import test resources from ./data/lom_demo_data/ folder
 *  json_export: export test resources to ./data/lom_demo_data/ folder,
 *               for sharing it with other developers
 *  json_export_100: create 100 nearly empty lom resources and
 *                   save them to ./data/lom_demo_data/ folder
 *  load: print_r() a single lom resource by given lom_id
 *  insert: insert a complete lom resource to test save handler
 *  set_env_api: if you have a fresh database copy of a live system,
 *               you can use this to set all options to central api test system
 * @param string $lom_id
 *  only used for "load" action
 */
function drush_archibald_test() {
  $args   = func_get_args();
  $action = @$args[0];
  $lom_id = @$args[1];

  switch ($action) {
    case 'import_demo':
      if (!empty($lom_id)) {
        if (is_file(ARCHIBALD_DRUSH_JSON_PATH . '/' . $lom_id)) {
          $data = json_decode(file_get_contents(realpath(ARCHIBALD_DRUSH_JSON_PATH . '/' . $lom_id)));
          $lom = ArchibaldLomSaveHandler::jsonImport($data);
          ArchibaldLomSaveHandler::insert($lom);
        }
      }
      else {
        $image_list = archibald_demo_data_get_image_list();

        $d = dir(ARCHIBALD_DRUSH_JSON_PATH);
        while (FALSE !== ($entry = $d->read())) {
          if (is_file($d->path . '/' . $entry)) {
            $data = json_decode(
              file_get_contents(realpath($d->path . '/' . $entry))
            );

            if (!empty($data->technical->preview_image)) {
              $image_ids = array_keys($image_list);
              $image_id = $image_ids[rand(0, count($image_ids) - 1)];
              $data->technical->preview_image = 'archibald_file/' . $image_id . '/' . $image_list[$image_id];
            }
            $lom = ArchibaldLomSaveHandler::jsonImport($data);
            ArchibaldLomSaveHandler::insert($lom);

            drush_log('Import ArchibaldLom: ' . $entry, 'ok');
          }
        }
        $d->close();
      }
      break;

    case 'json_export':
      $lom = archibald_load($lom_id);

      if (is_file(ARCHIBALD_DRUSH_JSON_PATH . '/' . $lom->getLomId())) {
        drush_log('Export ArchibaldLom update: ' . $lom_id, 'ok');
      }
      else {
        drush_log('Export ArchibaldLom new: ' . $lom_id, 'ok');
      }
      file_put_contents(
        ARCHIBALD_DRUSH_JSON_PATH . '/' . $lom->getLomId(), ArchibaldLomSaveHandler::jsonExport($lom)
      );
      break;

    case 'json_export_100':
      $lom = ArchibaldLomSaveHandler::load($lom_id);

      for ($i = 1; $i <= 100; $i++) {
        $general = $lom->getGeneral();

        $lang_string = new ArchibaldLomDataLangString();
        $lang_string->setString("Test Description automatic $i", "en");
        $lang_string->setString("Test Description automatisch $i", "de");

        $general->setTitle($lang_string);

        $lang_string = new ArchibaldLomDataLangString();
        $lang_string->setString(
          "Test Description automatic $i Duis leo lorem, " .
          "cursus quis ultricies vitae; porta quis ante! " .
          "Suspendisse potenti. Cum sociis natoque penatibus " .
          "et magnis dis parturient montes, nascetur ridiculus mus. " .
          "Vestibulum ante ipsum primis in faucibus orci luctus et " .
          "ultrices posuere cubilia Curae; Vivamus a enim sapien, " .
          "ac iaculis justo. Class aptent taciti sociosqu ad litora " .
          "torquent per conubia nostra, per inceptos himenaeos. " .
          "Cum sociis natoque penatibus et magnis dis parturient " .
          "montes, nascetur ridiculus mus. Pellentesque rutrum risus " .
          "at orci porttitor sit amet tempor tortor eleifend. " .
          "Nullam placerat, leo id adipiscing ", "en"
        );
        $lang_string->setString(
          "Test Description automatisch $i Duis leo lorem, " .
          "cursus quis ultricies vitae; porta quis ante! " .
          "Suspendisse potenti. Cum sociis natoque penatibus " .
          "et magnis dis parturient montes, nascetur ridiculus mus. " .
          "Vestibulum ante ipsum primis in faucibus orci luctus et " .
          "ultrices posuere cubilia Curae; Vivamus a enim sapien, " .
          "ac iaculis justo. Class aptent taciti sociosqu ad litora " .
          "torquent per conubia nostra, per inceptos himenaeos. " .
          "Cum sociis natoque penatibus et magnis dis parturient " .
          "montes, nascetur ridiculus mus. Pellentesque rutrum risus " .
          "at orci porttitor sit amet tempor tortor eleifend. " .
          "Nullam placerat, leo id adipiscing ", "de"
        );
        $general->setDescription($lang_string);

        $lom->setGeneral($general);

        $lom->lom_id = md5(uniqid(mt_rand(), TRUE));
        $lom_id = $lom->lom_id;

        if (is_file(ARCHIBALD_DRUSH_JSON_PATH . '/' . $lom->lom_id)) {
          drush_log('Export ArchibaldLom update: ' . $lom_id, 'ok');
        }
        else {
          drush_log('Export ArchibaldLom new: ' . $lom_id, 'ok');
        }
        file_put_contents(
          ARCHIBALD_DRUSH_JSON_PATH . '/' . $lom->lom_id, ArchibaldLomSaveHandler::jsonExport($lom)
        );
      }
      drush_log(
        "File " . $lom_id . " exported with 100 random entries", 'ok'
      );
      break;

    case 'load':
      $lom = ArchibaldLomSaveHandler::load($lom_id);
      print_r($lom->getValues());
      break;

    case 'insert':

      $lom = new ArchibaldLom();

      $vcard = "BEGIN:VCARD
VERSION:3.0
CLASS:PUBLIC
PRODID:-//class_vcard from TroyWolf.com//NONSGML Version 1//EN
REV:2011-04-26 03:56:54
FN:Christian Ackermann
N:Ackermann;Christian;;;
TITLE:ICT-Entwicklung und Support
ORG:Educa.ch - Schweizer Medieninstitut für Bildung und Kultur
ADR;TYPE=work:;;Erlachstrasse 22;Bern 9;CH;3000 ;
EMAIL;TYPE=internet,pref:christian.ackermann@educa.ch
TEL;TYPE=work,voice:+41 31 300 5541
TZ:-0500
END:VCARD";

      $general = new ArchibaldLomDataGeneral();

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString(
        "just a title from general", "en"
      );

      $lang_string->setString(
        "ist nur ne titel für general", "de"
      );

      $general->setTitle($lang_string);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString(
        "just a description from general", "en"
      );

      $lang_string->setString(
        "ist nur ne beschreibung für general", "de"
      );

      $general->setDescription($lang_string);

      $general->setAggregationLevel(ArchibaldLom::GENERAL_AGGREGATIONLEVEL_2);


      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("coverage 2", "en");
      $general->addCoverage($lang_string);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("coverage 1", "en");
      $general->addCoverage($lang_string);

      $general->setStructure(ArchibaldLom::GENERAL_STRUCTURE_ATOMIC);

      $general->addIdentifier(
        new ArchibaldLomDataIdentifier("ISBN", "5435-534r-fsdf234")
      );

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("keyword2", "en");
      $lang_string->setString("schlüsselwort2", "de");
      $general->addKeyword($lang_string);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("keyword1", "en");
      $lang_string->setString("schlüsselwort1", "de");
      $general->addKeyword($lang_string);

      $general->addLanguage('de');

      $lom->setGeneral($general);

      $live_cycle = new ArchibaldLomDataLifeCycle();
      $live_cycle->setStatus(ArchibaldLomDataLifeCycle::lifeCYCLE_STATUS_FINALE);
      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("1.0", "en");
      $lang_string->setString("äens döt nüll", "de");
      $live_cycle->setVersion($lang_string);

      $contribute = new ArchibaldLomDataContribute();
      $contribute->setRole(ArchibaldLomDataLifeCycle::CONTRIBUTE_ROLE_AUTHOR);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("date description", "en");
      $lang_string->setString("datumsbeschreibung", "de");

      $date = new ArchibaldLomDataDateTime(time(), $lang_string);
      $contribute->setDate($date);
      $contribute->addEntity($vcard);

      $live_cycle->addContribute($contribute);
      $lom->setLifeCycle($live_cycle);

      $rights = new ArchibaldLomDataRights();
      $rights->setCost(ArchibaldLomDataRights::COST_NO);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("right description", "en");
      $lang_string->setString("rechtebeschreibung", "de");
      $rights->setDescription($lang_string);
      $lom->setRights($rights);

      $technical = new ArchibaldLomDataTechnical();
      $technical->addFormat("application/octec-stream");
      $technical->addLocation("http://google.de/tollefile.txt");
      $requirement = new ArchibaldLomDataRequirement();
      $or_composite = new ArchibaldLomDataOrComposite();
      $or_composite->setMaximumVersion(1);
      $or_composite->setMinimumVersion(0.5);
      $or_composite->setType(
        ArchibaldLomDataOrComposite::TYPE_OPERATING_SYSTEM
      );

      $or_composite->setName(
        ArchibaldLomDataOrComposite::NAME_OPERATING_SYSTEM_MS_WINDOWS
      );
      $requirement->addOrComposite($or_composite);

      $technical->addRequirement($requirement);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("firefox", "en");
      $lang_string->setString("firefox", "de");
      $technical->setOtherPlattformRequirements($lang_string);

      $technical->setSize(1024);

      $technical->setDuration(
        new ArchibaldLomDataDuration(
          array(
            "hours" => 2,
            "minutes" => 10,
            "seconds" => 23.4,
          )
        )
      );

      $lom->setTechnical($technical);

      $annotation = new ArchibaldLomDataAnnotation();
      $annotation->setDate($date);
      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString(
        "just a description from annoation", "en"
      );
      $lang_string->setString(
        "ist nur ne beschreibung für annoation", "de"
      );
      $annotation->setDescription($lang_string);
      $annotation->setEntity($vcard);

      $lom->addAnnotation($annotation);


      $education = new ArchibaldLomDataEducation();

      $education->addContext(
        ArchibaldLomDataEducation::CONTEXT_SCHOOL
      );

      $education->addContext(
        ArchibaldLomDataEducation::CONTEXT_TRAINING
      );

      $education->addIntendedEndUserRole(
        ArchibaldLomDataEducation::INTENDED_END_USER_ROLE_TEACHER
      );
      $education->addLanguage(
        "de"
      );

      $education->addLearningResourceType(
        'documentary',
        ArchibaldLomDataEducation::LEARNING_RESOURCE_TYPE_COURSE
      );

      $education->addTypicalAgeRange(
        "12-17"
      );

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString(
        "just a description from education", "en"
      );
      $lang_string->setString(
        "ist nur ne beschreibung für education", "de"
      );
      $education->setDescription($lang_string);

      $education->setSemanticDensity(ArchibaldLomDataEducation::SEMANTIC_LEVEL_LOW);
      $lom->setEducation($education);

      $meta_meta_data = new ArchibaldLomDataMetaMetadata();

      $contribute = new ArchibaldLomDataContribute();
      $contribute->setRole(ArchibaldLomDataMetaMetadata::CONTRIBUTE_ROLE_CREATOR);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("date description", "en");
      $lang_string->setString("datumsbeschreibung", "de");

      $date = new ArchibaldLomDataDateTime(time(), $lang_string);
      $contribute->setDate($date);
      $contribute->addEntity($vcard);

      $meta_meta_data->addContribute($contribute);
      $meta_meta_data->setLanguage("de");
      $lom->setMetaMetadata($meta_meta_data);


      $classification = new ArchibaldLomDataClassification();
      $classification->setPurpose(ArchibaldLomDataClassification::PURPOSE_SKILL_LEVEL);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("classikeyword1", "en");
      $lang_string->setString("classikeyword1", "de");
      $classification->addKeyword($lang_string);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("class desc", "en");
      $lang_string->setString("classifikation beschreibung irgendwas", "de");
      $classification->setDescription($lang_string);

      $taxon_path = new ArchibaldLomDataTaxonPath();
      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("educa source", "en");
      $lang_string->setString("educa source", "de");
      $taxon_path->setSource($lang_string);

      $lang_string = new ArchibaldLomDataLangString();
      $lang_string->setString("master chief instructor for the world", "en");
      $lang_string->setString("gott", "de");

      $taxon = new ArchibaldLomDataTaxon("badboy", $lang_string);

      $taxon_path->addTaxon($taxon);

      $classification->addTaxonPath($taxon_path);
      $lom->addClassification($classification);

      ArchibaldLomSaveHandler::insert($lom);
      drush_log("ok\n", 'ok');
      echo "ok\n";
      break;

    case 'set_env_api':
      $var = 'https://release.api.dsb';
      variable_set('archibald_api_url', $var);
      echo "archibald_api_url: $var\n";

      $var = 'http://release.ontology.dsb/curriculum/latest.xml';
      variable_set('archibald_curriculum_per_curriculum_url', $var);
      echo "archibald_curriculum_per_curriculum_url: $var\n";

      $var = 'http://release.ontology.dsb';
      variable_set('archibald_ontology_url', $var);
      echo "archibald_ontology_url: $var\n";

      $solr = @$args[1];
      if (!empty($solr)) {
        $sql = "UPDATE
          apachesolr_environment
          SET
            url = 'http://devsolr:8993/solr/" . $solr . "'
          WHERE
            name = 'localhost'
        ";
        db_query($sql);
        echo "solr: $solr\n";
      }
      break;

    case 'mail':
      $subject = NULL;
      $body = NULL;

      $args_key = 2;
      $email_address = @$args[$args_key];
      if (empty($email_address)) {
        $email_address = variable_get('site_mail', '');
      }
      $args_key++;

      $mail_key = @$args[$args_key];
      if (empty($mail_key)) {
        echo "USAGE\n";
        echo "*****\n";
        echo "new_proposal\n";
        echo "new_unavailable\n";
        echo "proposal2draft\n";
        echo "status2final\n";
        echo "status2draft\n";
        echo "set_responsible\n";
        echo "proposal2publish\n";
        echo "*****\n";
        return;
        //$subject = 'Test e-mail';
        //$body = 'Test body';
      }
      else {
        if ($lom_id == 0) {
          switch ($mail_key) {
            default:
              $row = db_query_range("SELECT lom_id FROM {archibald_lom_stats} WHERE proposal_contact_name != ''", 0, 1)->fetchAssoc();
              if (!empty($row)) {
                $lom_id = $row['lom_id'];
              }
              break;
          }
        }
      }
      $args_key++;

      // $language = @$args[$args_key];
      // if (empty($language)) {
      //   $language = 'de';
      // }
      // $args_key++;

      // $params = array(
      //   'lom_id' => $lom_id,
      //   'subject' => $subject,
      //   'body' => $body,
      // );

      // drupal_mail('archibald', $mail_key, $email_address, $language, $params);

      archibald_send_mail($lom_id, $mail_key, NULL, '', '', $email_address);
      break;

    default:
      drush_log($action . ": command not understood", 'error');
      break;
  }
}

/**
 * drush command for syncing taxonomys
 */
function drush_archibald_taxonomy_sync($source) {
  if (!isset($source)) {
    echo "Argument missing: Set 'csv' to import taxonomies from CSV file or " . "set ontology identifier to import from ontology server.\n\n\n";
    return;
  }

  if ($source == 'csv') {
    // Sync local taxonomy from csv file
    archibald_sync_taxonomy($source);
  }
  else {
    // Sync local taxonomy from ontology server
    archibald_sync_taxonomy_os($source);
  }
}

/**
 * get files form temfolder and upload them
 *
 * @return array
 *   list of fid (file ids)
 */
function archibald_demo_data_get_image_list() {
  $image_ids = array();

  $d = dir(ARCHIBALD_DRUSH_JSON_PATH . '/images/');
  while (FALSE !== ($filename = $d->read())) {
    if (is_file($d->path . '/' . $filename)) {
      $fid = db_query(
        'SELECT fid FROM {file_managed} WHERE filename LIKE :filename',
        array(
         ':filename' => $filename,
        )
      )->fetchColumn(0);

      if (empty($fid)) {
        $temp_file = realpath($d->path . '/' . $filename);

        $destination = 'public://files/preview_image/';

        // Begin building file object.
        $file = new stdClass();
        // uid 1 is Admin.
        $file->uid      = 1;
        $file->status   = FILE_STATUS_PERMANENT;
        $file->filename = trim($filename, '.');
        $file->filemime = file_get_mimetype($temp_file);
        $file->filesize = filesize($temp_file);

        $destination_scheme = file_uri_scheme($destination);

        $file->uri = file_destination(
          $destination . $file->filename, FILE_EXISTS_RENAME
        );
        copy($temp_file, $file->uri);

        drupal_chmod($file->uri);

        $file = file_save($file);
        file_usage_add($file, 'archibald', 'identifier_file', 1);

        $fid = $file->fid;

        drush_log('Import image: ' . $filename, 'ok');
      }
      $image_ids[$fid] = $filename;
    }
  }
  $d->close();

  return $image_ids;
}

/**
 * reset all files with upload errors, so process can be later resumed
 */
function drush_archibald_reset_upload_errors() {
  $query = db_select('file_managed', 'fm');
  $query->innerJoin(
    'archibald_file', 'df', 'df.fid = fm.fid AND df.timestamp = fm.timestamp'
  );

  $query->fields(
    'df', array('url')
  );

  $query->fields(
    'fm', array(
      'fid',
      'timestamp',
      'filename',
      'filemime',
      'filesize',
    )
  );
  $query->condition('df.url', 'error:%', 'LIKE');
  $res = $query->execute();

  while ($row = $res->fetchObject()) {
    drush_log('Reset: ' . $row->filename . ' (' . $row->fid . ')', 'ok');
    db_update('archibald_file')->condition('fid', $row->fid)->condition('timestamp', $row->timestamp)->fields(array('url' => ''))->execute();
  }

  $res = db_query(
    "SELECT lom_id, publication_version, publication_log, publication_progress " .
    "FROM {archibald_lom_stats} as archibaldls " .
    "WHERE ( " .
    " publication_progress IS NULL OR " .
    " publication_progress<100) " .
    "AND publication_version IS NOT NULL"
  );

  while ($row = $res->fetchObject()) {
    if (!empty($row)) {
      if (empty($row->publication_log)) {
        $row->publication_log = new stdClass();
      }
      else {
        $row->publication_log = unserialize($row->publication_log);
      }

      if ($row->publication_log->lom_status == 'error' ||
        $row->publication_log->lom_status == 'files not ready'
      ) {
        drush_log('Reset: ' . $row->lom_id, 'ok');

        db_update('archibald_lom_stats')->condition('lom_id', $row->lom_id)->fields(
          array(
            'publication_progress' => NULL,
            'publication_log' => NULL,
          )
        )->execute();
      }
    }
  }

  db_update('archibald_lom_stats')->fields(array(
    'batch_op_key' => '',
    'batch_op_time' => 0,
  ))->execute();

  variable_set('archibald_upload_error', 0);
}

/**
 * Callback running curriculum setup_process methods
 */
function drush_archibald_resetup_curriculum() {
  if (!empty($args[0])) {
    drush_archibald_resetup_curriculum_run($args[0]);
    return TRUE;
  }

  $dir = DRUPAL_ROOT . '/' . drupal_get_path('module', 'archibald') . '/includes/curriculum/';

  foreach (scandir($dir) as $file) {
    if (preg_match('/^curriculum_([a-z0-9]+)\.class\.php$/i', $file, $curriculum)) {
      $curriculum[1];
      drush_archibald_resetup_curriculum_run($curriculum[1]);
    }
  }
}

/**
 * Subfunction that calls the setup process
 *
 * @param string $curriculum
 */
function drush_archibald_resetup_curriculum_run($curriculum) {
  $curr_name = 'curriculum_' . $curriculum;
  $class_name = 'ArchibaldCurriculum' . drupal_ucfirst($curriculum);
  module_load_include('php', 'archibald', 'includes/curriculum/' . $curr_name . '.class');
  $curr_handler = new $class_name();
  if (method_exists($curr_handler, 'setupProcess')) {
    $curr_handler->setupProcess();
    drush_log('Reimported: ' . $curriculum, 'ok');
  }
  else {
    drush_log('Dont have a setup_process: ' . $curriculum, 'notice');
  }
}

/**
 * Run solr indexer
 */
function drush_archibald_solr_index() {
  require_once dirname(__FILE__) . "/../includes/LomSolrSearch.class.php";

  $archibald_lom_solr_search = new ArchibaldLomSolrSearch();
  $archibald_lom_solr_search->cron();
}

/**
 * Fix the server_type
 */
function drush_archibald_fix_server_type() {
  if (db_driver() == 'pgsql') {
    $sql = "SELECT
              fid,
              timestamp,
              array_to_string(array_agg(CASE WHEN file_server_type IS NULL THEN 'NULL' ELSE file_server_type END), ',') AS fst
            FROM
              archibald_file
            GROUP BY
              fid, timestamp
            ORDER BY
              fid";
  }
  else
  {
    $sql = "SELECT
              fid,
              timestamp,
              GROUP_CONCAT( CASE ISNULL(file_server_type) WHEN 1 THEN 'NULL' ELSE file_server_type END ) AS fst
            FROM
              archibald_file
            GROUP BY
              fid, timestamp
            ORDER BY
              fid";
  }
  $res = db_query($sql);
  while ($row = $res->fetchAssoc()) {
    $fst = explode(',', $row['fst']);
    if (count($fst) > 1) {
      $sql_delete = FALSE;
      print_r($row);
      if( in_array('no', $fst) ){
        $sql_delete = "DELETE
          FROM
            archibald_file
          WHERE
            fid = " . $row['fid'] . "
          AND
            timestamp = " . $row['timestamp'] . "
          AND
            (file_server_type != 'no' OR file_server_type IS NULL)
        ";
        echo "**NO**";
      }
      elseif ( in_array('archibald', $fst) ) {
        $sql_delete = "DELETE
          FROM
            archibald_file
          WHERE
            fid = " . $row['fid'] . "
          AND
            timestamp = " . $row['timestamp'] . "
          AND
            (file_server_type != 'archibald' OR file_server_type IS NULL)
        ";
        echo "**ARCHIBALD**";
      }
      elseif ( in_array('educa', $fst) ) {
        $sql_delete = "DELETE
          FROM
            archibald_file
          WHERE
            fid = " . $row['fid'] . "
          AND
            timestamp = " . $row['timestamp'] . "
          AND
            (file_server_type != 'educa' OR file_server_type IS NULL)
        ";
        echo "**EDUCA**";
      }
      if ($sql_delete){
        echo "\n";
        print_r($sql_delete);
        db_query($sql_delete);
      }
      echo "\n\n\n";
    }
  }
}

/**
 * sync coverage from data file to local db
 *
 * @param string $type
 *  list: list all possible types
 *  swd: import an swd data file
 *  rameau: import an rameau data file
 * @param string $charset encoding of data file
 *  utf-8
 *  iso5426
 * @param string $file
 *  full path to the data file
 * @param boolean $nocleanup
 *  FALSE: default, clean up database bevor importing
 *  TRUE: merge old and new import together
 */
function drush_archibald_import_coverages($type = "list", $charset = "", $file = "", $nocleanup = FALSE) {

  if ((empty($type) || $type == 'list') && empty($charset) && empty($file)) {
    drush_log(
      'Usage: drush archibald_sync_coverages <type> <charset> <file>', 'warning'
    );

    include_once SITEPATH . "/includes/Dir.class.php";

    drush_log(
      'Possible charsets:', 'warning'
    );

    $dir = new ArchibaldDir("includes/coverage_sync/charsets", FALSE);
    $dir->justFiles();

    foreach ($dir as $entry) {
      echo '-- ' . $entry->name . "\n";
    }
    echo "\n";
    drush_log('Possible import types:', 'warning');

    $dir = new ArchibaldDir("includes/coverage_sync", FALSE);
    $dir->justFiles();

    foreach ($dir as $entry) {
      echo '-- ' . $entry->name . "\n";
    }
    return;
  }

  $type = drupal_strtolower($type);
  $file_type = SITEPATH . "/includes/coverage_sync/" . $type . ".inc";
  if (!file_exists($file_type)) {
    drush_log('Import type: ' . $type . ' not found', 'error');
    return;
  }

  require_once SITEPATH . "/includes/AbstractCoverageType.class.php";
  require_once SITEPATH . "/includes/coverage_sync/" . $type . ".inc";


  $class    = "ArchibalCoverage" . drupal_ucfirst($type);
  $import   = new $class();
  $keywords = $import->buildKeywords($file, $charset);

  if ($keywords === FALSE) {
    // Build_keywords returned FALSE so an error occured.
    switch ($import->error) {
      case ArchibaldAbstractCoverageType::ERROR_CHARSET_NOT_FOUND:
        $error = t('Charset not found:') . ' ' . $charset;
        break;

      case ArchibaldAbstractCoverageType::ERROR_CHARSET_VARIABLE_NOT_SET:
        $error = t('Charset variable not found within file:') . ' ' . $charset;
        break;

      case ArchibaldAbstractCoverageType::ERROR_SOURCE_NOT_FOUND:
        $error = t('Source file not found: @file', array('@file' => $file));
        break;
    }
    drush_log('Import error: ' . $error, 'error');
    return;
  }

  drush_log('Got ' . count($keywords), 'ok');

  if (empty($nocleanup)) {
    drush_log('Cleaning old vocabulary: ' . $type, 'warning');
    db_query(
      "DELETE FROM {archibald_coverage_list} WHERE context = :type",
      array(
        "type" => $type,
      )
    );

    drush_log('Vocabulary cleaned', 'ok');
  }
  else {
    drush_log('Skipping vocabulary clean-up', 'ok');
  }
  drush_log('Insert terms', 'warning');
  $language = $import->getLanguage();
  foreach ($keywords as $kw) {
    db_query(
      "INSERT IGNORE INTO {archibald_coverage_list} " .
      "(coverage, context, language) " .
      "VALUES (:coverage,:type, :language)",
      array(
        'coverage' => $kw,
        'type' => $type,
        'language' => $language,
      )
    );
    drush_log('Term "' . $kw . '" saved', 'ok');
  }
  drush_log($type . ' import finished', 'ok');
}

/**
 * Delete marked items permanently
 */
function drush_archibald_purge_deleted() {
  $results = db_select('archibald_lom', 'l')
    ->fields('l', array('lom_id', 'version'))
    ->condition('deleted', 1)
    ->orderBy('save_time', 'DESC')
    ->execute()
    ->fetchAll();

  foreach ($results AS $row) {
    print_r( $row );
    ArchibaldLomSaveHandler::purge($row->lom_id, $row->version, TRUE);
  }
}


function drush_archibald_test_mmi() {
  $query = "SELECT * FROM
    (
    SELECT entry, COUNT(entry) AS count_entries
      FROM archibald_meta_metadata_identifier
      GROUP BY meta_metadata_id, entry
    ) AS s
    WHERE
      s.count_entries > 1";
    $res = db_query( $query );
    while ($row = $res->fetchAssoc()) {
      print_r( $row );
    }
}


/**
 * Delete given LOM ID from SOLR index
 */
function drush_archibald_delete_from_solr( $lom_id ) {
  if (variable_get('archibald_solr_search_activ', 0) == 1 && module_exists('apachesolr')) {
    // Delete from Solr Index
    $solr_search = new ArchibaldLomSolrSearch();
    $solr_search->deleteFromIndex($lom_id);
  }
}

function drush_archibald_set_api( $flavor = 'production', $ip = null ) {
  switch ($flavor) {
    default:
    case 'production':
      $env = array(
        'api_server' => 'https://dsb-api.educa.ch/v2',
        'per_curriculum' => 'http://ontology.biblio.educa.ch/curriculum/latest.xml',
        'ontology_server' => 'http://ontology.biblio.educa.ch'
      );
      break;

    case 'demo':
      $env = array(
        'api_server' => 'https://demo-dsb-api.educa.ch/v2',
        'per_curriculum' => 'http://ontology.biblio.educa.ch/curriculum/latest.xml',
        'ontology_server' => 'http://ontology.biblio.educa.ch'
      );
      break;

    case 'dev':
      $env = array(
        'api_server' => 'https://dev-dsb-api.educa.ch/v2',
        'per_curriculum' => 'http://ontology.biblio.educa.ch/curriculum/latest.xml',
        'ontology_server' => 'http://ontology.biblio.educa.ch'
      );
      break;

    case 'local':
      $env = array(
        'api_server' => 'https://release.api.dsb',
        'per_curriculum' => 'http://release.ontology.dsb/curriculum/latest.xml',
        'ontology_server' => 'http://release.ontology.dsb'
      );
      break;

    case 'docker':
      if (is_null($ip)) {
        echo "Please set the IP address of the host for the docker containers (e.g 192.168.1.42)";
        return false;
      }
      $env = array(
        'api_server' => "http://$ip:9100/v2",
        'per_curriculum' => 'http://ontology.biblio.educa.ch/curriculum/latest.xml',
        'ontology_server' => 'http://ontology.biblio.educa.ch'
      );
      break;
  }

  variable_set('archibald_api_url', $env['api_server']);
  echo "archibald_api_url: " . $env['api_server'] . "\n";

  variable_set('archibald_curriculum_per_curriculum_url', $env['per_curriculum']);
  echo "archibald_curriculum_per_curriculum_url: " . $env['per_curriculum'] . "\n";

  variable_set('archibald_ontology_url', $env['ontology_server']);
  echo "archibald_ontology_url: " . $env['ontology_server'] . "\n";
}


function drush_archibald_get_api() {
  echo "archibald_api_url: " . variable_get('archibald_api_url') . "\n";
  echo "archibald_curriculum_per_curriculum_url: " . variable_get('archibald_curriculum_per_curriculum_url') . "\n";
  echo "archibald_ontology_url: " . variable_get('archibald_ontology_url') . "\n";
}