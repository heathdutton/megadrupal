<?php
/**
 * @file
 * Result controller class.
 */

class ConsultResultController extends ConsultInterviewSubEntityController {
  /**
   * Overrides buildContent to add question text.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    $build['question_text'] = array(
      '#markup' => $entity->getMessageText(TRUE),
    );
    return $build;
  }
}
