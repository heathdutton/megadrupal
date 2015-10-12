<?php
/**
 * @file
 * Hooks provided by the media_translation module.
 */

/**
 * Notifies other modules that the translation set for a media file has been updated.
 *
 * @param $fid
 *   The File ID of the updated media file.
 * @param $tsid
 *   The Translation Set ID of the updated media file.
 * @param $translation_set
 *   The actual updated translation set object.
 * @param $translations
 *   An array of all of the translations in the translation set with the following structure:
 *   $translations = array(
 *      'languagecode' => array(
 *        'fid' => fid,
 *        'filename' => filename,
 *        'file' => file object
 *      )
 *    );
 */
function hook_media_translation_updated($fid, $tsid, $translation_set, $translations) {

}