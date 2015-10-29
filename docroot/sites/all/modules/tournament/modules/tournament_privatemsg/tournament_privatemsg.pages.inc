<?php

/**
 * Menu callback; Send messages to tournament participants.
 */
function tournament_privatemsg_participant_message($node) {
  module_load_include('inc', 'privatemsg', 'privatemsg.pages');

  $recipient = array(
    'recipient' => $node->nid,
    'type' => 'tournament',
    'title' => $node->title,
  );

  $recipient = (object) $recipient;
 
  return drupal_get_form('privatemsg_new', $recipient);;
}
