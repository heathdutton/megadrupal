<?php
/**
 * @file
 * Contains merge process for setting the hidden flag.
 */

class PartyMergeProcessSetHidden extends EntityMergeProcessBase {

  /**
   * Overrides EntityMergeProcessBase::run()
   */
  public function run() {
    // Mark the second party as merged and save.
    $this->entity2->hidden = TRUE;
    $this->entity2->merged_party = $this->entity1->pid;
    $this->entity2->save();
  }
}
