<?php

/**
 * @file
 * Class that defines a Drupal file Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal;

use Drupal\maps_import\Exception\MappingException;
use Drupal\maps_import\Mapping\Mapping;
use Drupal\maps_import\Converter\ConverterInterface;

class File extends Entity {

  /**
   * @inheritdoc
   */
  protected function getTranslationClass() {
    return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\FieldTranslation';
  }

  /**
   * @inheritdoc
   */
  protected function createEntity($languageId, $entityId = NULL) {
    // Get media information from database.
    $entity = Mapping::getCurrentEntity();
    $object = $entity->getRelatedEntities();

    // @todo should we provide support for translatable files?
    $object = reset($object);
    $entity->setUpdated($object['updated']);

    if (!isset($object['filename']) || ! strlen($object['filename'])) {
      throw new MappingException('The media filename is missing.');
    }

    if (!$wrapper = parent::createEntity($languageId, $entityId)) {
      throw new MappingException("Cannot create the file metadata wrapper.");
    }

    $converter_options = $this->getConverter()->getOptions();

    // Preset management
    $options = $this->getProfile()->getOptions();
    $preset = '';
    if ($this->getBundle() === 'image') {
      $preset = 'original/';
      if (empty($converter_options['file_management']) || $converter_options['file_management'] == 'local') {
        $presets = maps_import_image_presets($this->getProfile());
        $media_settings = $options['media_settings'][$object['type']];
        $preset = (!empty($presets[$media_settings['preset']]) ? $presets[$media_settings['preset']] : '') . '/';
      }
    }

    // Media type management.
    $media_types = maps_import_get_maps_media_types($this->getProfile());

    $uri = $this->getConverter()->getMapping()->getFileUri($options['media_settings'][$object['type']]['path'], $preset, $object['filename']);

    // @todo by using this method, we can not manage any preset.
    if (!empty($converter_options['file_management']) && $converter_options['file_management'] == 'download') {
      $existingEntity = db_select('maps_import_media_ids', 'mimi')
        ->fields('mimi', array('updated'))
        ->condition('maps_id', $entity->getId())
        ->execute()
        ->fetch();

      if (!$existingEntity || $existingEntity->updated < $entity->getUpdated()) {
        $dir = dirname($uri);

        if (!file_prepare_directory(drupal_realpath($dir), FILE_CREATE_DIRECTORY)) {
          throw new MappingException('The media @uri does not exist.', 0, array('@uri' => $uri));
        }

        // Construct the remote URI with security token.
        $exploded_url = explode("?", $object['url']);
        $remote_uri = array_shift($exploded_url);
        if (!strpos($remote_uri, $this->getProfile()->getToken())) {
          $remote_uri = str_replace("/media/", "/media/{$this->getProfile()->getToken()}/", $remote_uri);
        }

        $copy = $this->fileCopy($remote_uri, $uri);
      }
    }

    $file = $wrapper->value();

    // Remove the fid property, so if it's a new File, Drupal make an insert request.
    if (isset($file->fid)) {
      unset($file->fid);
    }

    if (empty($uri)) {
    	return NULL;
    }

    if (!file_exists($uri)) {
      throw new MappingException('The media @uri does not exist.', 0, array('@uri' => $uri));
    }

    // Is this file already stored in the managed files?
    if (empty($file->fid) && $fid = $this->getExistingManagedFiles($uri)) {
      $file->fid = $fid;
    }

    // @todo Handle file user property.
    $file->filename = $object['filename'];
    $file->uri = $uri;
    $file->filemime = file_get_mimetype($uri);
    $file->filesize = filesize($uri);
    $file->status  = FILE_STATUS_PERMANENT;

    return $wrapper;
  }

  /**
   * Get the existing managed files.
   *
   * @param $uri
   *   The condition uri.
   */
  protected function getExistingManagedFiles($uri) {
    $result = db_select('file_managed')
      ->fields('file_managed')
      ->condition('uri', $uri)
      ->execute()
      ->fetchAll();

    if (empty($result)) {
      return FALSE;
    }

    $file = reset($result);
    return $file->fid;
  }

  /**
   * Copies a file from a remote server to the
   * local server.
   *
   * @param $source
   *   The source uri.
   * @param $dest
   *   The destination uri.
   *
   * @return bool
   *   Whether the copy is successful.
   */
  protected function fileCopy($source, $dest) {
    if (!fopen($source, 'r')) {
      return FALSE;
    }

    $real_dest = drupal_realpath($dest);

    if (!copy($source, $dest)) {
      if ($real_dest === FALSE ||
        !copy($source, $real_dest)) {
        return FALSE;
      }
    }

    drupal_chmod($dest);

    return TRUE;  
  }  
    
}
