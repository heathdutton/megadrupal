<?php
/**
 * @file
 * Contains the TrelloBoardEntityController class.
 */

/**
 * Class for the TrelloBoardEntityController.
 */
class TrelloBoardEntityController extends TrelloAPIEntityController {

  /**
   * Handle build content for TrelloBoard entities.
   */
  public function buildContent($board, $view_mode = 'default', $langcode = NULL, $content = array()) {
    if ($view_mode == 'archived') {
      $lists = $board->getArchivedLists();
      foreach ($lists as $list) {
        $content['lists'][] = $list->view();
      }
    }
    else {
      $lists = $board->getLists();
      foreach ($lists as $list) {
        $content['lists'][] = $list->view();
      }
    }
    return parent::buildContent($board, $view_mode, $langcode, $content);
  }
}
