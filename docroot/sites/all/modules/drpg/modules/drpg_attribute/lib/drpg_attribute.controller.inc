<?php

/**
 * @file
 * The controller for the attribute entity.
 */

class DrpgAttributeEntityController extends EntityAPIController {

  /**
   * Saves an attribute entity.
   *
   * @param DrpgAttribute $attribute
   *   The attribute to save.
   *
   * @return DrpgAttribute
   *   The saved attribute.
   */
  public function save($attribute) {
    $attribute->updated = time();

    if (isset($attribute->is_new) && $attribute->is_new) {
      $attribute->created = time();
    }

    parent::save($attribute);

    return $attribute;
  }

}
