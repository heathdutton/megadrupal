<?php
/**
 * @file
 * Contains the TrelloLabelEntityController class.
 */

/**
 * Class for the TrelloLabelEntityController.
 */
class TrelloLabelEntityController extends TrelloAPIEntityController {

  /**
   * Implements EntityAPIControllerInterface.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    $view = parent::view($entities, $view_mode, $langcode, $page);
    foreach ($view['trello_api_label'] as &$build) {
      $build['#theme'] = 'trello_api_label';
      $build['#trello_api_label'] = $build['#entity'];
      unset($build['#entity']);
    }
    return $view;
  }

}
