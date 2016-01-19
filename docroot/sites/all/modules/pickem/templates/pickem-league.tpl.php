<?php

/**
 * @file
 * Display a league dashboard.
 *
 * Available variables:
 * - $league_name: the (sanitized) name of the league.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
  *
 * Other variables:
 * - $league: Full league object. Contains data that may not be safe.
 */
?>
<div id="league-<?php print $league->lid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content clearfix"<?php print $content_attributes; ?>>
    <?php
      print render($content['league_nav']);
      print render($content['messages']);
    ?>
  </div>
</div>
