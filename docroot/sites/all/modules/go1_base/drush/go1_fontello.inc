<?php
/**
 * @file ./drush/go1_fontello.inc
 */

use \Drupal\go1_base\Drush\Command\FontEllo as FontElloCommand;

/**
 * Callback for go1_fontello command.
 */
function drush_fontello() {
  go1_id(new FontElloCommand())->execute();
}
