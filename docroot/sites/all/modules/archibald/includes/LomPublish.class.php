<?php

require_once dirname(__FILE__) . '/archibald.transition.php';
use \Educa\DSB\LomDataSanitization;

/**
 * @file
 * class that is used to publish local lom resources to the central catalog
 */

/**
 * Require abstract parent class
 */
require_once dirname(__FILE__) . '/dsb_api.class.php';

/**
 * This class publish lom objetecs with are marked for this at database
 *
 * @author Heiko Henning <h.henning@educa.ch>
 */
class ArchibaldLomPublish extends ArchibaldAbstractDsbApi {

  /**
   * objects with is to process now
   * @var object
   */
  protected $otp = NULL;

  /**
   * current lom object with willbe processed
   * @var ArchibaldLom
   */
  protected $lom = NULL;

  /**
   * index of $this->objectToPublish->
   *                  publication_log->files
   * for current processed file
   * @var int
   */
  private $currentProcessedFile = -1;

  /**
   * process all lom objects with are marked as to publish in database
   *
   * @return boolean status
   */
  public function cron($batch_op_key=NULL, $batch_op_time=NULL) {
    // if the cron job encountered an error while processing
    if (variable_get('archibald_upload_error', 0) == 1) {
      return $this->drushLog('archibald_upload_error equals 1', 'error');
    }

    if (ini_get('max_execution_time') == 0) {
      $maintenance_max_execution_time = 240;
    }
    else {
      $maintenance_max_execution_time = ini_get('max_execution_time');
    }

    if ($maintenance_max_execution_time > 20) {
      $maintenance_max_execution_time -= 10;
    }

    if (!lock_acquire('lom_publish', ($maintenance_max_execution_time + 1))) {
      return $this->drushLog('Publish: action is locked, perhaps a gui publish process is open', 'error');
    }

    // get a list of candidates to publish
    $objects_to_publish = $this->getObjectsToPublish($batch_op_key, $batch_op_time);

    $this->drushLog('Determine objects to publish, found: ' . count($objects_to_publish), 'ok');

    if (empty($objects_to_publish)) {
      lock_release('lom_publish');
      return FALSE;
    }


    $maintenance_start_time = time();
    while (($maintenance_start_time + ($maintenance_max_execution_time / 2)) > time()) {

      $this->objectToPublish = NULL;

      // search an object without errors and not 100% completed
      // then publish it


      foreach ($objects_to_publish as $otp) {
        $pub_status = '';
        if (isset($otp->publication_log->lom_status)) {
          $pub_status = $otp->publication_log->lom_status;
        }

        if (in_array($pub_status, array('files not ready', 'error')) || (INT) $otp->publication_progress == 100) {
          continue;
        }
        // set chosen candidate to publish
        $this->objectToPublish = $otp;
        break;
      }

      // success, we have all published or an error )-;
      if (empty($this->objectToPublish)) {
        lock_release('lom_publish');
        return;
      }

      // start processing
      // load object to publish into object and start
      $this->lom = ArchibaldLomSaveHandler::load($this->objectToPublish->lom_id, $this->objectToPublish->publication_version);
      $this->objectToPublish->lom_stats = archibald_get_lom_stats($this->objectToPublish->lom_id);
      $this->drushLog('Start process lom object: ' . ArchibaldLom::determinTitle($this->lom), 'notice');

      if (!isset($this->objectToPublish->publication_log->lom_status)) {
        // if it is the first time the publication process runs, we touch this resource
        // echo "\n" . '################# INIT PUBLICATION LOG #####################' . "\n";
        $this->initPublicationLog();
      }

      // upload the files first
      while ($this->processFile() && (($maintenance_start_time + ($maintenance_max_execution_time / 2)) > time())) {
        // on upload attempt we run into a fatal permanent error
        if (variable_get('archibald_upload_error') == 1) {
          break;
        }
      }

      if (($maintenance_start_time + ($maintenance_max_execution_time / 2)) > time()) {
        // check if all files are online
        $all_files_uploaded = TRUE;
        foreach ($this->objectToPublish->publication_log->files as $file) {
          if ($file->progress < 100) {
            $all_files_uploaded = FALSE;
          }
        }
        if ($all_files_uploaded == TRUE) {
          if ($this->processLom() == TRUE) {
            $this->objectToPublish->publication_log->lom_status = 'success';
          }
          else {
            $this->objectToPublish->publication_log->lom_status = 'error';
          }


          if ($this->objectToPublish->publication_log->lom_status == 'success') {
            $log_type = 'ok';
          }
          else {
            $log_type = 'error';
          }

          $this->drushLog('Process lom object: ' . ArchibaldLom::determinTitle($this->lom), $log_type);

        }
        else {
          $this->drushLog('Lom object "files not ready": ' . ArchibaldLom::determinTitle($this->lom), 'error');

          if (!$this->contentPartnerCanCentral()) {
            $this->drushLog('Lom object, self healing, reset upload publish bit to unpublished: ' . ArchibaldLom::determinTitle($this->lom), 'error');

            db_update('archibald_lom_stats')
              ->condition('lom_id', $this->objectToPublish->lom_id)
              ->fields(
                array(
                  'publication_version' => NULL,
                )
              )
              ->execute();
          }
          else {
            $this->objectToPublish->publication_log->lom_status = 'files not ready';
          }

          db_update('archibald_lom_stats')
            ->fields(array(
              'batch_op_key' => '',
              'batch_op_time' => 0,
            ))
            ->condition('lom_id', $this->objectToPublish->lom_id)
            ->execute();
        }
      }

      // update stats in db
      $this->caclulatePublicationProgress(TRUE);

      // on upload attempt we run into a fatal permanent error
      if (variable_get('archibald_upload_error') == 1) {
        break;
      }
    }

    lock_release('lom_publish');
    return TRUE;
  }

