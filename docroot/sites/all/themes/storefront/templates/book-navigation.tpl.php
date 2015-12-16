<?php
/**
 * @file
 * @param $book_id
 *  Unique identifier for the book.
 * @param $tree
 *  Structure of the book in links.
 * @param $prev_url
 *  Path to the previous book page.
 * @param $prev_title
 *  Title of the previous book page.
 * @param $parent_url
 *  Path to the book page's parent.
 * @param $parent_title
 *  Title of the book page's parent.
 * @param $next_url
 *  Path to the next book page.
 * @param $next_title
 *  Title of the next book page.
 */

?>
<?php if ($tree || $has_links): ?>
  <footer id="book-navigation-<?php print $book_id; ?>" class="book-navigation">
    <?php print $tree; ?>
    <?php if ($has_links): ?>
    <div class="page-links clearfix">
      <?php if ($prev_url) : ?>
        <a href="<?php print $prev_url; ?>" class="page-previous" title="<?php print t('Go to previous page'); ?>"><?php print '‹ ' . $prev_title; ?></a>
      <?php endif; ?>
      <?php if ($parent_url) : ?>
        <a href="<?php print $parent_url; ?>" class="page-up" title="<?php print t('Go to parent page'); ?>"><?php print t('up'); ?></a>
      <?php endif; ?>
      <?php if ($next_url) : ?>
        <a href="<?php print $next_url; ?>" class="page-next" title="<?php print t('Go to next page'); ?>"><?php print $next_title . ' ›'; ?></a>
      <?php endif; ?>
    </div>
    <?php endif; ?>
  </footer>
<?php endif; ?>
