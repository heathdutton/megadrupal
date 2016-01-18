<div id="article-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix" role="article"<?php print $attributes; ?>>
  
  <?php if ($teaser): ?>
    <?php if ($display_submitted): ?>
  	<div class="submission">
      <?php print $submitted; ?>
    </div>
    <?php endif; ?>
  <?php endif; ?>
  
  <?php if ($title && !$page): ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h2<?php print $title_attributes; ?>>
          <a href="<?php print $node_url; ?>" rel="bookmark"><?php print $title; ?></a>
        </h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
  <?php endif; ?>

  <?php print $unpublished; ?>

  <div<?php print $content_attributes; ?>>
    <?php
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php if (!$teaser): ?>
    <?php if ($display_submitted): ?>
  	<div class="submission">
    <?php print $user_picture; ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
    </div>
    <?php endif; ?>
  <?php endif; ?>
  
  <?php if ($teaser == true): ?>
  <?php print $newreadmore; ?>
  <?php endif; ?>

  <?php if ($links = render($content['links'])): ?>
    <div class="node-links clearfix"><?php print $links; ?></div>
  <?php endif; ?> 

</div>
  <?php print render($content['comments']); ?>