<?php

/**
 * @file
 * Definition of SalsaEntityDonatePage class.
 */

/*
 * SalsaEntityDonatePage class.
 */
class SalsaEntityDonatePage extends SalsaEntity {

  /**
   * Overrides SalsaEntity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    // @todo: Re-add header and footer text in a more controllable way.
    /*
    if (isset($this->Header)) {
      $content['header'] = array(
        '#type' => 'item',
        '#markup' => $this->Header,
      );
    }*/

    $content['donate_form'] = drupal_get_form('salsa_donate_page_form', $this, $view_mode);

    /*
     if (isset($this->Footer)) {
      $content['footer'] = array(
        '#type' => 'item',
        '#markup' => $this->Footer,
      );
    }*/

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
}
