<?php
  /**
   * @file
   * Template to display the current lendee of an item.  This template is only
   * ever reached if there is a valid checkout for an item, otherwise we never
   * get here, so assume $user and $reservation are filled out.
   *
   * $queue - the request queue
   * $node - the node in question
   * $user - the current user, so we can see if they've got requests
   *
   */
?>
<p>
  Below is a preview of what the next email sent out on this announcement will be, to give you a feel of what it will look like. If you don't like some aspect of it, now is the time to change it.
</p>
<div id="node-announce-preview">

  <div id="node-announce-preview-from">From: <?php print $announce->email_from; ?></div>
  <div id="node-announce-preview-to">To: <?php print $announce->email_to; ?></div>
  <div id="node-announce-preview-subject">Subject: <?php print token_replace($announce->subject, array('node' => $node)); ?></div>
  <hr>
<div id="node-announce-preview-body"><?php print token_replace($announce->message, array('node' => $node)); ?></div>
</div>
