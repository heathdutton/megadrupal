<?php
/**
 * @file
 * Overrides the node.tpl.php template for the Product Display node.
 *
 * @param $node->nid
 *  The unique identifier for the node.
 * @param $unpublished
 *  Print the words 'unpublished' only visible to those with permsissions to
 *  access unpublished nodes.
 * @param $title
 *  Title of the node.
 * @param $node_url
 *  The path to the node.
 * @param $submitted
 *  The creation date of the node.
 * @param $content
 *  Information held within the node.
 */

?>
<article id="article-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="article-inner <?php print $image_class;?>">
    <?php if ($teaser):?>
      <div class="teaser-inner">
    <?php endif;?>
    <?php print $unpublished; ?>

    <?php if ($page): print '<div class="product-info clearfix">'; endif;?>
      <?php if ($page || $teaser):?>
        <?php if (!empty($content['product:field_image'][0])):?>
          <?php print render($content['product:field_image']);?>
        <?php endif;?>
      <?php endif;?>
      <?php print render($title_prefix); ?>
      <?php if ($title && !$page): ?>
        <header>
        <?php if ($title): ?>
          <h1<?php print $title_attributes; ?>>
            <a href="<?php print $node_url; ?>" rel="bookmark">
              <?php print $title; ?>
            </a>
          </h1>
        <?php endif; ?>
      </header>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php if ($display_submitted): ?>
        <footer class="submitted<?php $user_picture ? print ' with-user-picture' : ''; ?>">
        <?php print $user_picture; ?>
        <p class="author-datetime"><?php print $submitted; ?></p>
      </footer>
      <?php endif; ?>
      <?php if ($page || $teaser):?>
        <?php if (!empty($content['product:commerce_price'])):?>
          <?php print render($content['product:commerce_price']);?>
        <?php endif;?>
        <?php if (!empty($content['field_product'])):?>
          <?php print render($content['field_product']);?>
        <?php endif;?>
      <?php endif;?>
    <?php if ($page): print '</div>'; endif;?>
    <div<?php print $content_attributes; ?>>
      <?php
        hide($content['comments']);
        hide($content['links']);
        print render($content);
      ?>
    </div>
    <?php if ($teaser):?></div><?php endif;?>
  </div>

  <?php if (($links = render($content['links'])) && $page): ?>
    <nav class="clearfix"><?php print $links; ?></nav>
  <?php endif; ?>

  <?php print render($content['comments']); ?>
</article>
