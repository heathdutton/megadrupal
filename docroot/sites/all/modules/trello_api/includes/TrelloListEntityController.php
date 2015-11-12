<?php
/**
 * @file
 * Contains the TrelloListEntityController class.
 */

/**
 * Class for the TrelloListEntityController.
 */
class TrelloListEntityController extends TrelloAPIEntityController {

  /**
   * Handle build content for TrelloList entities.
   */
  public function buildContent($list, $view_mode = 'default', $langcode = NULL, $content = array()) {
    $cards = $list->getCards();
    foreach ($cards as $card) {
      $content['cards'][] = $card->view();
    }
    return parent::buildContent($list, $view_mode, $langcode, $content);
  }

}
