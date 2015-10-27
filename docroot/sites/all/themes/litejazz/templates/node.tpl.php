<?php

/**
 * @file
 * LiteJazz node.tpl.php
 * 
 * for Default theme implementation to display a block see modules/node/node.tpl.php.
 */
?>
<!-- div  id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>"<?php print $attributes; ?> -->
  <div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>  
  <?php if ($picture) { print $user_picture; }?>
  <?php print render($title_prefix); ?>
  <?php if (!$page) { ?>
    <?php if ($title) { ?>
      <h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2>
    <?php }; ?>
  <?php }; ?>
  <?php print render($title_suffix); ?>
  <?php if ($submitted) { ?>
    <span class="submitted"><?php print $submitted?></span> 
  <?php }; ?>
  <div class="content">
    <?php print $content_attributes?>
    <?php
    hide($content['comments']);
    hide($content['links']);
    print render($content);?>
  </div>
    <?php
      if ($teaser) {
      unset($content['links']['comment']['#links']['comment-add']);
      unset($content['links']['comment']['#links']['comment_forbidden']);
    }
    ?>
  <div class="clearfix clear"></div>
  <?php if (!empty($content['links']['node']['#links']) || !empty($content['links']['comment']['#links']) || !empty($content['links']['blog']['#links'])): ?>
    <div class="links">&raquo; <?php print render($content['links']); ?></div>
  <?php endif; ?>
  <?php if ($content['comments'] && ($page)) { ?>
    <?php print render($content['comments']); ?>
  <?php }; ?>
</div>

