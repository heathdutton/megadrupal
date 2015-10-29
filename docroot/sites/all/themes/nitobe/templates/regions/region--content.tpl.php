<?php
// $Id: region--content.tpl.php,v 1.3 2010/11/26 06:17:01 shannonlucas Exp $
/**
 * @file
 * The content region for Nitobe.
 */
?>
<?php if ($content): ?>
  <div class="<?php print $classes; ?>">
    <?php echo $content; ?>
  </div>
<?php endif; ?>
