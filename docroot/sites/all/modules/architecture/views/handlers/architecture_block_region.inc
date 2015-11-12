<?php

/**
 * @file
 * Views handler for blocks.
 */

/**
 * @inheritdoc
 */
class architecture_block_region_views_handler extends views_handler_field {

  /**
   * Render the required settings for this nodetype.
   */
  function render($values) {
    $value = $this->get_value($values);
    return $value == -1 ? NULL : $value;
  }
}
