<?php

/**
 * @file
 * Simpliste theme implementation to display a node.
 */
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

<?php if ($user_picture || $display_submitted || !$page): ?>
  <header>
    <?php print $user_picture; ?>

    <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php if ($display_submitted): ?>
      <p class="submitted"><?php print $submitted; ?></p>
    <?php endif; ?>
  </header>
<?php endif; ?>

<div class="content"<?php print $content_attributes; ?>>
  <?php
    // We hide the comments, tags and links now so that we can render them later.
    hide($content['comments']);
    hide($content['links']);
    print render($content);
  ?>
</div> <!-- /.content -->

<?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
  <footer>
    <?php print render($content['links']); ?>
  </footer>
<?php endif; ?>

<?php print render($content['comments']); ?>

</article> <!-- /node -->