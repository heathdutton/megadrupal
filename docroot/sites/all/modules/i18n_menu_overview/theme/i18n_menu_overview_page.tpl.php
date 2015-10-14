<?php
/**
 * @file
 * tpl file to display the menu per language.
 */

?>
<div class="action_add">
  <?php
  $dummy = explode('_', arg(2));
  $menu = isset($menu) ? $menu : $dummy[0];
  print l(t('Add link'), 'admin/structure/menu/manage/' . $menu .
    '/add', array(
    'query' => array(
      'destination' => 'admin/structure/' . arg(2),
      'language_id' => $language_id,
    ),
  ));
  ?>
</div>
<div class="action_edit">
  <?php

  print l(t('Edit menu'), 'admin/structure/menu/manage/' . $menu . '/edit', array('query' => array('destination' => 'admin/structure/' . arg(2))));
  ?>
</div>
<?php
  print $table;
  print $remaining_form_elements;
?>
