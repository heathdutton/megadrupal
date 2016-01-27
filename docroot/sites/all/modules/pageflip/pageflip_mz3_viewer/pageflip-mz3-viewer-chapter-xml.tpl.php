<?php

/**
 * @file
 * PageFlip MegaZine3 Viewer chapter template. Renders one chapter and
 * its content, in MegaZine's XML format.
 */
?>
  <?php if ($override): ?>

    <?php print $override ?>

  <?php else: ?>

    <chapter anchor="chapter<?php print $chapter_index ?>">

      <?php print $content ?>

    </chapter>

  <?php endif; ?>

