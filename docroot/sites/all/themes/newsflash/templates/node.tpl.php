<?php

/**
 * @file
 * NewsFlash node.tpl.php
 * for Default theme implementation see modules/node/node.tpl.php
 *
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="node-header">
    <?php if ($picture) { ?>
      <?php print $user_picture ?>
    <?php }; ?>
    <?php print render($title_prefix); ?>
    <?php if (!$page) { ?>
      <?php if ($title) { ?>
        <h2 <?php print $title_attributes; ?>>
          <a href="<?php print $node_url ?>" title="<?php print $title ?>">
            <?php print $title?>
          </a>
        </h2>
      <?php }; ?>
    <?php }; ?>
    <?php print render($title_suffix); ?>
  </div>
  <div class="content clearfix"<?php print $content_attributes ?>>
    <?php print $content_attributes?>
    <?php
      hide($content['comments']);
      hide($content['links']);
    print render($content); ?>
  </div>
  <?php if ($display_submitted && !$page) { ?>
    <span class="meta submitted">
      <?php /** print $permalink; */ ?>
      <?php  print t('By !username at !datetime', array('!username' => $name, '!datetime' => $date)); ?>
    </span>
  <?php } else { ?>
    <span class="meta submitted">
      <?php /** print $permalink; */ ?>
      <?php print $submitted; ?>
    </span>
  <?php }; ?>
  <?php
  if ($teaser) {
    unset($content['links']['comment']['#links']['comment-add']);
    unset($content['links']['comment']['#links']['comment_forbidden']);
  }
  ?>
  <div class="links nf-node-links">
    <?php if (!empty($content['links'])): ?>
      <?php print render($content['links']); ?>
    <?php endif; ?>
  </div>
  <?php if ($content['comments'] && ($page)) { ?>
    <?php print render($content['comments']); ?>
  <?php }; ?>
</div>
