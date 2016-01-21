<?php

/**
 * Area handler to display the cart message block.
 */
class commerce_cartmessage_handler_messageblock extends views_handler_area {
  function render($empty = FALSE) {
    //if ($this->view->display_handler instanceOf views_plugin_display_page) {
      $message = commerce_cartmessage_setmessage();
      return theme('commerce_cartmessage', array('message' => $message));
    //}
  }
}
