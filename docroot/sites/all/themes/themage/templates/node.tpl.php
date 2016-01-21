<?php
/**
 * @file
 * Themage's implementation to display a node.
 */
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <header>
    <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
  </header>

  <section class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </section>

  <footer role="contentinfo">
    <?php print $user_picture; ?>
    
    <?php if ($display_submitted): ?>
      <?php print $submitted; ?>
    <?php endif; ?>
  </footer>
  
  <?php print render($content['links']); ?>
  <?php print render($content['comments']); ?>
</article>
