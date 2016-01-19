<?php

/**
 * @file
 * The controller for the recipe item entity.
 */

class DrpgRecipeItemEntityController extends EntityAPIController {

  /**
   * Saves a recipe item entity.
   *
   * @param DrpgRecipeItem $recipe_item
   *   The recipe item to save.
   *
   * @return DrpgRecipeItem
   *   The saved recipe item.
   */
  public function save($recipe_item) {
    $recipe_item->updated = time();

    if (isset($recipe_item->is_new) && $recipe_item->is_new) {
      $recipe_item->created = time();
    }

    parent::save($recipe_item);

    return $recipe_item;
  }

}
