<?php

namespace Drupal\campaignion_manage\BulkOp;

class BatchBase {
  public function __construct(&$data) {}
  public function apply($contact, &$result) {}
  public function commit() {}
}
