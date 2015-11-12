<?php
/**
 * @file
 * Question controller class.
 */

class ConsultQuestionController extends ConsultInterviewSubEntityController {
  /**
   * Overrides buildContent to add question text.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    $build['question_text'] = array(
      '#markup' => $entity->getQuestionText(TRUE),
    );
    return $build;
  }

  /**
   * Remove interview related values from sub entities.
   */
  public function export($entity, $prefix = '') {
    $vars = get_object_vars($entity);
    unset($vars[$this->statusKey], $vars[$this->moduleKey], $vars['is_new']);
    if ($this->nameKey != $this->idKey) {
      unset($vars[$this->idKey]);
    }

    // Remove interview data that was added to the sub entity for easy access.
    unset($vars['interview']);
    unset($vars['weight']);
    unset($vars['enabled']);
    unset($vars['shown']);

    return entity_var_json_export($vars, $prefix);
  }
}
