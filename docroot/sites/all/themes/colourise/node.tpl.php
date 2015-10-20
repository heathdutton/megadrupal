<?php

/**
 * @file
 * Colourise's theme implementation to display a node.
 */
?>

<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clearfix">

   <?php print render($title_prefix); ?>
 <?php if (!$page): ?> 
    <h2 class="title">
       <a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a>
    </h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <div class="post-info">

    <?php if ($user_picture): ?>
       <?php print $user_picture ?>
    <?php endif; ?>


    <?php if ($display_submitted): ?>
      <div class="meta">
        <span class="submitted"><?php print $submitted ?></span>
      </div>
    <?php endif; ?>
    
  </div>

  <div class="content">
   <?php
     // We hide the comments and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        print render($content); 
   ?>
  </div>

  <?php if ($content['links']): ?>
    <div class="node-links">
      <?php print render($content['links']); ?>
    </div>
  <?php endif; ?>

  <?php if ($content['comments']): ?>
   <?php print render($content['comments']); ?>
  <?php endif; ?>

</div>
