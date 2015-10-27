<?php

/**
 * @file
 * Base filter class and core implementations.
 */

/**
 * Class FacetapiFilterFoldingItems
 */
class FacetapiFilterFoldingItems extends FacetapiFilter {

  /**
   * Executes the filter by returning an altered render array.
   *
   * @param array $build
   *   The facet's base render array.
   *
   * @return array
   *   The altered render array that will be passed to the widget plugin.
   */
  public function execute(array $build) {
    return $this->cleanNestedLevels($build);
  }

  /**
   * Removes terms not actives nor at direct level of an active one.
   *
   * @param array $build
   *   The facet's base render array.
   * @param bool $keep
   *   Indicate to keep current child.
   *
   * @return array
   *   The altered render array.
   */
  protected function cleanNestedLevels(array $build, $keep = TRUE) {
    foreach ($build as $tid => $term) {
      if ($keep || $term['#active']) {
        $build[$tid]['#item_children'] = $this->cleanNestedLevels($term['#item_children'], $term['#active']);
      }
      else {
        unset($build[$tid]);
      }
    }

    return $build;
  }
}
