<?php

namespace Drupal\campaignion\CRM\Import\Source;

class WebformSubmission extends \Drupal\little_helpers\Webform\Submission implements SourceInterface {

  public function value($key) {
    return $this->valueByKey($key);
  }
}
