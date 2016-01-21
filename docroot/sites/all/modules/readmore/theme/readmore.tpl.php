<?php

/**
 * @file
 * Default theme implementation to display a divided text.
 *
 * Available variables:
 * - $summary: Truncated string.
 * - $text: All text.
 */
?>
<div class="readmore-summary">
  <?php print $summary; ?>
</div>
<div class="readmore-text" style="display:none;">
  <?php print $text; ?>
</div>