  /**
   * get list lom objetec from database with needs to be processed
   */
  public function getObjectsToPublish($batch_op_key=NULL, $batch_op_time=NULL) {
    $objects_to_publish = array();
    $sql = "SELECT DISTINCT ls.lom_id, ls.publication_version,
              ls.publication_log, ls.publication_progress
                 FROM {archibald_lom_stats} AS ls
                 INNER JOIN {archibald_lom} AS l ON (ls.lom_id=l.lom_id)
                 WHERE l.deleted IS NULL
                 AND (ls.publication_progress IS NULL
                  OR ls.publication_progress<100)
                 AND ls.publication_version IS NOT NULL";
    if ($batch_op_key && $batch_op_time) {
      $sql .= " AND ls.batch_op_key = :batch_op_key ";
      $res = db_query($sql, array(
        ':batch_op_key' => $batch_op_key
      ));
    }
    else {
      $sql .= " AND (ls.batch_op_time = 0 OR ls.batch_op_time < :limit_time) ";
      $res = db_query($sql, array(
        ':limit_time' => time() - 1800
      ));
    }

    while ($elm = $res->fetchObject()) {
      if (!empty($elm)) {
        if (empty($elm->publication_log)) {
          $elm->publication_log = new stdClass();
        }
        else {
          $elm->publication_log = unserialize($elm->publication_log);
        }
        $objects_to_publish[] = $elm;
      }
    }

    return $objects_to_publish;
  }

  /**
   * create dfeault structure for
   * $this->objectToPublish->publication_log
   * and load all files from lob objetec with we need to process
   */
  protected function initPublicationLog() {
    $this->objectToPublish->publication_log->lom_status = 'processing files';

    // array of objects(
    //  fid->integer,
    //  type->string,
    //  filename->string,
    //  filemime->string,
    //  filesize->interger,
    //  progress->integer,
    //  url->string)
    $this->objectToPublish->publication_log->files = array();

    // General identifier.
    foreach ($this->lom->getGeneral()->getIdentifier() as $identifier) {
      if ($identifier->getCatalog() == 'URL') {
        preg_match('/archibald_file\/([0-9]+)\//', $identifier->getEntry(), $old_fid);

        if (!empty($old_fid[1])) {
          $upload_infos = $this->getFileUploadInformations($old_fid[1]);
          $upload_infos->fid = $old_fid[1];
          $upload_infos->type = 'identifier';
          $this->objectToPublish->publication_log->files[$upload_infos->fid] = $upload_infos;
        }
      }
    }

    // technical.preview_image
    $preview_image = $this->lom->getTechnical()->getPreviewImage();
    $preview_image_image = $preview_image->getImage();
    preg_match('/archibald_file\/([0-9]+)\//', $preview_image_image, $old_fid);
    if (!empty($old_fid[1])) {
      $upload_infos = $this->getFileUploadInformations($old_fid[1]);
      $upload_infos->fid = $old_fid[1];
      $upload_infos->type = 'preview_image';
      $this->objectToPublish->publication_log->files[$upload_infos->fid] = $upload_infos;
    }

    $this->add_content_partner_contributor();

    // lifeCycle contributor
    foreach ($this->lom->getLifeCycle()->getContribute() as $contribute) {
      if (!isset($contribute->entity[0])) {
        continue;
      }

      $parser = new ArchibaldAppDataVcard_Parser($contribute->entity[0]);
      $vcard  = $parser->parse();
      $vcard  = $vcard[0];

      $logo = NULL;
      if (isset($vcard->logo->uri)) {
        preg_match('/archibald_file\/([0-9]+)\//', $vcard->logo->uri, $tmp_fid);

        if (!empty($tmp_fid[1])) {
          $upload_infos = $this->getFileUploadInformations($tmp_fid[1]);
          $upload_infos->fid = $tmp_fid[1];
          $upload_infos->type = 'lifecycle_contributor';
          $this->objectToPublish->publication_log->files[$upload_infos->fid] = $upload_infos;
        }
      }
    }

    // metaMetadata contributor
    foreach ($this->lom->getMetaMetadata()->getContribute() as $contribute) {
      if (!isset($contribute->entity[0])) {
        continue;
      }

      // echo "\n" . "###### BEGIN VCARDS ######" . "\n";
      // print_r( $contribute );
      $parser = new ArchibaldAppDataVcard_Parser($contribute->entity[0]);
      $vcard  = $parser->parse();
      if (isset($vcard[0])) {
        $vcard  = $vcard[0];
        $logo = NULL;
        if (isset($vcard->logo->uri)) {
          preg_match('/archibald_file\/([0-9]+)\//', $vcard->logo->uri, $tmp_fid);
          if (!empty($tmp_fid[1])) {
            // print_r( $tmp_fid[1] );
            $upload_infos = $this->getFileUploadInformations($tmp_fid[1]);
            $upload_infos->fid = $tmp_fid[1];
            $upload_infos->type = 'contribute_person_logo';
            $this->objectToPublish->publication_log->files[$upload_infos->fid] = $upload_infos;
          }
        }
      }
      // echo "\n" . "###### END VCARDS ######" . "\n\n";
    }
  }

