<?php

/**
 * @file
 * Page pair template for PageFlip HTML viewer. Renders one pair of book pages.
 */

/**
 * We get a "page pair" each time in this template.
 * If a page has no image set, then it might have overridden HTML.
 * If it has no image and no overridden HTML, it should be a blank page.
 */
?>

    <div<?php if ($pair_index == 0): ?> id="pageflip-chapter-<?php print $chapter_index; ?>"<?php endif; ?> class="pageflip-page-pair pageflip-page-pair-<?php print $settings['resolution']; ?>"<?php print $pair_attributes; ?>>
      <?php /* $left['content'] is set if we have an ad */ ?>
      <?php if ($left['content']): ?>
        <div class="pageflip-left-page pageflip-ad-page pageflip-ad-page-<?php print $settings['resolution']; ?>"<?php print $page_attributes; ?>>
          <?php print $left['content']; ?>
        </div>
      <?php else: ?>
        <?php /* It's not an ad, so make a link */ ?>
        <a id="pageflip-page-<?php print $page_index; ?>" class="pageflip-left-page"<?php if ($page_index > 0): print ' href="#/' . ($page_index - 2) . '" '; endif; ?>>
          <?php if ($left['image']): ?>
            <?php /* It's a regular page */ ?>
            <img width="<?php print $page_width; ?>" height="<?php print $page_height; ?>" src="<?php print $left['image']; ?>" />
          <?php else: ?>
            <?php /* It's a blank page */ ?>
            <div class="pageflip-left-page pageflip-blank-page pageflip-blank-page-<?php print $settings['resolution']; ?>"<?php print $page_attributes; ?>></div>
          <?php endif; ?>
        </a>
      <?php endif; ?>

      <?php /* $right['content'] is set if we have an ad */ ?>
      <?php if ($right['content']): ?>
        <div class="pageflip-right-page pageflip-ad-page pageflip-ad-page-<?php print $settings['resolution']; ?>"<?php print $page_attributes; ?>>
          <?php print $right['content']; ?>
        </div>
      <?php else: ?>
        <?php /* It's not an ad, so make a link */ ?>
        <a id="pageflip-page-<?php print $page_index + 1; ?>" class="pageflip-right-page"<?php if (!$very_last_pair): print ' href="#/' . ($page_index + 2) . '" '; endif; ?>>
          <?php if ($right['image']): ?>
            <?php /* It's a regular page */ ?>
            <img width="<?php print $page_width; ?>" height="<?php print $page_height; ?>" src="<?php print $right['image']; ?>" />
          <?php else: ?>
            <div class="pageflip-right-page pageflip-blank-page pageflip-blank-page-<?php print $settings['resolution']; ?>"<?php print $page_attributes; ?>></div>
          <?php endif; ?>
        </a>
      <?php endif; ?>

    </div>
