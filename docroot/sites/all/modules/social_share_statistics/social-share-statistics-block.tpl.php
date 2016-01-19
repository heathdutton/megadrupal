<?php

/**
 * @file
 * Module implementation to display the share buttons.
 *
 * Available variables:
 *   - $items: A complete array of share buttons and
 *     text to display total share count.
 */
?>

<!-- Div id fb-root is necessary-->
<div id="fb-root"></div>

<div class="fb_count">
	<?php print $items["fb_imagepath"] ?>
	<label><?php print $items["fb_count"] ?></label>
</div>
<div class="twt_count">
	<?php print $items["twitter_share"] ?>
	<label><?php print $items["twitter_count"] ?></label>
</div>
<?php if ($items['total_shares_display']): ?>
  <div class="total_count"><?php print $items["count_text"] ?><label><?php print $items["total_shares"] ?></label></div>
<?php endif; ?>
