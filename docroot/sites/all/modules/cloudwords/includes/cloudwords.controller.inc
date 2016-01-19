<?php

/**
 * @file
 * Contains CloudwordsTranslatableController.
 */

class CloudwordsTranslatableController extends EntityAPIController {

  /**
   * Overrides EntityAPIController::delete().
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    parent::delete($ids, $transaction);

    // Since we are deleting one or multiple translatables, we need to delete
    // the content mappings as well.
    if ($ids) {
      db_delete('cloudwords_content')
        ->condition('ctid', $ids)
        ->execute();
    }
  }

}
