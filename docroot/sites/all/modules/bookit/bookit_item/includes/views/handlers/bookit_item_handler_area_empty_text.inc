<?php

/**
 * Area handler to display the empty text message for bookable items.
 */
class bookit_item_handler_area_empty_text extends views_handler_area {

  public function render($empty = FALSE) {
    $exposed_input = $this->view->get_exposed_input();
    if (!empty($exposed_input)) {
      return t('No bookable items match your search criteria.');
    }

    return t('No bookable items have been created yet. <a href="!url">Add an item</a>.', array('!url' => url('admin/bookit/items/add')));
  }
}
