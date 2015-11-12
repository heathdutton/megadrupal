<?php

/**
 * @file
 * Beale Street node.tpl.php
 *
 * for Default theme implementation see module/node/node.tpl.php
 *
 */
?>

<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ($picture) { print $user_picture; }?>
  <?php print render($title_prefix); ?>
  <?php if (!$page) { ?>
    <h2 class="title">
    <a href="<?php print $node_url?>"><?php print $title?></a></h2>
  <?php }; ?>
  <?php print render($title_suffix); ?>
  <span class="submitted-node">
    <?php print $submitted?>
  </span>
  <div class="content clearfix"<?php print $content_attributes?>>
    <?php print $content_attributes?>
    <?php       
    hide($content['comments']);
    hide($content['links']);
    print render($content); ?>
    <?php
      if ($teaser) {
      unset($content['links']['comment']['#links']['comment-add']);
      unset($content['links']['comment']['#links']['comment_forbidden']);
    }
    ?>
    <?php if (!empty($content['links']['node']['#links']) || !empty($content['links']['comment']['#links']) || !empty($content['links']['blog']['#links'])): ?>
      <div class="links">&raquo; <?php print render($content['links']) ?></div>
    <?php endif; ?>
  </div>
  <?php if ($content['comments'] && ($page)) { ?>
    <?php print render($content['comments']); ?>
  <?php }; ?>
</div>
