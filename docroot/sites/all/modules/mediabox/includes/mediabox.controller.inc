<?php

/**
 * @file
 * Mediabox entity controller.
 */
class MediaboxController extends EntityAPIController {

  /**
   * Load mediabox entity by fid.
   *
   * @param $fid
   * @return mediabox
   */
  public function loadByFid($fid) {
    $id = db_select('mediabox', 'mb')
      ->fields('mb')
      ->condition('fid', $fid)
      ->execute()->fetchColumn();

    return array_shift(parent::load(array($id)));
  }

  public function create(array $values = array()) {

    global $user;

    $values += array(
      'title' => '',
      'description' => '',
      'caption' => '',
      'alternate_text' => '',
      'meta_dimensions' => '0x0',
      'created' => REQUEST_TIME,
      'modified' => REQUEST_TIME,
      'user_id' => $user->uid,
    );

    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

      // Make Description and Status themed like default fields.
      $content['description'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' => t('Description'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'text',
        '#entity_type' => 'example_task',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->description)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->description))
      );

      return parent::buildContent($entity, $view_mode, $langcode, $content);
    }
}
