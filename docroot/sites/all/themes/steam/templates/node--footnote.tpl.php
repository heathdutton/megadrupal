<?php

/**
 * @file
 * Default theme implementation to display a node.
 */
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <header class="clearfix">
    <div class="header-top">
      <?php print render($title_prefix); ?>
      <a href="<?php print $node_url; ?>"<?php print $view_mode == 'full' ? '': ' class="lightbox"'; ?>>
        <h1<?php print $title_attributes; ?>><?php print $title; ?></h1>
      <?php if ($view_mode != 'full'): ?></a><? endif; ?>
        <h2<?php print $title_abbr_attributes; ?>><?php print $title_abbr; ?></h2>
      <?php if ($view_mode == 'full'): ?></a><? endif; ?>
      <?php print render($resource_icons); ?>
      <?php print render($content['os_sharethis_widget']); ?>
      <?php print render($title_suffix); ?>
    </div>
    <?php if ($view_mode == 'full'): ?>
      <div class="header-metadata">
        <?php print render($permalink); ?>
        <?php if (!empty($categories)): ?>
          <div class="categories-wrapper"><span class="categories-label"><?php print $categories_label; ?></span>
          <div class="categories"><?php print $categories; ?></div>
        <?php endif; ?>
      </div>
    <?php endif; ?>
  </header>

  <?php if ($display_submitted || $user_picture): ?>
    <footer class="author">
      <?php print $user_picture; ?>
      <?php print $submitted; ?>
    </footer>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <footer class="link-wrapper">
    <?php print $last_updated; ?>
  </footer>

  <?php print render($content['comments']); ?>

</article>
