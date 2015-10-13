<?php
// $Id: forums.tpl.php,v 1.7 2009/12/03 20:21:50 dries Exp $

/**
 * @file
 * Default theme implementation to display a forum which may contain forum
 * containers as well as forum topics.
 *
 * Variables available:
 * - $forums: The forums to display (as processed by forum-list.tpl.php)
 * - $topics: The topics to display (as processed by forum-topic-list.tpl.php)
 * - $forums_defined: A flag to indicate that the forums are configured.
 *
 * @see template_preprocess_forums()
 * @see theme_forums()
 */
?>
<?php if ($forums_defined): ?>
<div id="forum">
<div class="forum-top">
  <?php print $forums; ?>
  <?php print $topics; ?>
</div>
<div class="forum-bottom"><span class="forum-bottom"></span></div>
</div>
<?php endif; ?>
