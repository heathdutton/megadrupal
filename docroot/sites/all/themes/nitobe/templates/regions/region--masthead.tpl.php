<?php
// $Id: region--masthead.tpl.php,v 1.1 2010/11/14 03:24:00 shannonlucas Exp $
/**
 * @file
 * Nitobe's masthead region.
 *
 * @see _nitobe_build_masthead()
 * @see nitobe_preprocess_region()
 */
?>
<div id="masthead" class="grid-16 <?php echo $classes; ?>">
  <?php if ($content) echo $content; ?>
</div><!-- /masthead -->
<hr class="rule-top grid-16"/>
