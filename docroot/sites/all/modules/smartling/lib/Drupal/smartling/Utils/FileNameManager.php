<?php

/**
 * @file
 * Smartling log.
 */

namespace Drupal\smartling\Utils;

/**
 * Class FileNameManager.
 */
class FileNameManager {
  protected $file_type_map;

  public function __construct($file_type_map, $drupal_api_wrapper) {
    $this->file_type_map = $file_type_map;
    $drupal_api_wrapper->alter('smartling_file_type_map', $this->file_type_map);
  }

  public function buildFileName($submission) {
    $entity_type = $submission->getEntityType();
    if (isset($this->file_type_map[$entity_type]) && $this->file_type_map[$entity_type] == 'gettext') {
      $file_name = 'smartling_interface_translation_' . $submission->getBundle() . '.pot';
    }
    else {
      $file_name = strtolower(trim(preg_replace('#\W+#', '_', $submission->getTitle()), '_')) . '_' . $submission->getEntityType() . '_' . $submission->getRID() . '.xml';
    }
    return $file_name;
  }


  public function buildTranslatedFileName($submission) {
    $file_name = $submission->getFileName();
    $file_name = substr($file_name, 0, strlen($file_name) - 4);

    $extension = ($submission->getEntityType() == 'smartling_interface_entity') ? 'po' : 'xml';

    return $file_name . '_' . $submission->getTargetLanguage() . '.' . $extension;
  }

}
