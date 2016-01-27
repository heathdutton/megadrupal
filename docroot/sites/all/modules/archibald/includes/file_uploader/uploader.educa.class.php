<?php

/**
 * @file
 * Provides the default upload handler to the educa file server
 */

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * file uploader to educa file server
 * used by lom publish
 *
 * @author Heiko Henning h.henning@educa.ch
 */
class ArchibaldUploaderEduca extends ArchibaldUploaderCurlBased implements ArchibaldUploaderInterface {

  /**
   * get the name of the uploader
   *
   * @return array
   *  first value is the computer readable code
   *  second value is the human readable text
   */
  public function getUploaderName() {
    return array('educa' => t('Educa file Server'));
  }

  /**
   * get the description of the uploader
   *
   * @return string
   *   html
   */
  public function getUploaderDescription() {
    return t('Upload files to educa.ch file server.') . '<br />' . t('In order to use this function you need to have an agreement with educa.ch');
  }

  /**
   * get settings configuration form
   *
   * @param array $form
   * @param array $form_state
   *
   * @return array
   *   drupal form
   */
  public function getSettingsForm($form, &$form_state, $content_partner_id) {
    //Educa file uploader needs always to be configured for non identifier
    //uploads, so this is empty array.
    return array();
  }

  /**
   * validate configuration form
   *
   * @param array $form
   * @param array $form_state
   */
  public function validateSettingsForm($form, &$form_state) {}

  /**
   * submit configuration form
   *
   * @param array $form
   * @param array $form_state
   */
  public function submitSettingsForm($form, &$form_state, $content_partner_id) {}

  /**
   * upload a single file to educa file server
   * @see http://wiki.ubuntu/digitale_schulbibliothek/api/doku/command/file_upload
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
  public function uploadFile($content_partner_id, $file_real_path, $fid, $callback, &$object_to_publish) {
    // watchdog('lom_upload', 'Uploading file: @message',
    //   array(
    //     '@message' =>  print_r($object_to_publish, 1)
    //   ),
    //   WATCHDOG_INFO
    // );

    watchdog('lom_upload', 'Uploading file : @file_real_path (@fid)',
      array(
        '@file_real_path' =>  $file_real_path,
        '@fid' =>  $fid,
      ),
      WATCHDOG_INFO
    );

    $this->objectToPublish = $object_to_publish;
    $this->lomApiAuth($auth_token);

    $headers = array(
      'X-TOKEN-KEY' => $auth_token,
    );

    $postarr = array(
      'file' => '@' . $file_real_path,
    );

    $this->parentCallbackMethode = $callback;

    $a_url = variable_get('archibald_api_url', 'https://dsb-api.educa.ch/v2');
    list($result_raw, $info) = $this->openUrl(
      $a_url . '/file',
      $postarr,
      'POST',
      $headers
    );

    $result_object = json_decode($result_raw);
    if (empty($result_raw) || empty($result_object)) {
      return (object)array('code' => 9999, 'message' => $result_raw);
    }

    $result_object->code = $info->status;

    return $result_object;
  }

  /**
   * Deletes the given file
   *
   * @param object $file
   *   The file object
   *
   * @return boolean
   */
  public function deleteFile($file) {

  }
}

