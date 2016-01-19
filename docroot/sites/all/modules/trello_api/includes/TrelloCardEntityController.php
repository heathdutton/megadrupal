<?php
/**
 * @file
 * Contains the TrelloCardEntityController class.
 */

/**
 * Class for the TrelloCardEntityController.
 */
class TrelloCardEntityController extends TrelloAPIEntityController {

  /**
   * Handle build content for TrelloCard entities.
   */
  public function buildContent($card, $view_mode = 'default', $langcode = NULL, $content = array()) {
    if (!empty($card->labels)) {
      foreach ($card->labels as $label) {
        $content['labels'][] = $label->view();
      }
    }
    if (!empty($card->description)) {
      $content['description'] = array(
        '#theme' => 'html_tag',
        '#tag' => 'p',
        '#value' => check_plain($card->description),
      );
    }
    return parent::buildContent($card, $view_mode, $langcode, $content);
  }
}
