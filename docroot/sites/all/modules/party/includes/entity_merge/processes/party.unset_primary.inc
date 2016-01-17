<?php
/**
 * @file
 * Contains merge process for unsetting primary emails.
 */

class PartyMergeProcessUnsetEmail extends EntityMergeProcessBase {

  /**
   * Overrides EntityMergeProcessBase::run()
   */
  public function run() {
    // Unset primary email field ready for merging.
    $ids = array($this->entity1->pid, $this->entity2->pid);
    db_update('party')
      ->fields(array(
        'email' => NULL,
      ))
      ->condition('pid', $ids)
      ->execute();
  }
}
