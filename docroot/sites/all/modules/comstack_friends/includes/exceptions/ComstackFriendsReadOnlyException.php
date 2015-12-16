<?php

/**
 * @file
 * Contains \ComstackFriendsReadOnlyException.
 */

class ComstackFriendsReadOnlyException extends ComstackException {
  protected $message = "You can't make any changes relating to relationships right now.";
}
