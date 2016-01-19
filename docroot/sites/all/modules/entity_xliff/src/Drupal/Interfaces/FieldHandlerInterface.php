<?php

/**
 * @file
 * Describes how entity xliff field handlers should be defined.
 */

namespace EntityXliff\Drupal\Interfaces;


interface FieldHandlerInterface {

  /**
   * Returns the value from the provided structure or value wrapper (depending
   * on the type of field).
   *
   * @param \EntityMetadataWrapper $wrapper
   *   The metadata wrapper for a field of this type (e.g. $w->your_field or
   *   potentially $w->your_field[$delta]).
   *
   * @return string|array
   *   This may return either a string (in the case of a simple, scalar value)
   *   or an array in the case of a field that encapsulates multiple, distinct
   *   translatable items. If an array is returned, its leaves should consist of
   *   at a minimum, the following items:
   *   - #label: The label for this field value (this may be used as contextual
   *     information by a translator).
   *   - #text: The actual string to be translated.
   */
  public function getValue(\EntityMetadataWrapper $wrapper);

  /**
   * Sets a given value on the provided structure or value wrapper (depending on
   * the type of field).
   *
   * @param \EntityMetadataWrapper $wrapper
   *   The metadata wrapper for a field of this type (e.g. $w->your_field or
   *   potentially $w->your_field[$delta]).
   *
   * @param mixed $value
   *   The value to be saved. This will be provided in a form very closely
   *   matching the form provided via FieldHandlerInterface::getValue(). If the
   *   value provided by that method is scalar, then $value will also be scalar.
   *   If the value provided by that method is an array, then $value will be an
   *   array, though instead of the #text and #label keys, the translated string
   *   will be provided directly.
   */
  public function setValue(\EntityMetadataWrapper $wrapper, $value);

}
