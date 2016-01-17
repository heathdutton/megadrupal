<?php

/**
 * @file
 * File integration for the media_theplatform_mpx module.
 */

/**
 * Implements hook_file_mimetype_mapping_alter().
 */
function media_theplatform_mpx_file_mimetype_mapping_alter(&$mapping) {
  $mapping['mimetypes'][] = 'player/mpx';
  $mapping['mimetypes'][] = 'video/mpx';
}

/**
 * Implements hook_file_entity_access().
 */
function media_theplatform_mpx_file_entity_access($op, $file, $account) {
  if ($op == 'download' && ($file->filemime == 'player/mpx' || $file->filemime == 'video/mpx')) {
    // Mpx players and videos are not downloadable.
    return FILE_ENTITY_ACCESS_DENY;
  }
}

/**
 * Implements hook_file_delete().
 */
function media_theplatform_mpx_file_delete($file) {
  if ($file->filemime === 'player/mpx') {
    // Deleting a mpxPlayer:
    // This is assuming that you know you're doing.
    // If a user tries to delete through the Media forms, they can't submit if
    // the mpxPlayer is referenced by other mpxMedia. If they're just deleting
    // this record in the database or thru PHP, well, it could kill other
    // mpxMedia Files that reference it and then good luck to you.
    db_delete('mpx_player')->condition('fid', $file->fid)->execute();
  }
  elseif ($file->filemime === 'video/mpx') {
    // Deleting a Video:
    /** @var MediaThePlatformMpxStreamWrapper $wrapper */
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $parts = $wrapper->get_parameters();

    // Check for other mpxMedia Files that also reference this mpx_video.
    $guid = $parts['mpx_id'];

    // Delete any thumbnails in file system that may exist.
    _media_theplatform_mpx_delete_video_images(array('guid' => $guid));

    if ($alt_fid = db_query_range("SELECT fid FROM {file_managed} WHERE fid <> :fid AND uri LIKE :uri", 0, 1, array(':fid' => $file->fid, ':uri' => db_like('mpx://m/' . $guid . '/') . '%'))->fetchField()) {
      // If other files exist, update mpx_video record with the next fid.
      db_update('mpx_video')
        ->fields(array('fid' => $alt_fid))
        ->condition('fid', $file->fid)
        ->execute();
      entity_get_controller('file')->resetCache(array($alt_fid));
      watchdog('media_theplatform_mpx', 'Reassigned mpx video data for @guid from deleted file @fid to file @alt-fid.', array('@guid' => $guid, '@fid' => $file->fid, '@alt-fid' => $alt_fid));
    }
    else {
      // Otherwise delete the mpx video record.
      db_delete('mpx_video')->condition('fid', $file->fid)->execute();
    }
  }
}
