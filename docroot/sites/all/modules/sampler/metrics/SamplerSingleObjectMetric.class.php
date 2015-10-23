<?php


/**
 * @file
 * Class for comments metric.
 */

class SamplerSingleObjectMetric extends SamplerMetric {

  // Since this metric only has one object ID, provide that here as a constant.
  const OBJECT_ID = 1;

  public function trackObjectIDs() {
    // We're tracking the metric across all objects, which only ever
    // has one object_id, so we just hard code it.
    $object_ids = array(self::OBJECT_ID);
    return $object_ids;
  }
}
