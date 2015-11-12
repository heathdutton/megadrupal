<?php // $Id$  ?>
<div id="node-<?php print $node->nid ?>" class="blog">

<?php if (!$page): ?>
  <h2 class="teaser-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>">
    <?php print $title ?></a></h2>
<?php endif; ?>

<?php if ($submitted || $terms): ?>
  <div class="meta">
    <?php if ($submitted): ?>
      <div <?php if ($fancydates) :?>class="fancydate"<?php endif; ?>>
       <?php print $blog_date ?>
      </div>
    <?php endif; ?>
    <?php if ($terms): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
  </div>
<?php endif; ?>

  <div class="content clear-block">
    <?php print $picture ?>
    <?php print $content ?>
  </div>

<?php if ($links): ?>
  <div class="node-links"><?php print $links ?></div>
<?php endif; ?>

</div>
