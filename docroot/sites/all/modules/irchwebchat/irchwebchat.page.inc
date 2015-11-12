<?php

/**
 * @file
 * Irchwebchat page functions
 */


/**
 * Menu callback; displays an IRC-Hispano webchat page.
 */
function irchwebchat_page() {
  require_once('irchwebchat.functions.inc');
  
  $output = _irchwebchat_generate_chat_form(IRCHWEBCHAT_DISPLAY_TYPE_EMBED);

  return $output;
}
