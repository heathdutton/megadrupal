<?php
?>
<?php if ($page == 0 && $id > 1 && theme_get_setting('frontpage_teasers_separators')): ?><div class="clear-block node-list-separator node-list-separator-<?php print $zebra ?>"></div><?php endif; ?>
<div id="node-<?php print $node->type; ?>-<?php print $node->nid; ?>" class="<?php print $classes; ?><?php if ($is_front): print ' node-front'; endif; ?><?php if ($id == '1'): print ' node-first'; endif; ?> node-<?php print $zebra ?>"<?php print $attributes; ?>>
  <?php if ($user_picture): ?>
    <?php if ($page || theme_get_setting('teasers_userpics')): ?>
      <?php print $user_picture; ?>
      <?php endif; ?>
    <?php endif; ?>
  <?php print render($title_prefix); ?>
  <?php if (!$page || !$user_picture || !theme_get_setting('teasers_userpics')): ?>
  <?php if (!$page): ?><h2 class="title<?php if (!$user_picture || !theme_get_setting('teasers_userpics')): print ' title-no-pic'; endif; ?>"<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2><?php endif; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($display_submitted): ?>
    <span class="submitted"><?php print $submitted ?></span>
  <?php endif; ?>
  <div class="content clearfix"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <div class="links"><?php print render($content['links']); ?></div>
    <?php endif; ?>

    <?php print render($content['comments']); ?>
  </div>

</div>
