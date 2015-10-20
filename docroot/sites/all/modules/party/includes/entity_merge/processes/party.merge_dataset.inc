<?php
/**
 * @file
 * Contains merge process for merging a data set.
 */

class PartyMergeProcessMergeDataSet extends EntityMergeProcessBase {

  /**
   * Overrides EntityMergeProcessBase::registerSubProcesses()
   */
  public function registerSubProcesses() {
    $data_set_name = $this->conf['data_set_name'];
    $info = party_get_data_set_info($data_set_name);
    $class = $info['class'];

    $controller1 = $this->entity1->getDataSetController($data_set_name);
    $controller2 = $this->entity2->getDataSetController($data_set_name);
    $processes = $class::registerMergeProcesses($controller1, $controller2);

    return $processes;
  }

  /**
   * Overrides EntityMergeProcessBase::run()
   */
  public function run() {
    $data_set_name = $this->conf['data_set_name'];
    $info = party_get_data_set_info($data_set_name);
    $class = $info['class'];

    $controller1 = $this->entity1->getDataSetController($data_set_name);
    $controller2 = $this->entity2->getDataSetController($data_set_name);
    $class::runMerge($controller1, $controller2);
  }
}