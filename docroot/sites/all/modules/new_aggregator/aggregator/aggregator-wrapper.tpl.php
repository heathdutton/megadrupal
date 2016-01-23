<?php
// $Id: aggregator-wrapper.tpl.php,v 1.3 2008/07/20 18:06:12 aronnovak Exp $

/**
 * @file
 * Default theme implementation to wrap aggregator content.
 *
 * Available variables:
 * - $content: All aggregator content.
 * - $page: Pager links rendered through theme_pager().
 *
 * @see template_preprocess()
 * @see template_preprocess_comment_wrapper()
 */
?>
<div id="aggregator">
  <?php print $content; ?>
  <?php print $pager; ?>
</div>
