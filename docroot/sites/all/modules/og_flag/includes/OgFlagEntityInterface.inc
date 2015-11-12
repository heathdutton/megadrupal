<?php
/**
 * @file
 * Interface implemented by og_flag override classes.
 */

/**
 * Defines the common og_flag specific methods.
 */
interface OgFlagEntityInterface {
  /**
   * Updates the flag for the context of the current request.
   */
  public function setContext($entity_id = NULL);

  /**
   * Initializes og_flag specific variables.
   */
  public function OgFlagInit();
}
