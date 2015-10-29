<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if ($title_prefix || $title_suffix || $display_submitted || !$page): ?>
  <header class="node-meta clearfix">
    <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
      <div class="node-title">
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
      </div>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php if ($display_submitted): ?>
      <div class="submitted">
        <?php print $user_picture; ?>
        <?php if (theme_get_setting('font_awesome_include', 'boot_press')): ?>
          <i class="fa fa-thumb-tack"></i> <?php print $submitted; ?>
        <?php else: ?>
          <span class="glyphicon glyphicon-pushpin"></span> <?php print $submitted; ?>
        <?php endif; ?>
      </div>
    <?php endif; ?>
  </header>
  <?php endif; ?>
  
  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_tags']);
      print render($content);
    ?>
  </div>

  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
    <footer>
    <?php print render($content['field_tags']); ?>
    <?php print render($content['links']); ?>
    </footer>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

</article>
