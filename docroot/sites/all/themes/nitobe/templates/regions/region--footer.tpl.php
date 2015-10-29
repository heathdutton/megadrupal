<?php
// $Id: region--footer.tpl.php,v 1.2 2010/11/14 03:24:00 shannonlucas Exp $
/**
 * @file
 * The footer region for Nitobe.
 */
?>
<?php if ($content): ?>
<div id="footer-area" class="container-16">
  <hr class="rule-top grid-16"/>
  <div id="footer" class="grid-16 <?php echo $classes; ?>">
    <?php echo $content; ?>
  </div><!-- /footer -->
</div><!-- /footer-area -->
<hr class="break"/>
<?php  endif; ?>
