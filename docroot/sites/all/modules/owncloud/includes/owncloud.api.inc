<?php

const OWNCLOUD_TYPE_VALIDATE = 'validate';

const OWNCLOUD_TYPE_CHECK_DELETE = 'file_check_delete';

const OWNCLOUD_TYPE_CREATED = 'file_created';
const OWNCLOUD_TYPE_CHANGED = 'file_changed';
const OWNCLOUD_TYPE_RENAMED = 'file_renamed';
const OWNCLOUD_TYPE_DELETED = 'file_deleted';
const OWNCLOUD_TYPE_RESTORED = 'file_restored';

function owncloud_api($hash) {
  $response = array();
  try {
    $inputJSON = file_get_contents('php://input');
    if (empty($inputJSON)) {
      throw new Exception('Can not read input');
    }

    if (owncloud_hash($inputJSON) != $hash) {
      $response['msg'] = 'Invalid request';
    }
    else {
      $input = drupal_json_decode($inputJSON);
      if (!is_array($input) || empty($input)) {
        throw new Exception('Json input is empty');
      }

      if ($input['activity'] == OWNCLOUD_TYPE_VALIDATE) {
        $response['hash'] = owncloud_hash($input['token']);
      }
      else {
        $file = owncloud_file_load_by_remote_id($input['id'], $input['name']);
        $file->owncloud['filesize'] = isset($input['size']) ? $input['size'] : $file->filesize;

        switch ($input['activity']) {
          case OWNCLOUD_TYPE_CHECK_DELETE:
            $usages = file_usage_list($file);
            $response['used'] = !empty($usages);
            break;

          /** @noinspection PhpMissingBreakStatementInspection */
          case OWNCLOUD_TYPE_RENAMED:
            $file->filename = $input['name'];
            $file->filemime = file_get_mimetype($file->filename);

          case OWNCLOUD_TYPE_CREATED:
          case OWNCLOUD_TYPE_CHANGED:
          case OWNCLOUD_TYPE_RESTORED:
            file_save($file);
            break;

          case OWNCLOUD_TYPE_DELETED:
            // We don't use file_delete() here because the file has already been
            // deleted and hence that would fail. We only want to delete the
            // database record.
            db_delete('file_managed')->condition('fid', $file->fid)->execute();
            db_delete('file_usage')->condition('fid', $file->fid)->execute();
            entity_get_controller('file')->resetCache();
            break;

        }
        // Flush all image derivatives to make sure we deliver the correct one
        // next time this file is being requested.
        image_path_flush($file->uri);
      }
    }
  }
  catch (Exception $ex) {
    watchdog('ownCloud', $ex->getMessage(), array(), WATCHDOG_ERROR);
    header("HTTP/1.1 502 Error");
    $response['error'] = TRUE;
    $response['error message'] = $ex->getMessage();
  }
  drupal_json_output($response);
  drupal_exit();
}
