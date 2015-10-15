<?php

/**
 * @file
 * Replace <div> with <table> since DoCoMo doesn't allow width attribute in div.
 */
?>

<div class="text"><?php print $title; ?></div>
<div class="bar" style="background-color:#dddddd;width:100%;">
  <table style="width: <?php print ($percentage > 0) ? $percentage .'%;background-color:#000000;' : '100%;background-color:#dddddd;'; ?>border-style:none;padding:0px;" class="foreground"><tr><td style="padding:0px;"/></tr></table>
</div>
<div class="percent" style="text-align:right;">
  <?php print $percentage; ?>% (<?php print format_plural($votes, '1 vote', '@count votes'); ?>)
</div>
