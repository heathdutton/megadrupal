<?php

/**
 * @file
 * Default theme implementation to display the notes.
 *
 * Variables available:
 * - $notes: The array of the notes.
 * - $notes_list: The themed output of node_note-note.tpl.php
 * - $node: The node object the notes are connected to.
 *
 * @see template_preprocess_node_notes_wrapper()
 */
?>
<?php if ($notes) : ?>
  <h2 class="node-notes-title"><?php echo t('Notes');?></h2>
  <ul class="node-notes-wrapper">
    <?php echo $notes_list; ?>
  </ul>
  <?php echo $pager; ?>
<?php else : ?>
  <h2 class="node-notes-title"><?php echo t('There are no notes');?></h2>
<?php endif; ?>
