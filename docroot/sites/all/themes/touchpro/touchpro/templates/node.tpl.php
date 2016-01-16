<?php

/**
 * @file
 * Default theme implementation to display a node.
 */
?>

<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if (!$page): ?>
  <header>
    <?php print render($title_prefix); ?>
    <h1<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h1>
    <?php print render($title_suffix); ?>
  </header>
  <?php endif; ?>

  <div class="content clearfix"<?php print $content_attributes; ?>>
  <?php
    print render($content['field_image']);
    hide($content['field_image']);
  ?>

  <?php if ($display_submitted || $user_picture): ?>
    <header class="author">
      <?php print $user_picture; ?>
      <?php print $submitted; ?>
    </header>
  <?php endif; ?>
    <?php
      // We hide the comments and links now so that we can render them later.
      $terms = render($content['field_tags']);
      $terms .= render($content['field_category']);
      $terms .= render($content['field_categories']);
      $links = render($content['links']);
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_tags']);
      hide($content['field_category']);
      hide($content['field_categories']);
      print render($content);
    ?>
  </div>

  <?php if ($links OR $terms): ?>
    <footer class="link-wrapper clearfix">
      <?php print $links; ?>
      <?php print $terms; ?>
    </footer>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

</article>
