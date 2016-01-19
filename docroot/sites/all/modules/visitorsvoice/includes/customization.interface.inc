<?php

/**
 * @file
 * Contains VisitorsvoiceCustomizationInterface.
 */


/**
 * Represents a specific customization for a certain item and set of keywords.
 *
 * Such specific customizations are attached to each item when indexing so that
 * they can later be retrieved when searching, to change or re-order search
 * results.
 */
interface VisitorsvoiceCustomizationInterface {

  /**
   * Describes customizations that change the order of results.
   *
   * @see getType()
   */
  const TYPE_ORDER = 'order';

  /**
   * Describes customizations that add items to search results.
   *
   * @see getType()
   */
  const TYPE_ADD = 'add';

  /**
   * Describes customizations that remove items from search results.
   *
   * @see getType()
   */
  const TYPE_REMOVE = 'remove';

  /**
   * Returns the type of customization.
   *
   * @return string
   *   The customization type. One of the TYPE_* constants of this interface.
   */
  public function getType();

  /**
   * Returns the customization ID to which this specific customization belongs.
   *
   * Customizations are linked to the keywords used in a search. Each search can
   * have at most one set of customizations attached, represented by the
   * customization ID.
   *
   * @return int
   *   The ID of the customization to which this specific customization belongs.
   */
  public function getCustomizationId();

  /**
   * Returns the place in the result list the item should have.
   *
   * This should only be used when the customization type is "order". The return
   * value of this method is otherwise undefined.
   *
   * @return int
   *   The place in the result list the item should have. I.e., 1 for the first
   *   place, 2 for the second, etc.
   */
  public function getOrder();

}
