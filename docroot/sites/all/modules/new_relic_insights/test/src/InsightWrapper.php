<?php

/**
 * @file
 * Contains a wrapper that modifies the way the Insight entity class behaves so
 * that tests will actually run on drupal.org. Bummer.
 */

class InsightWrapper extends Insight {

  protected function setUp() {
    $entity_info = new_relic_insights_entity_info(TRUE);
    $this->entityInfo = $entity_info['insight'];
    $this->idKey = $this->entityInfo['entity keys']['id'];
    $this->nameKey = isset($this->entityInfo['entity keys']['name']) ? $this->entityInfo['entity keys']['name'] : $this->idKey;
    $this->statusKey = empty($info['entity keys']['status']) ? 'status' : $info['entity keys']['status'];
  }


}
