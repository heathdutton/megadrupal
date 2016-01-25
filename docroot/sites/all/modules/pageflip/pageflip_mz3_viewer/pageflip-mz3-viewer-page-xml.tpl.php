<?php

/**
 * @file
 * PageFlip MegaZine3 Viewer book-page template. Renders a single page
 * in MegaZine's XML format.
 */
?>
    <?php if ($content): ?>

      <!-- overridden page content: -->
      <?php print $content ?>

    <?php else: ?>

      <!-- normal page: -->
      <page<?php print $page_attributes ?>>
        <img width="<?php print $page_width ?>" height="<?php print $page_height ?>" aa="true">
          <src scale="0.5"><?php print $image_low ?></src>
          <src scale="1.0"><?php print $image_low ?></src>
          <src scale="1.5"><?php print $image_high ?></src>
        </img>
      </page>

    <?php endif ?>
