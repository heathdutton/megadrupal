<?php
// $Id: book-navigation.tpl.php,v 1.3 2009/02/18 14:28:21 webchick Exp $

/**
 * @file
 * Default theme implementation to navigate books. Presented under nodes that
 * are a part of book outlines.
 *
 * Available variables:
 * - $tree: The immediate children of the current node rendered as an
 *   unordered list.
 * - $current_depth: Depth of the current node within the book outline.
 *   Provided for context.
 * - $prev_url: URL to the previous node.
 * - $prev_title: Title of the previous node.
 * - $parent_url: URL to the parent node.
 * - $parent_title: Title of the parent node. Not printed by default. Provided
 *   as an option.
 * - $next_url: URL to the next node.
 * - $next_title: Title of the next node.
 * - $has_links: Flags TRUE whenever the previous, parent or next data has a
 *   value.
 * - $book_id: The book ID of the current outline being viewed. Same as the
 *   node ID containing the entire outline. Provided for context.
 * - $book_url: The book/node URL of the current outline being viewed.
 *   Provided as an option. Not used by default.
 * - $book_title: The book/node title of the current outline being viewed.
 *   Provided as an option. Not used by default.
 *
 * @see template_preprocess_book_navigation()
 */
?>
<?php if ($tree || $has_links): ?>
  <div id="book-navigation-<?php print $book_id; ?>" class="book-navigation">
  
    <?php if ($tree): ?>
      <div class="book-navigation-tree">
        <h3><?php print t('In this section:'); ?></h3>
        <?php print $tree; ?>
      </div>
    <?php endif; ?>
  
    <?php if ($has_links): ?>
    <h3><?php print t('More:'); ?></h3>
    <div class="page-links clearfix">
      <?php if ($prev_url) : ?>
        <a href="<?php print $prev_url; ?>" class="page-previous" title="<?php print t('Go to previous page'); ?>"><?php print t(' &larr; ') . $prev_title; ?></a>
      <?php endif; ?>

      <?php if ($next_url) : ?>
        <a href="<?php print $next_url; ?>" class="page-next" title="<?php print t('Go to next page'); ?>"><?php print $next_title . t(' &rarr;'); ?></a>
      <?php endif; ?>
    </div>
    <?php endif; ?>

  </div>
<?php endif; ?>
