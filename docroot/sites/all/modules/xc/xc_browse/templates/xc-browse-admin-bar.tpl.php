<?php
/**
 * @file templates/xc-browse-admin-bar.tpl.php
 * Default theme implementation of a navigation bar on the browse admin UI
 *
 * Available variables:
 * - $label: The label of the bar
 * - $content: The properties of the form element
 * - $edit_link: link to edit this form element
 * - $delete_link: link to delete this form element
 *
 * @see template_preprocess_xc_browse_admin_bar()
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */?>
<fieldset class="collapsible collapsed">
  <legend><?php print $label ?></legend>

  <?php if (!empty($content)) : ?>
    <p><?php print $content; ?></p>
  <?php endif; ?>

  <p><?php print $edit_link ?> &mdash; <?php print $delete_link ?></p>
</fieldset>
