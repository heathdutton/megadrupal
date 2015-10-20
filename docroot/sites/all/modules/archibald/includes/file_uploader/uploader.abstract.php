<?php

/**
 * @file
 * interfaces for file uploader used by lom publish
 */
module_load_include('php', 'archibald', 'includes/dsb_api.class');

/**
 * interfaces for file uploader used by lom publish
 *
 * @author Heiko Henning h.henning@educa.ch
 */
interface ArchibaldUploaderInterface {

  /**
   * get the name of the uploader
   *
   * @return array
   *  first value is the computer readable code
   *  second value is the human readable text
   */
  public function getUploaderName();

  /**
   * get the description of the uploader
   *
   * @return string
   *   html
   */
  public function getUploaderDescription();

  /**
   * get settings configuration form
   *
   * @param array $form
   * @param array $form_state
   *
   * @return array
   *   drupal form
   */
  public function getSettingsForm($form, &$form_state, $content_partner_id);

  /**
   * validate configuration form
   *
   * @param array $form
   * @param array $form_state
   */
  public function validateSettingsForm($form, &$form_state);

  /**
   * submit configuration form
   *
   * @param array $form
   * @param array $form_state
   */
  public function submitSettingsForm($form, &$form_state, $content_partner_id);

  /**
   * upload a single file
   *
   * @param int $content_partner_id the content partner id which want to upload
   * @param string $file_real_path
   *   full path to the file to upload
   * @param array $callback
   * @param object $object_to_publish
   *
   * @return object
   *
   *   code integer
   *     99   error ssl key problems
   *     501  error server throws wrong parameter
   *     405  error server throws file type not allowed
   *     413  error server throws file too big
   *     502  error server access denied
   *     xxx  error unknown error
   *     200  upload successfull
   *
   *   message string
   *     will displayed if code is not one of the defined
   *
   *   url string
   *     public url to access uploaded files
   *     and have to be set if code=200
   */
  public function uploadFile($content_partner_id, $file_real_path, $fid, $callback, &$object_to_publish);

  /**
   * Deletes the given file
   *
   * @param object $file
   *   The file object
   *
   * @return boolean
   */
  public function deleteFile($file);
}

/**
 * abstract class for libCurl based uploader
 *
 * @author Heiko Henning h.henning@educa.ch
 */
abstract class ArchibaldUploaderCurlBased extends ArchibaldAbstractDsbApi {

  /**
   * array to use with call_user_func()
   * @var array
   */
  protected $parentCallbackMethode = NULL;

  /**
   * curl upload/download progress callback function
   *
   * @param integer $download_size
   *   bytes of data to download
   * @param integer $downloaded
   *   current downloaded bytes
   * @param integer $upload_size
   *   bytes of data to upload
   * @param integer $uploaded
   *   current uploaded bytes
   *
   * @return boolean
   */
  protected function uploadFileProgress($download_size, $downloaded, $upload_size, $uploaded) {

    $progress = 0;

    if ((!empty($download_size) || !empty($upload_size))) {
      $progress = 100 / ($upload_size + $download_size) * ($uploaded + $downloaded);

      if ($progress < 1) {
        $progress = 0;
      }

      if ($progress > 100) {
        $progress = 100;
      }
    }

    if (!empty($this->parentCallbackMethode)) {
      call_user_func_array($this->parentCallbackMethode, array($progress));
    }

    return 0;
  }

  /**
   * placebo needed because of archibald_api ectension
   */
  public function cron() {}
}

