<?php
/**
 * @file
 * Preview the mailing as if it were an email UI.
 */
?>
<h2><?php print t('Preview'); ?></h2>
<div id="lyris-mailing-preview">
  <div class="item to"><span class="label">To:</span><span class="value"><?php print $to; ?></span></div>
  <div class="item reply-to"><span class="label">Reply-To:</span><span class="value"><?php print $reply_to; ?></span></div>
  <div class="item from"><span class="label">From:</span><span class="value"><?php print $from; ?></span></div>
  <div class="item subject"><span class="label">Subject:</span><span class="value"><?php print $subject; ?></span></div>
  <div class="edit-link"><?php print $edit_content_link; ?></div>
  <iframe src="<?php print $preview_path; ?>" name="lyris-content-frame" id="lyris-content-frame" frameborder="0">
    Mailing preview goes here.
  </iframe>
</div>
