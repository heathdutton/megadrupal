<?php
/**
 * @file
 * Renders a summary string of a groups permissions.
 * JQuery will render the attribute "hover_text"  if user has the mouse over it.
 */
?>
<div>
<?php if ($role) :?><?php echo $role ?>:<?php endif ?>
<?php foreach ($allow as $a) :?>
  <span class="odir-access-hover-me" hover_text="<?php echo $a['title'] ?>"><?php echo $a['shortname'] ?></span>
<?php endforeach ?>
</div>
