<?php

/**
 * @file
 * changed_fields_extended_field_comparator.field_comparator.inc file.
 */

/**
 * Class ExtendedFieldComparator.
 */
class ExtendedFieldComparator extends CFDefaultFieldComparator {

  /**
   * {@inheritdoc}
   */
  public function getDefaultComparableProperties($fieldType) {
    $properties = array();

    // Return comparable field properties for extra or custom field type.
    if ($fieldType == 'some_field_type') {
      $properties = array(
        'some_field_property_1',
        'some_field_property_2',
      );
    }

    return $properties;
  }

}
