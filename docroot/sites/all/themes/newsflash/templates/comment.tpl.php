<?php

/**
 * @file
 * NewsFlash comment.tpl.php
 * for Default theme implementation see modules/comment/comment.tpl.php
 *
 */
?>
<div class="comments-nf">
  <div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <?php if ($new): ?>
      <span class="new">
        <?php print $new ?>
      </span>
    <?php endif; ?>
    <?php print render($title_prefix); ?>
      <h3 <?php print $title_attributes; ?>><?php print $title ?></h3>
    <?php print render($title_suffix); ?>
    <div class="nf-comment-submitted clearfix">
      <?php if ($picture) { ?>
        <?php print $picture ?>
      <?php }; ?>
      <span class="meta comment-submitted">
        <?php print t('Submitted by !username <br /> on !datetime', array('!username' => $author, '!datetime' => $created)); ?>
      </span>
      <?php if (theme_get_setting('newsflash_permalink')): ?>
      <span class="nf-comment-permalink">
        <?php print $permalink; ?>
      </span>
      <?php endif; ?>
    </div>
    <div class="content"<?php print $content_attributes; ?>>
      <?php
      /** We hide the comments and links now so that we can render them later. */
      hide($content['links']);
      print render($content);
      ?>
      <?php if ($signature): ?>
        <div class="user-signature clearfix">
          <?php print $signature ?>
        </div>
      <?php endif; ?>
      </div>
  </div>
  <div class="comments-links-nf">
    <?php print render($content['links']) ?>
  </div>
</div>
