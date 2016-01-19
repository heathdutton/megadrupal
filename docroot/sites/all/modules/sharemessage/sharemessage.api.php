<?php

/**
 * @file
 * Hooks provided by share message module.
 */


/**
 * Allow other modules to alter sharemessage token context.
 *
 * @param $sharemessage
 *   Currently loaded sharemessage object.
 * @param $context
 *   Token Context.
 */
function hook_sharemessage_token_context_alter($sharemessage, &$context) {
  global $language;

  // Alter sharemessage title.
  $sharemessage->sharemessage_title[$language->language][0]['value'] = 'Altered Title';

  // Add taxonomy_vocabulary object type in a $context array.
  $context['taxonomy_vocabulary'] = menu_get_object('taxonomy_vocabulary');
}
