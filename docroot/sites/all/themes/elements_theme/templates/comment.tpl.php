<?php // $Id$ ?>
<li>
  <cite><?php print $author; ?></cite> on <?php print format_date($comment->timestamp); ?>
  <div class="commenttext">
    <?php
        // We hide the comments and links now so that we can render them later.
        hide($content['links']);
        print render($content);
      ?>
  </div>
  <?php if ($picture) : ?>
    <br class="clear" />
  <?php endif; ?>
  <div class="links"><?php print render($content['links']); ?></div>
</li>