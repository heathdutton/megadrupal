<?php
/**
 * @file
 * Template file for collapse and expand all fieldsets via the Fieldset helper module.
 *
 * Available variables:
 * - $id: The unique ID of the fieldset-helper-toggle-all widget.
 */
?>
<div class="fieldset-helper-toggle-all clear-block" id="<?php print $id; ?>">
<a href="#expand-all" class="fieldset-helper-expand-all"><?php print t('Expand all'); ?></a>
|
<a href="#collapse-all" class="fieldset-helper-collapse-all"><?php print t('Collapse all'); ?></a>
</div>
