<?php

/**
 * @file
 * Default implementation of the Commerce Views Pane content template.
 *
 * Available variables:
 * - $content: The rendered view content
 * - $title: The safe title of the view
 * - $order: The commerce_order entity
 *
 * @see template_preprocess()
 * @see template_process()
 */
?>
<?php if ($content): ?>
<div class="commerce-views-pane-view-content-wrapper">
<?php if ($title): ?>
<h3 class="commerce-views-pane-view-title"><?php print $title; ?></h3>
<?php endif; ?>
<div class="commerce-views-pane-view-content"><?php print $content; ?></div>
</div>
<?php endif; ?>
