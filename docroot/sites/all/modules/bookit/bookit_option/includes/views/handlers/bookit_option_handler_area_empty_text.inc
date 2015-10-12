<?php

/**
 * Area handler to display the empty text message for booking options.
 */
class bookit_option_handler_area_empty_text extends views_handler_area {

  public function render($empty = FALSE) {
    $exposed_input = $this->view->get_exposed_input();
    if (!empty($exposed_input)) {
      return t('No booking options match your search criteria.');
    }

    return t('No booking options have been created yet. <a href="!url">Add a booking option</a>.', array('!url' => url('admin/bookit/config/options/add')));
  }
}
