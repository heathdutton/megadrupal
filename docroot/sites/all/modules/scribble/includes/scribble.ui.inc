<?php

/**
 * @file
 * Definition of ShareMessageController and ShareMessageUIController classes.
 */

/**
 * Controller class for the ShareMessage entity.
 */
class ScribbleController extends EntityAPIControllerExportable {

  /**
   * Overrides EntityAPIController::create().
   */
  public function create(array $values = array()) {
    $values += array(
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );
    return parent::create($values);
  }

  /**
   * Overrides EntityAPIController::buildContent().
   *
   * Don't trigger the view rule because the event is not defined.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    // Remove previously built content, if exists.
    $entity->content = $content;
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->language;

    // Add in fields.
    if (!empty($this->entityInfo['fieldable'])) {
      // Perform the preparation tasks if they have not been performed yet.
      // An internal flag prevents the operation from running twice.
      $key = isset($entity->{$this->idKey}) ? $entity->{$this->idKey} : NULL;
      field_attach_prepare_view($this->entityType, array($key => $entity), $view_mode);
      $entity->content += field_attach_view($this->entityType, $entity, $view_mode, $langcode);
    }
    module_invoke_all($this->entityType . '_view', $entity, $view_mode, $langcode);
    module_invoke_all('entity_view', $entity, $this->entityType, $view_mode, $langcode);
    $build = $entity->content;
    unset($entity->content);
    return $build;
  }

}

/**
 * UI Controller class for the ShareMessage entity.
 */
class ScribbleUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::overviewTableRow().
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
    $row[] = l(t('Image list'), $this->path . '/' . $id . '/image-list');
    $row[] = l(t('Slideshow'), 'scribble/' . $id . '/slideshow');
    $row[] = l(t('Download'), 'scribble/' . $id . '/download');
    $row[] = l(t('Blackboard'), 'scribble/' . $id);
    return $row;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableHeaders().
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $headers = parent::overviewTableHeaders($conditions, $rows, $additional_header);
    $headers[] = t('Manage images');
    $headers[] = t('Slideshow');
    $headers[] = t('Download images');
    $headers[] = t('Blackboard link');
    return $headers;
  }

}
