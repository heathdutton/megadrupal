<?php

/**
 * @file
 * Definition of SalsaSignUpPage class.
 */

/*
 * SalsaSignUpPage class.
 */
class SalsaSignUpPage extends SalsaEntity {

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

    if (isset($this->Header)) {
      $content['Description'] = array(
        '#type' => 'item',
        '#markup' => '<div>' . t($this->Header) . '</div>',
      );
    }

    $content['signup_page_form'] = drupal_get_form('salsa_signup_page_form', $this);

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
}
