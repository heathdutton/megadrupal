<?php

/**
 * @file
 * Contains theme for previous/next links.
 *
 * Available variables;
 * - $previd: Id of the previous node 
 * - $nextid: Id of the next node 
 * - $midid: Middle link 
 * - $midopts: l() options for middle link. Required for jump link
 * - $midtitle: Title of the middle link 
 * - $prevtitle: Title of the previous node
 * - $nexttitle: Title of the next node
 */
?>

<div class="prev-next-links">
  <?php if($previd): ?>
    <div class="prev-link">
      <?php print l($prevtitle, 'node/' . $previd); ?>
    </div>
  <?php endif; ?>

  <?php if($nextid): ?>
    <div class="next-link">
      <?php print l($nexttitle, 'node/' . $nextid); ?>
    </div>
  <?php endif; ?>

  <?php if($midid): ?>
    <div class="mid-link">
      <?php print l($midtitle, $midid, $midopts); ?>
    </div>
  <?php endif; ?>

</div>
