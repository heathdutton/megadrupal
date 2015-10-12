<?php //print_r(get_defined_vars());die; ?>
<div id="node-<?php print $node->nid; ?>" class="hentry <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $user_picture; ?>
  <div class="entry-meta">
    <?php if($authered_on) : ?>
      <?php print $authered_on; ?>
    <?php endif; ?>
    <?php print render($title_prefix); ?>
      <?php //if (!$page): ?>
        <h2<?php print $title_attributes; ?> class='entry-title'><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
      <?php //endif; ?>
    <?php print render($title_suffix); ?>
    <?php if($author) : ?>
      <?php print $author; ?>
    <?php endif; ?>
  </div>

  <div class="entry-content content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>
</div>
<div class="separator"></div>
