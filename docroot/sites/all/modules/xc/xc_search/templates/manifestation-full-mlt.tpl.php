<?php
/**
 * @file
 * Template for the full record display's More Like This listing element.
 *
 * Variables:
 * - $node_id (int): the node identifier
 * - $link (string): the link to node
 * - $image_url: the ural of cover image
 * - $title_link: the linked litle
 * - $title_clean: the cleaned title, which contains no HTML tags
 * - $creator: the creator of the item
 * - $count: the count number
 * - $show: whether to show template of not
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<?php if ($show): ?>

  <div class="xc-mlt">
    <div class="xc-mlt-count"><?php print $count; ?></div>

    <?php if (isset($image_url)): ?>
      <span id="coverart-<?php print $node_id; ?>" class="xc-mlt-coverart coverart">
        <a href="<?php print $link; ?>" title="<?php print $title_clean; ?>"><img src="<?php print $image_url; ?>"
          alt="<?php print t('Cover image of !title', array('!title' => $title_clean)); ?>"
          title="<?php print t('Cover image of !title', array('!title' => $title_clean)); ?>"
          class="xc-mlt-coverart-img" /></a>
      </span>
    <?php endif; ?>

    <?php if (isset($title_link)): ?>
      <span class="xc-mlt-title"><?php print $title_link; ?></span>

      <?php if (isset($creator) && !empty($creator)): ?>
        <span class="xc-mlt-author" title="<?php print $all_creators; ?>">by <?php print $creator; ?></span>
      <?php endif; ?>
    <?php endif; ?>
  </div>

<?php endif ?>