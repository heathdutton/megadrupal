<?php

/**
 * @file
 * Views handler to show Follow link field.
 */

/**
 * Field handler for Follow link.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_twitter_web_intents_follow_link extends views_handler_field_twitter_web_intents_link {

  // Overrides default link text.
  protected $defaultText = 'Follow';

  // Overrides required column from Twitter table.
  protected $requiredTwitterField = 'screen_name';

  /**
   * Render the field.
   *
   * @param stdClass $values
   *   The values retrieved from the database.
   */
  public function render($values) {
    return parent::render_link('https://twitter.com/intent/user?screen_name=' . $values->twitter_screen_name);
  }

}
