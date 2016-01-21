<?php
// $Id: forum-icon.tpl.php,v 1.8 2010/11/22 08:07:57 webchick Exp $

/**
 * @file
 * Default theme implementation to display an appropriate icon for a forum post.
 *
 * Available variables:
 * - $new_posts: Indicates whether or not the topic contains new posts.
 * - $icon_class: The icon to display. May be one of 'hot', 'hot-new', 'new',
 *   'default', 'closed', or 'sticky'.
 * - $first_new: Indicates whether this is the first topic with new posts.
 *
 * @see template_preprocess_forum_icon()
 * @see theme_forum_icon()
 */
?>
<?php if ($new_posts || $first_new): ?>
  <a name="new">
<?php endif; ?>
<?php global $theme_key; ?>
<?php print theme('image', array('path' => drupal_get_path('theme', $theme_key) . '/css/images/forum/' . 'forum-' . $icon_class . '.png')) ?>

<?php if ($new_posts): ?>
  </a>
<?php endif; ?>