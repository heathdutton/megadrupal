<?php
/**
 * @file
 * A template file for the videopublishing video default format.
 *
 * @param array $item;
 */
?>

<div class="videopublishing-video videopublishing-video-default-formatter">
  <h3 class="videopublishing-video-title"><?php print $item['title']; ?></h3>
  <div class="videopublishing-video-body">
    <?php if (isset($item['embed_code']) && $item['embed_code']) : ?>
      <?php print $item['embed_code']; ?>
    <?php else : ?>
      <?php print t('This video will be available shortly.'); ?>
    <?php endif; ?>

    <?php if (isset($item['body']) && $item['body']) : ?>
      <p><?php print $item['body']; ?></p>
    <?php endif; ?>
  </div>
</div>