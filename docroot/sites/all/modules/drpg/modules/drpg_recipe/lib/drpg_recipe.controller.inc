<?php

/**
 * @file
 * The controller for the recipe entity.
 */

class DrpgRecipeEntityController extends EntityAPIController {

  /**
   * Saves a recipe entity.
   *
   * @param DrpgRecipe $recipe
   *   The recipe to save.
   *
   * @return DrpgRecipe
   *   The saved recipe.
   */
  public function save($recipe) {
    $recipe->updated = time();

    if (isset($recipe->is_new) && $recipe->is_new) {
      $recipe->created = time();
    }

    parent::save($recipe);

    return $recipe;
  }

}
