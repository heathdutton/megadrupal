<?php

/**
 * @file
 * Displays Kaltura player.
 *
 * Available variables:
 * - $html_id: Object HTML ID.
 * - $height: Player height.
 * - $width: Player width.
 *
 * @see http://player.kaltura.com/docs/kwidget
 * @see http://player.kaltura.com/docs/responsive
 *
 * @ingroup themeable
 */
?>
<div style="width: 100%;<?php if ($width): ?> max-width: <?php print $width; ?>px;<?php endif; ?> display: inline-block; position: relative;">
  <?php if ($width && $height): ?>
    <div style="margin-top: <?php print 100 * $height / $width; ?>%;"></div>
  <?php endif; ?>
  <div id="<?php print $html_id; ?>" style="position: absolute; top: 0; left: 0; right: 0; bottom: 0;"></div>
</div>
