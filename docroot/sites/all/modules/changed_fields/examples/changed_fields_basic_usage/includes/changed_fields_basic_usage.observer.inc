<?php

/**
 * @file
 * changed_fields_basic_usage.observer.inc file.
 */

/**
 * Class BasicUsageObserver.
 */
class BasicUsageObserver extends CFObserver {

  /**
   * {@inheritdoc}
   */
  public function update(CFSubjectInterface $nodeSubject) {
    $node = $nodeSubject->getNode();
    $changedFields = $nodeSubject->getChangedFields();

    // Do something with $node depends on $changedFields.
  }

}
