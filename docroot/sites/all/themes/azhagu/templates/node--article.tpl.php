<?php

/**
 * @file
 * Default theme implementation to display a node.
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="header-node">
    
    <div class="date">
      <span class="month"><?php print $date_month ?></span>
      <span class="day"><?php print $date_day ?></span>
      <span class="year"><?php print $date_year ?></span>
    </div>
  
     <div class="title-meta">
      <?php print render($title_prefix); ?>
        <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
      <?php print render($title_suffix); ?>

      <div class="meta">
        <ul>
           <?php if ($display_submitted): ?>
          <li class="author-name"><?php print $name; ?></li>
          <?php endif; ?>
          <?php if ($tags = render($content['field_tags'])): ?>
            <li class="tags"><span><?php print $tags; ?></span></li>
          <?php endif; ?>
          <li class="comments"><span><a href="<?php print $node_url;?>#comments"><?php print $comment_count; ?> Comments</a></span></li>
        </ul>
      </div>
    </div>
  </div>


  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
    <div class="footer-readmore">
      <?php if ($teaser): ?>
        <?php print $newreadmore; ?>
      <?php endif; ?>
    </div>
  </div>

  <?php print render($content['links']); ?>
  <?php print render($content['comments']); ?>

</div>
