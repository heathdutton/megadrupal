<?php
/**
 * @file templates/xc-browse-admin-ui.tpl.php
 * Default theme implementation of a browse UI admin display
 *
 * Available variables:
 * - $content: The the properties table of the browse UI
 * - $label: The label of the browse UI
 * - $tabs (array): The list of tabs (content panes)
 * - $message: Mesage to the user to add tabs, if there is no tabs at all
 *
 * @see template_preprocess_xc_browse_admin_ui()
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<fieldset>
  <legend><?php print $label ?></legend>

  <?php print $content; ?>

  <?php if (!empty($message)) : ?>
    <p><?php print $message ?></p>
  <?php else : ?>

    <?php foreach ($tabs as $tab) : ?>
      <?php print $tab; ?>
    <?php endforeach; ?>

  <?php endif ?>
</fieldset>