  /**
   * process a single file
   *
   * @return boolean status
   */
  protected function processFile() {
    if (empty($this->objectToPublish->publication_log->files)) {
      return FALSE;
    }

    foreach ($this->objectToPublish->publication_log->files as $file_to_process_id => &$file_to_process) {
      if ($file_to_process->progress == 0) {
        break;
      }
    }

    // the last file was also already uploaded
    if ($file_to_process->progress >= 100) {
      return FALSE;
    }

    $this->drushLog('Start File upload: ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'notice');

    $this->currentProcessedFile = $file_to_process_id;

    $status = $this->uploadFileToFileserver($file_to_process);
    // $status = new stdClass();
    // $status->code = 2015;

    if (is_object($status)) {
      switch (@$status->code) {
        case 99:
          watchdog(
            'file_upload', 'File upload error. While processing "@filename (@type)". Cannot open SSL private key.',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          variable_set('archibald_upload_error', 1);
          $file_to_process->progress = 0;
          $file_to_process->url = '';

          $this->drushLog('Upload result: "Can not open SSL Private Key" for ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'error');

          break;

        case 98:
          watchdog(
            'file_upload',
            'File upload error. While processing "@filename (@type)". Cannot find file. You should edit the description, replace this file and then republish.',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          $file_to_process->progress = 1;
          $file_to_process->url = 'error: file not found on local filesystem';

          $this->drushLog('Upload result: "Can not find file" ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'error');
          break;

        case 97:
          watchdog(
            'file_upload',
            'File upload error. No uploader configured for this content partner @server_type (@username)',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
              '@username' => $this->getContentPartnerCredential()->username,
              '@server_type' => $this->getContentPartnerCredential()->file_server_type,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          variable_set('archibald_upload_error', 1);
          $file_to_process->progress = 0;
          $file_to_process->url = '';

          $this->drushLog('Upload result: "File Upload error. No uploader configured for this content partner" for ' . $file_to_process->filename . ' (' . $file_to_process->type . ') (' . $this->getContentPartnerCredential()->file_server_type . ')', 'error');
          break;

        case 400:
          watchdog(
            'file_upload',
            'File upload error while processing "@filename (@type)". File server throws "wrong parameter". Please contact your administrator.',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          variable_set('archibald_upload_error', 1);
          $file_to_process->progress = 0;
          $file_to_process->url = '';

          $this->drushLog('Upload result: "wrong parameter" for ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'error');
          break;

        case 415:
          watchdog(
            'file_upload',
            'File upload error while processing "@filename (@type)". File server throws "not allowed file type" @server_type (@username). Please contact your administrator.',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
              '@username' => $this->getContentPartnerCredential()->username,
              '@server_type' => $this->getContentPartnerCredential()->file_server_type,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          $file_to_process->progress = 1;
          $file_to_process->url = 'error: not allowed extension';

          $this->drushLog('Upload result: "not allowed extension" for ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'error');
          break;

        case 413:
          watchdog(
            'file_upload',
            'File Upload error while processing "@filename (@type)". File server throws "file too big". Please contact your administrator. @server_type (@username)',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
              '@username' => $this->getContentPartnerCredential()->username,
              '@server_type' => $this->getContentPartnerCredential()->file_server_type,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          $file_to_process->progress = 1;
          $file_to_process->url = 'error: file too big';

          $this->drushLog('Upload result: "file too big" for ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'error');
          break;

        case 401:
        case 403:
          watchdog(
            'file_upload',
            'File upload error while processing "@filename (@type)". File server throws "access denied". Please contact your administrator. @server_type (@username)',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
              '@username' => $this->getContentPartnerCredential()->username,
              '@server_type' => $this->getContentPartnerCredential()->file_server_type,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          variable_set('archibald_upload_error', 1);
          $file_to_process->progress = 0;
          $file_to_process->url = '';

          $msg = @$status->message;

          $this->drushLog('Upload result: "access denied (' . $msg . ')" for ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'error');
          break;

        default:
          watchdog(
            'file_upload',
            'File upload error while processing "@filename (@type)". File server throws "unknown error". Please contact your administrator. @code !message.',
            array(
              '@type' => $file_to_process->type,
              '@filename' => $file_to_process->filename,
              '@lom_id' => $this->objectToPublish->lom_id,
              '@code' => @$status->code,
              '!message' => @$status->message,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          variable_set('archibald_upload_error', 1);
          $file_to_process->progress = 0;
          $file_to_process->url = '';
          $this->drushLog('Upload result: "unknown error" for ' . $file_to_process->filename . ' (' . $file_to_process->type . ')  ' . 'code:' . $status->code . ' ' . 'message:' . $status->message, 'error');
          break;
      }
    }
    else {
      watchdog(
        'file_upload',
        'The file "@filename" has now been published. @url',
        array(
          '@type' => $file_to_process->type,
          '@filename' => $file_to_process->filename,
          '@lom_id' => $this->objectToPublish->lom_id,
          '@url' => $status,
        ),
        WATCHDOG_NOTICE,
        'archibald/' . $this->objectToPublish->lom_id
      );


      $this->drushLog('API returned file URL : ' . print_r($status, 1));
      $file_to_process->url = $status;
      $this->setFileUploadInformations($file_to_process);

      $this->drushLog('Upload success: ' . $file_to_process->url . ' for ' . $file_to_process->filename . ' (' . $file_to_process->type . ')', 'ok');
    }

    return TRUE;
  }

  /**
   * process current lom object
   * send it to server and make some error handling
   * @see http://wiki.ubuntu/digitale_schulbibliothek/api/doku/save
   *
   * @param boolean $second_run
   *   if a lom auth fails, delete auth cache and re run
   */
  protected function processLom($second_run = FALSE) {
    if ($this->lomApiAuth($auth_token, $second_run)) {
      if ($second_run == FALSE) {
        $this->completeLomObject();
      }

      /* replace: local URL Pathes in identifier,
       * preview_image and
       * metaMetaData->Contributor vCard logos
       * agains central remote stored files
       */

      $this->replaceUrls();
      $this->replaceRightsLicence();
      $this->replaceRelationCatalogs();
      $this->replaceLocations();

      // print_r( $this->lom->getMetaMetadata() );
      // die();

      $ret = $this->lomApiPublish($this->lom);
      if (empty($ret)) {
        $ret = (object)array(
          'code' => 501,
          'message' => (empty($ret) ? 'empty return object' : $ret),
        );
      }
      elseif (!isset($ret->message) || empty($ret->message)) {
        $ret = (object)array(
          'code' => (empty($ret->code)) ? 501 : $ret->code,
          'message' => $ret,
        );
      }

      switch ($ret->code) {
        case '200':
          // ok
          $message = 'Success';

          watchdog(
            'lom_upload',
            'The description "!title" has now been published to the national catalogue.',
            array(
              '!title' => ArchibaldLom::determinTitle($this->lom),
              '@lom_id' => $this->objectToPublish->lom_id,
              '@message' => $message,
            ),
            WATCHDOG_NOTICE,
            'archibald/' . $this->objectToPublish->lom_id
          );

          // 2time watchdog cause of the differen watchdog type
          // need for publish history
          watchdog(
            'content',
            'Description "!title" was published.',
            array(
              '!title' => ArchibaldLom::determinTitle($this->lom),
              '@lom_id' => $this->objectToPublish->lom_id,
            ),
            WATCHDOG_NOTICE,
            'archibald/' . $this->objectToPublish->lom_id
          );

          return TRUE;

        case '502':
          // Authentication timed out, please revalidate.
          // cause of our first call is $this->lomApiAuth(),
          // this should never happen
          if ($second_run == FALSE) {
            $this->processLom(TRUE);
          }
          else {
            if (is_object($ret->message)) {
              $message = $ret->message->message;
            }
            else {
              $message = $ret->message;
            }

            watchdog(
              'lom_upload',
              'While processing description "!title" the server throws "@message". Please contact your administrator.',
              array(
                '!title' => ArchibaldLom::determinTitle($this->lom),
                '@lom_id' => $this->objectToPublish->lom_id,
                '@message' => $message,
              ),
              WATCHDOG_ERROR,
              'archibald/' . $this->objectToPublish->lom_id
            );
          }
          break;

        case '501':
          // error
        default:
          if (is_object($ret->message)) {
            $message = $ret->message->message;
          }
          else {
            $message = $ret->message;
          }

          watchdog(
            'lom_upload',
            'While processing description "!title" the server throws "@message". Please contact your administrator.',
            array(
              '!title' => ArchibaldLom::determinTitle($this->lom),
              '@lom_id' => $this->objectToPublish->lom_id,
              '@message' => $message,
            ),
            WATCHDOG_ERROR,
            'archibald/' . $this->objectToPublish->lom_id
          );

          return FALSE;
      }
    }
    else {
      watchdog(
        'lom_upload',
        'While publishing description "!title" the server throws "acces denied". Please contact your administrator.',
        array(
          '!title' => ArchibaldLom::determinTitle($this->lom),
          '@lom_id' => $this->objectToPublish->lom_id,
        ),
        WATCHDOG_ERROR,
        'archibald/' . $this->objectToPublish->lom_id
      );

      variable_set('archibald_upload_error', 1);
      return FALSE;
    }

    return FALSE;
  }

  /**
   * upload file a configured file server

   *
   * @param object $file_to_process
   *
   * @return mixed
   *  string with url on success, else array with error information
   */
  protected function uploadFileToFileserver($file_to_process) {
    //We only want third party file server locations of the type is an
    //identifier
    if ($file_to_process->type != 'identifier') {
      $uploader = 'educa';
    }
    else {
      $uploader = $this->getContentPartnerCredential()->file_server_type;
    }

    if (empty($uploader) || $uploader == 'none') {
      return (object)array('code' => 97);
    }

    module_load_include('php', 'archibald', 'includes/file_uploader/uploader.' . $uploader . '.class');

    $uploader_class = 'ArchibaldUploader' . drupal_ucfirst($uploader);
    if (!class_exists($uploader_class)) {
      return (object)array('code' => 97);
    }
    $uploader_instance = new $uploader_class();

    $file = file_load($file_to_process->fid);
    $file_real_path = $file->uri;

    if (isset($file) && $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri)) {
      $file_real_path = $wrapper->realpath();
    }

    if (!is_file($file_real_path)) {
      // uhhh, the user should reupload this file
      return (object)array('code' => 98);
    }

    $result_object = $uploader_instance->uploadFile(
      $this->getContentPartnerCredential()->content_partner_id,
      $file_real_path,
      $file_to_process->fid,
      array($this, 'uploadFileProgress'),
      $this->objectToPublish
    );
    $this->drushLog('Get upload status: ' . $result_object->code . ( isset($result_object->message) ? ' (' . $result_object->message . ')' : ''), 'notice');

    if ($result_object->code == 200) {
      $this->objectToPublish->publication_log->files[$this->currentProcessedFile]->progress = 100;

      $this->currentProcessedFile = -1;
      $this->caclulatePublicationProgress(TRUE);

      return $result_object->fileUrl;
    }

    $this->currentProcessedFile = -1;
    return $result_object;
  }

  /**
   * curl upload/download progress callback function
   *
   * @param float $progress
   *   progress in percent
   */
  public function uploadFileProgress($progress) {
    $last_drush_msg = &archibald_static(__FUNCTION__, "");

    if (!isset($this->objectToPublish->publication_log->files[$this->currentProcessedFile])) {
      // opps this mistake should never happen
      return (0);
    }

    $current_processed_file = &$this->objectToPublish->publication_log->files[$this->currentProcessedFile];
    $current_processed_file->progress = $progress;


    $drush_msg = 'Upload progress: ' . $current_processed_file->progress . ' for ' . $current_processed_file->filename . ' (' . $current_processed_file->type . ')';

    if ($last_drush_msg != $drush_msg) {
      $this->drushLog($drush_msg, 'notice');
    }
    $last_drush_msg = $drush_msg;


    $this->caclulatePublicationProgress();

    return (0);
  }

  /**
   * Publish a Description
   *
   * @param mixed $resource
   *   the lom resource, can be lom id,
   *   lom object or json_decoded lom object string
   * @param string $auth
   *   the auth key,
   *   optional
   *   default = read current from db
   *
   * @return mixed
   *   return an object on success, else FALSE
   */
  protected function lomApiPublish($resource, $auth = '') {
    $this->lomApiAuth($auth_token, TRUE);


    if (is_object($resource)) {
      $resource = LomDataSanitization::jsonExport($resource);
    }

    $resource = json_decode($resource, true);
    $national_id = urlencode($resource['lomId']);
    $this->drushLog("Publishing LOM ID : $national_id" , 'info');
    $resource = json_encode($resource);

    $postarr = array(
      'description' => $resource
    );

    $headers = array(
      'X-TOKEN-KEY' => $auth_token,
    );

    $a_url = variable_get('archibald_api_url', 'https://dsb-api.educa.ch/v2');
    $national_url = "$a_url/description";

    list($raw_result, $info) = $this->openUrl(
      "$national_url/$national_id",
      array(),
      'GET',
      $headers
    );

    $this->drushLog("Publishing to : $national_url/$national_id" , 'info');

    // If the resource already exists in the National Catalogue
    if ($info->status == 200) {
      $this->drushLog("Description is already published, using PUT" , 'info');
      list($raw_result, $info) = $this->openUrl(
        "$national_url/$national_id",
        $postarr,
        'PUT',
        $headers
      );
    } else {
      $this->drushLog("Description is not published yet, using POST" , 'info');
      list($raw_result, $info) = $this->openUrl(
        $national_url,
        $postarr,
        'POST',
        $headers
      );
    }

    $result = json_decode($raw_result);

    if (empty($result)) {
      $result = new stdClass();
      $result->message = $raw_result;
    }

    $result->code = $info->status;

    return $result;
  }

  /**
   * Delete a Description
   *
   * @param mixed $resource
   *   the lom resource, can be lom id,
   *   lom object or json_decoded lom object string
   * @param boolean $second_run
   *   if a lom auth fails, delete auth cache and re- run
   *
   * @return boolean
   *   return TRUE on success, else FALSE
   */
  public function lomApiUnpublish(ArchibaldLom $resource, $second_run = FALSE) {
    $this->loadContentPartnerByLomId($resource->lom_id);
    if ($this->lomApiAuth($auth_token, $second_run) == FALSE) {
      watchdog(
        'lom_upload',
        'While unpublishing description "!title" the server throws "acces denied". Please contact your administrator.',
        array(
          '!title' => ArchibaldLom::determinTitle($resource),
          '@lom_id' => $resource->lom_id,
        ),
        WATCHDOG_ERROR,
        'archibald/' . $resource->lom_id
      );

      variable_set('archibald_upload_error', 1);
      return FALSE;
    }

    $headers = array(
      'X-TOKEN-KEY' => $auth_token,
    );

    $a_url = variable_get('archibald_api_url', 'https://dsb-api.educa.ch/v2');
    $national_id = $resource->lom_id;
    if (strpos($national_id, 'archibald###') === false) {
      $national_id = "archibald###" . $national_id;
    }
    $national_id = urlencode($national_id);
    $national_url = "$a_url/description";

    list($raw_result, $info) = $this->openUrl(
      "$national_url/$national_id",
      array(),
      'DELETE',
      $headers
    );

    $result = json_decode($raw_result);
    if (empty($result)) {
      watchdog(
        'lom_upload',
        'While unpublishing description "!title" the server throws: "!message". Please contact your administrator.',
        array(
          '!title' => ArchibaldLom::determinTitle($resource),
          '@lom_id' => $resource->lom_id,
          '!message' => $raw_result,
        ),
        WATCHDOG_ERROR,
        'archibald/' . $resource->lom_id
      );

      return FALSE;
    }

    if ($info->status == 401) {
      // Authentication timed out, please revalidate.
      // cause of our first call is $this->lomApiAuth(),
      // this should never happen
      if ($second_run == FALSE) {
        return $this->lomApiUnpublish($resource, TRUE);
      }
    }

    if ($info->status != 200) {
      watchdog(
        'lom_upload',
        'While unpublishing description "!title" the server throws: !code => !message. Please contact your administrator.',
        array(
          '!title' => ArchibaldLom::determinTitle($resource),
          '@lom_id' => $resource->lom_id,
          '!code' => $info->status,
          '!message' => $raw_result,
        ),
        WATCHDOG_ERROR,
        'archibald/' . $resource->lom_id
      );

      return FALSE;
    }
    else {
      watchdog(
        'content',
        'Description "!title" was unpublished.',
        array(
          '!title' => ArchibaldLom::determinTitle($resource),
          '@lom_id' => $resource->lom_id,
        ),
        WATCHDOG_NOTICE,
        'archibald/' . $resource->lom_id);
    }

    return TRUE;
  }

  /**
   * get informations about upload status of a file to educa fileserver
   *
   * @param integer $fid
   *
   * @return object
   *   filename->string,
   *   filemime->string,
   *   filesize->interger,
   *   url->string,
   *   progress->integer
   */
  public function getFileUploadInformations($fid) {
    $file_server_type = $this->getContentPartnerCredential()->file_server_type;


    $query = db_select('file_managed', 'fm');
    $query->leftJoin(
      'archibald_file',
      'df',
      'df.fid = fm.fid AND df.timestamp = fm.timestamp ' .
      // check if file was upload to this file server
      'AND df.file_server_type = :file_server_type',
      array(
        ':file_server_type' => $file_server_type
      )
    );

    $query->fields('df', array('url'));

    $query->fields('fm',
      array(
        'filename',
        'filemime',
        'filesize',
        'timestamp',
      )
    );

    $query->condition('fm.fid', $fid);

    $query->range(0, 1);
    $infos = $query->execute()->fetchObject();

    // if file was allreday uploaded, we dont need to uploadt it againe
    if (!empty($infos->url) && !empty($file_server_type)) {
      if (drupal_substr($infos->url, 0, 6) == 'error:') {
        // a error while upload happens,
        //  this status could be resetted with drush
        $infos->progress = 1;
      }
      else {
        // this file is allready online
        $infos->progress = 100;
      }
    }
    else {
      // file need to be uploaded
      $infos->progress = 0;
    }

    return $infos;
  }

  /**
   * update file upload information in database
   *
   * @param object $file_info
   */
  protected function setFileUploadInformations($file_info) {
    $file_server_type = $this->getContentPartnerCredential()->file_server_type;

    // delete old record
    db_delete('archibald_file')
      ->condition('fid', $file_info->fid)
      ->condition('file_server_type', $file_server_type)
      ->execute();

    // insert new record
    db_insert('archibald_file')->fields(
      array(
        'fid' => $file_info->fid,
        'timestamp' => $file_info->timestamp,
        'url' => $file_info->url,
        'file_server_type' => $file_server_type,
      )
    )->execute();
  }

  /**
   * calculate the progress of file upload
   */
  private function caclulatePublicationProgress($force = FALSE, $percent = NULL) {
    $last_stats_update = &archibald_static(__FUNCTION__, 0);

    if (!is_null($percent) && (INT) $percent > 1) {
      $this->objectToPublish->publication_progress = (INT) $percent;
    }
    else {
      $sum_size = 0;
      $cur_size = 0;
      foreach ($this->objectToPublish->publication_log->files as $file) {
        if ($file->progress < 1) {
          $file->progress = 0;
        }

        if ($file->progress > 100) {
          $file->progress = 100;
        }

        $sum_size += $file->filesize;
        $cur_size += ($file->filesize / 100 * $file->progress);
      }

      $lom_json = LomDataSanitization::jsonExport($this->lom);
      $sum_size += drupal_strlen($lom_json) * 2;

      $this->objectToPublish->publication_progress = ceil(100 / $sum_size * $cur_size);

      // if lom object was successfully uploaded
      if ($this->objectToPublish->publication_log->lom_status == 'success') {
        $this->objectToPublish->publication_progress = 100;
      }
    }

    if ($force || ($last_stats_update + 5) > time()) {
      $last_stats_update = time();
      $this->updateStats();
    }
  }

  /**
   * update stats at database
   */
  protected function updateStats() {
    $this->drushLog('Updating stats for: ' . $this->objectToPublish->lom_id, 'notice');

    db_update('archibald_lom_stats')->condition('lom_id', $this->objectToPublish->lom_id)->fields(
      array(
        'publication_progress' => ceil($this->objectToPublish->publication_progress),
        'publication_log' => serialize($this->objectToPublish->publication_log),
      )
    )->execute();
  }

  /**
   * make lom object complete to publish
   * it will called internaly without parameter
   * it will use $this->lom insted of first parameter to process
   *
   * @param ArchibaldLom $lom
   *
   * @return ArchibaldLom
   */
  public function completeLomObject($lom = NULL) {
    // if called internaly
    if (empty($lom)) {
      $lom = &$this->lom;
    }


    /*
     * Set logos in vCards
     */
    $meta_metadata = $lom->getMetaMetadata();
    $meta_metadata->sortContribute('date', 'asc');
    $mmd_contributes = $meta_metadata->getContribute();

    $creation_date = NULL;
    if (!empty($mmd_contributes)) {
      // First contribute holds the creation date
      // Set it in MetaMetaData
      $meta_metadata->setCreationDate($mmd_contributes[0]->getDate());

      // We only need the last modifier
      // The unique entry obtained holds the last modified date
      $mmd_contributes = array(array_pop($mmd_contributes));
      $meta_metadata->setLastModifiedDate($mmd_contributes[0]->getDate());

      // Overwrite the Contribute contents
      // At this stage, there is only one contribute, whose vCard holds the last modified date
      $meta_metadata->setContribute($mmd_contributes);
      $lom->setMetaMetadata($meta_metadata);
    }

    return $lom;
  }

  /**
   * proccessing  publication_log->files
   * and replace in lom object all local place holders with urls to files
   */
  protected function replaceUrls() {
    foreach ($this->objectToPublish->publication_log->files as $file) {

      // If we process the lom within the second time or more we need to get the file info again.
      if (empty($file->url)) {
        $upload_infos = $this->getFileUploadInformations($file->fid);
        if (!empty($upload_infos) && !empty($upload_infos->url))  {
          $file->url = $upload_infos->url;
        }
      }

      // erro handling but this should realy never happen
      if (empty($file->url)) {
        watchdog(
          'file_replacing',
          'Error while replacing files in descriptions, no URL for file: @file',
          array(
            '@file' => print_r($file, TRUE),
          ),
          WATCHDOG_ERROR,
          'archibald/' . $this->objectToPublish->lom_id
        );

        $this->drushLog('file_replacing: no url found for file: ' . print_r($file, TRUE), 'error');
        continue;
      }

      switch ($file->type) {
        case 'identifier':
          $this->replaceUrlsIdentifier($file->fid, $file->url);
          break;

        case 'preview_image':
          $this->replaceUrlsPreviewImage($file->fid, $file->url);
          break;

        case 'contribute_person_logo':
          $this->replaceUrlPersonLogo($file->fid, $file->url);
          break;

        case 'content_partner_logo':

          // At this stage, we should have one contributor in the metaData: the person that initially created the description
          $meta_metadata = $this->lom->getMetaMetadata();
          $cp_data = $this->getContentPartnerCredential();

          $contributor = new ArchibaldLomDataContribute();

          $contributor->setDate($meta_metadata->getCreationDate());
          $contributor->setRole(ArchibaldLomDataMetaMetadata::CONTRIBUTE_ROLE_CREATOR);

          $vcard = new ArchibaldAppDataVcard();
          $vcard->setLogoUri($file->url);
          $vcard->setOrganization($cp_data->name);
          $contributor->addEntity(ArchibaldAppDataVcardGenerator::generate($vcard));
          $this->lom->getMetaMetadata()->addContribute($contributor);

          break;

        case 'lifecycle_contributor':
          $this->replaceUrlsLifecycleContributor($file->fid, $file->url);
          break;
      }
    }
  }

  /**
   * parse rights.licence object to html string
   */
  protected function replaceRightsLicence() {
    $rights = $this->lom->getRights();
    $lang_string = $rights->getDescription();

    if ($lang_string instanceof ArchibaldLomDataLangString) {

      foreach (language_list() as $code => $language) {

        if ($language->enabled == FALSE) {
          continue;
        }

        $lizenz = $lang_string->getString($code);
        $lizenz_title = archibald_get_term_by_key($lizenz, 'rights_licenses', FALSE, $code, $lizenz_url);

        if ($lizenz == $lizenz_title) {
          $lizenz_title = '';
        }

        if (!empty($lizenz_title)) {
          if (drupal_substr($lizenz, 0, 1) == '/' || drupal_substr($lizenz, 0, 4) == 'http') {
            $lang_string->setString('<a href="' . $lizenz . '" target="_blank">' . $lizenz_title . '</a>', 'en');
          }
          else {
            if (!empty($lizenz_url)) {
              $lang_string->setString('<a href="' . url($lizenz_url) . '" target="_blank">' . $lizenz_title . '</a>', $code);
            }
            else {
              $lang_string->setString($lizenz_title, $code);
            }
          }
        }
        else {
          $lang_string->setString(check_markup($lizenz, 'markdown'), $code);
        }
        $rights->setDescription($lang_string);
        $this->lom->setRights($rights);
      }
    }
  }

  /**
   * internal processing function to replace relation catalog in lom object
   * convert the VCs to CSs
   *
   */
  private function replaceRelationCatalogs() {
    $relations = $this->lom->getRelation();
    if (empty($relations)) return;

    $end_relations = array();
    foreach ($relations as $relation) {
      $catalog = $relation->getCatalog()->getValue();
      $relation->setCatalog(strtoupper($catalog));
      $end_relations[] = $relation;
    }
    $this->lom->setRelation($end_relations);
  }


  /**
   * internal processing function to replace location array in lom object
   * convert the array to string (using the value). The type is used only in Archibald.
   *
   */
  private function replaceLocations() {
    $t = $this->lom->getTechnical();
    $locations = $t->getLocation();
    if (empty($locations)) return;

    $end_locations = array();
    foreach ($locations as $location) {
      $end_locations[] = $location->getValue();
    }
    $t->setLocation($end_locations);
    $this->lom->setTechnical($t);
  }

  /**
   * internal processing function to replace urls in lom object
   * replace the identifier urls
   *
   * @param integer $fid
   * @param url $url
   */
  private function replaceUrlsIdentifier($fid, $url) {
    $general = $this->lom->getGeneral();
    foreach ($general->getIdentifier() as $iid => $identifier) {
      if ($identifier->getCatalog() == 'URL') {
        preg_match('/archibald_file\/([0-9]+)\//', $identifier->getEntry(), $old_fid);

        if (!empty($old_fid[1]) && $old_fid[1] == $fid) {
          $identifier->setEntry($url);
          $general->setIdentifier($identifier, $iid);
        }
      }
    }
    $this->lom->setGeneral($general);
  }

  /**
   * internal processing function to replace urls in lom object
   * replace the preview image url
   *
   * @param integer $fid
   * @param url $url
   */
  private function replaceUrlsPreviewImage($fid, $url) {
    $i = $this->lom->getTechnical()->getPreviewImage();
    $i->setImage($url);
    $this->lom->getTechnical()->setPreviewImage($i);
  }

  /**
   * internal processing function to replace urls in lom object
   * replace the company logo urls
   *
   * @param integer $fid
   * @param url $url
   */
  private function replaceUrlPersonLogo($fid, $url) {
    $meta_meta = $this->lom->getMetaMetadata();
    foreach ($meta_meta->getContribute() as $cid => $contribute) {
      $contribute_was_changed = FALSE;
      foreach ($contribute->getEntity() as $eid => $entity) {
        if (empty($entity)) {
          continue;
        }
        $parser       = new ArchibaldAppDataVcard_Parser($entity);
        $vcard        = $parser->parse();
        $vcard_object = $vcard[0];
        /* @var $vcard_object ArchibaldAppDataVcard */

        // add logo to each  metaMetaData -> contributor -> entity vCard
        if (!empty($vcard_object->logo->uri) || !empty($vcard_object->logo->binary)) {
          // indivual logo
          preg_match('/archibald_file\/([0-9]+)\//', $vcard_object->logo->uri, $old_fid);

          // echo "\n" . "### BEGIN replaceUrlPersonLogo ###" . "\n";
          // print_r( $vcard_object->logo );
          // echo "\n";
          // print_r( $old_fid );
          // echo "\n" . "### END replaceUrlPersonLogo ###" . "\n";

          if (!empty($old_fid[1]) && $old_fid[1] == $fid) {
            $vcard_object->setLogoUri($url);
            $vcard_string = ArchibaldAppDataVcardGenerator::generate($vcard_object);
            $contribute->setEntity($vcard_string, $eid);
            $contribute_was_changed = TRUE;
          }
        }
        else {
          //default logo
          $vcard_object->setLogoUri($url);

          $vcard_string = ArchibaldAppDataVcardGenerator::generate($vcard_object);

          $contribute->setEntity($vcard_string, $eid);
          $contribute_was_changed = TRUE;
        }
      }

      if ($contribute_was_changed == TRUE) {
        $meta_meta->setContribute($contribute, $cid);
      }
    }
    $this->lom->setMetaMetadata($meta_meta);
  }

  /**
   * internal processing function to replace urls in lom object
   * replace the lifecycle.contributor logo.uri s
   *
   * @param integer $fid
   * @param url $url
   */
  private function replaceUrlsLifecycleContributor($fid, $url) {
    $life_cycle = $this->lom->getLifeCycle();
    foreach ($life_cycle->getContribute() as $cid => $contribute) {
      $contribute_was_changed = FALSE;
      foreach ($contribute->getEntity() as $eid => $entity) {
        $parser       = new ArchibaldAppDataVcard_Parser($entity);
        $vcard        = $parser->parse();
        $vcard_object = $vcard[0];

        if (!empty($vcard_object->logo->uri)) {
          preg_match('/archibald_file\/([0-9]+)\//', $vcard_object->logo->uri, $old_fid);

          if (!empty($old_fid[1]) && $old_fid[1] == $fid) {
            $vcard_object->setLogoUri($url);
            $vcard_string = ArchibaldAppDataVcardGenerator::generate($vcard_object);
            $contribute->setEntity($vcard_string, $eid);
            $contribute_was_changed = TRUE;
          }
        }
      }

      if ($contribute_was_changed == TRUE) {
        $life_cycle->setContribute($contribute, $cid);
      }
    }
    $this->lom->setLifeCycle($life_cycle);
  }

  private function drushLog($msg, $type='notice') {
    // watchdog( 'archibald_file_upload', $msg );

    if (function_exists('drush_log')) {
      drush_log($msg, $type);
    }

    switch ($type) {
      case 'error':
      case 'notice':
        return FALSE;
      default:
        return TRUE;
    }
  }

  /**
   * Append the content partner logo file to the uploaded one.
   */
  private function add_content_partner_contributor() {
    // companyLogo   metaMeta.contributor
    $content_partner = $this->getContentPartnerCredential();
    if (!empty($content_partner)) {
      $company_logo = $content_partner->icon_fid;

      if (!empty($company_logo)) {
        $upload_infos = $this->getFileUploadInformations($company_logo);
        $upload_infos->fid = $company_logo;
        $upload_infos->type = 'content_partner_logo';
        $this->objectToPublish->publication_log->files[$upload_infos->fid] = $upload_infos;
      }
    }
  }
}

