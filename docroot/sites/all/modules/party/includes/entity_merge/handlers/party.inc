<?php
/**
 * @file
 * The merge handler for merging two parties together.
 */

/**
 * Merge two Parties together.
 */
class PartyMergeHandler extends EntityMergeHandlerDefault {

  /**
   * Overrides EntityMergeHandlerDefault::registerSubProcesses().
   */
  public function registerSubProcesses() {
    $processes = array();
    $processes['unset_email'] = array(
      'class' => 'PartyMergeProcessUnsetEmail',
      'weight' => -40,
    );

    $processes['merge_fields'] = array(
      'class' => 'EntityMergeProcessMergeFields',
      'weight' => -5,
    );

    foreach (party_get_data_set_info() as $data_set_name => $info) {
      $processes['merge_data_set_' . $data_set_name] = array(
        'class' => 'PartyMergeProcessMergeDataSet',
        'weight' => 0,
        'data_set_name' => $data_set_name,
      );
    }

    $processes['invoke_hooks'] = array(
      'class' => 'EntityMergeProcessInvokeHooks',
      'weight' => 10,
    );

    $processes['save_target'] = array(
      'class' => 'EntityMergeProcessSaveTarget',
      'weight' => 20,
    );

    $processes['set_merged_flag'] = array(
      'class' => 'PartyMergeProcessSetHidden',
      'weight' => 30,
    );

    return $processes;
  }
}
