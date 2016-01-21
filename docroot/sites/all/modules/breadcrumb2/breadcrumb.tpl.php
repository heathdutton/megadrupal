<?php

/**
 * @file
 * Default theme implementation to display a breadcrumb.
 *
 * Available variables:
 * - $contextual_links: contextual links for breadcrumb.
 * - $breadcrumb: An array of breadcrumb link.
 * @see template_preprocess()
 * @see breadcrumb2_preprocess_breadcrumb()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>

<?php if (!empty($breadcrumb)): ?>
<div class="<?php print $classes; ?>">
 <?php if (!empty($contextual_links)): ?> 
    <div style='height:15px'> <?php print $contextual_links; ?> </div>
  <?php endif; ?>
  <h2 class="element-invisible">   <?php print t('You are here'); ?>  </h2>
  <div class="breadcrumb"><?php print $prefix; ?> <?php print implode(' ' . $separator . ' ', $breadcrumb); ?> </div>

</div>
<?php endif; ?>
