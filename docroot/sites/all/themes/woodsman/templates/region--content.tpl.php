<?php

/**
 * @file
 * provides all the basic functionality. However, in case you wish to customize
 * the output that Drupal generates through Alpha & Omega.
 * this file is a good place to do so.
 * Alpha comes with a neat solution for keeping this file as clean as possible
 * while the code for your subtheme grows.
 * Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */
?>

<div<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <a id="main-content"></a>
    <?php if (!empty($breadcrumb)): ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
    <?php endif; ?>    
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
    <?php if ($title_hidden): ?><div class="element-invisible"><?php endif; ?>
    <?php if (!empty($article_count)): ?>
      <span class="articles_count">
        <?php print $article_count; ?>
      </span>
    <?php endif; ?>
    <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php if ($title_hidden): ?></div><?php endif; ?>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ($tabs && !empty($tabs['#primary'])): ?><div class="tabs clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print $content; ?>
    <?php if ($feed_icons): ?><div class="feed-icon clearfix"><?php print $feed_icons; ?></div><?php endif; ?>
  </div>
</div>
