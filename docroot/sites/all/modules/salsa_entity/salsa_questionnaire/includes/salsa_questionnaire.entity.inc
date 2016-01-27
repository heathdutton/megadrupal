<?php

/**
 * @file
 * Definition of SalsaEntityQuestionnaire.
 */

/*
 * SalsaEntityQuestionnaire class.
 */
class SalsaEntityQuestionnaire extends SalsaEntity {

  /**
   * Overrides SalsaEntity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    if (isset($this->Title)) {
      $content['Title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>' . t($this->Title) . '</h2>',
      );
    }

    if (isset($this->Description)) {
      $content['Description'] = array(
        '#type' => 'item',
        '#markup' => '<div>' . t($this->Description) . '</div>',
      );
    }

    $content['questionnaire_form'] = drupal_get_form('salsa_questionnaire_form', $this);

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
}
