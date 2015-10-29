<?php
/**
 * @file
 * This is custom node.tpl.php for Centurion Framework
 */
?>

<article class="blogPost"<?php print $attributes; ?>> <?php print $user_picture; ?>
  <?php if (!$page && $title): ?>
  <header> <?php print render($title_prefix); ?>
    <h2 <?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    <?php print render($title_suffix); ?> </header>
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
    <nav class="links"><?php print render($content['links']); ?></nav>
    <?php endif; ?>
    <?php print render($content['comments']); ?> </div>
</article>
