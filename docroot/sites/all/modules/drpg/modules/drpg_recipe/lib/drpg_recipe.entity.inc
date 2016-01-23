<?php

/**
 * @file
 * Defines the recipe entity.
 */

class DrpgRecipe extends Entity {

  // @codingStandardsIgnoreStart
  public $recipe_id = NULL;
  public $label = '';
  public $created = '';
  public $updated = '';

  // Not stored. Used to provide access to referenced attributes.
  public $recipe_items;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_recipe');

    // Check for new item.
    if (!$this->recipe_id) {
      $this->created = time();
    }
  }

  /**
   * Specifies the default entity label.
   */
  protected function defaultLabel() {
    return $this->label;
  }

  /**
   * Specifies the entity URI.
   */
  protected function defaultUri() {
    return array('path' => 'admin/drpg/recipes/manage/' . $this->internalIdentifier());
  }

}
