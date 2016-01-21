<?php
/**
 * @file templates/xc-browse-render-bar.tpl.php
 * Default theme implementation of a single navigation bar on the browse form
 *
 * Available variables:
 * - $id: The id of the bar
 * - $label: The label of the bar
 * - $elements (array): The form elements of the tab. Its keys are:
 *   - message: a message if there is no form elements at all
 *   - list (array): the list of themed form element properties
 *   - add_link: link to add more form elements
 * - $bars (array): the bars associated to the tab. Its keys are:
 *   - message: a message if there is no bars at all
 *   - list (array): the list of themed bar properties
 *   - add_link: link to add more bars
 * - $edit_link: link to modify the tab
 * - $delete_link: link to delete the tab
 *
 * @see template_preprocess_xc_browse_admin_tab()
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
?>
<fieldset class="collapsible collapsed">
  <legend><?php print $label ?> tab</legend>

  <?php print $content; ?>

  <?php if (!empty($elements['message'])) : ?>
    <p><?php print $elements['message']; ?></p>
  <?php else : ?>
    <h2><?php print t('List of form elements') ?></h2>

    <?php foreach ($elements['list'] as $element) : ?>
      <?php print $element; ?>
    <?php endforeach ?>

    <p><?php print $elements['add_link'] ?></p>
  <?php endif; ?>

  <?php if (!empty($bars['message'])) : ?>
    <p><?php print $bars['message']; ?></p>
  <?php else : ?>

    <h2><?php print t('List of navigation lists') ?></h2>

    <?php foreach ($bars['list'] as $bar) : ?>
      <?php print $bar; ?>
    <?php endforeach ?>

    <p><?php print $bars['add_link']; ?></p>
  <?php endif; ?>

  <p>
    <a href="<?php print $edit_link ?>"><?php print t('edit') ?></a> &mdash;
    <a href="<?php print $delete_link ?>"><?php print t('delete') ?></a>
  </p>
</fieldset>
