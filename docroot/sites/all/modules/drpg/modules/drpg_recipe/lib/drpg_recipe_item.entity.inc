<?php

/**
 * @file
 * Defines the recipe item entity.
 */

class DrpgRecipeItem extends Entity {

  // @codingStandardsIgnoreStart
  public $recipe_item_id = NULL;
  public $recipe_id = NULL;
  public $item_id = NULL;
  public $quantity = 0;
  public $created = '';
  public $updated = '';
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_recipe_item');

    // Check for new recipe item.
    if (!$this->recipe_item_id) {
      $this->created = time();
    }
  }

}
