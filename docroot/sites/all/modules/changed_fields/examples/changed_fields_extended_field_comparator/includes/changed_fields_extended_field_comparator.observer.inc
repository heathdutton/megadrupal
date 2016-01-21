<?php

/**
 * @file
 * changed_fields_extended_field_comparator.observer.inc file.
 */

/**
 * Class ExtendedFieldComparatorObserver.
 */
class ExtendedFieldComparatorObserver extends CFObserver {

  /**
   * {@inheritdoc}
   */
  public function update(CFSubjectInterface $nodeSubject) {
    $node = $nodeSubject->getNode();
    $changedFields = $nodeSubject->getChangedFields();

    // Do something with $node depends on $changedFields.
  }

}
