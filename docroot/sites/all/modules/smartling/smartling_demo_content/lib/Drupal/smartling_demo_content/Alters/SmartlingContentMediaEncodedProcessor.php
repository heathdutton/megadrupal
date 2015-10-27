<?php

/**
 * @file
 * Class SmartlingContentMediaEncodedProcessor.
 */

namespace Drupal\smartling_demo_content\Alters;

use Drupal\smartling\Alters\SmartlingContentProcessorInterface;

/**
 * Demo url processor. No real value here for now.
 */
class SmartlingContentMediaEncodedProcessor implements SmartlingContentProcessorInterface {

  /**
   * Get file by id.
   *
   * @param int $fid
   *   File id.
   *
   * @return mixed
   *   Return file object.
   */
  protected function getFileById($fid) {
    // entity_load('file', FALSE, array('fid' => $fid));
    return file_load($fid);
  }

  /**
   * Get file by name.
   *
   * @param string $fname
   *   File name.
   *
   * @return mixed
   *   Return file object.
   */
  protected function getFileByName($fname) {
    return entity_load('file', FALSE, array('filename' => $fname));
  }

  /**
   * Process.
   *
   * @param string $item
   *   Item.
   * @param mixed $context
   *   Context.
   * @param string $lang
   *   Language.
   * @param string $field_name
   *   File name.
   * @param object $entity
   *   Entity object.
   */
  public function process(&$item, $context, $lang, $field_name, $entity) {
    $file = $this->getFileById($context[0][0][0]->fid);
    if (empty($file)) {
      return;
    }
    $new_file = $this->getFileByName($lang . '_' . $file->filename);

    if ($new_file) {
      $media_content = json_decode(htmlspecialchars_decode($item[1]));
      $media_content[0][0]->fid = $new_file[0]->fid;
      $item[1] = htmlspecialchars(json_encode($media_content));
    }
  }
}
