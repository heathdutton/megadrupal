<?php 
/**
 * @file
 * WhiteJazz D7.x note.tpl.php
 *
 **/
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($picture): print $user_picture; endif; ?>
  <?php print render($title_prefix); ?>
  <?php if ($page == 0) { ?>
    <?php if ($title) { ?>
      <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url?>"><?php print $title?></a></h2>
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
  <div class="clearfix clear"></div>
  <?php if (!empty($content['links']['node']['#links']) || !empty($content['links']['comment']['#links']) || !empty($content['links']['blog']['#links'])): ?>
    <div class="links">&raquo; <?php print render($content['links']); ?></div>
  <?php endif; ?>
  <?php if ($content['comments'] && ($page)) { ?>
    <?php print render($content['comments']); ?>
  <?php }; ?>
</div>
