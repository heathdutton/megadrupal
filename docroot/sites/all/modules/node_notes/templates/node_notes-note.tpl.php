<?php

/**
 * @file
 * Default theme implementation to display a note.
 *
 * Variables available:
 * - $noteid: The noteid of the note.
 * - $nid: The nid the note is connected to.
 * - $uid: The uid of the note author.
 * - $author: The author of the note.
 * - $created: The created date of the note.
 * - $body: The body of the note.
 * - $classes_array: An array containing the classes on the note parent <li>.
 * - $classes: The imploded string of class names in $classes_array.
 *
 * @see template_preprocess_node_notes_note()
 */
?>
<li id="node-note-<?php echo $noteid ?>" class="<?php echo $classes ?>">
  <div class="note-info">
    <div class="note-id"><a href="#node-note-<?php echo $noteid ?>">#<?php echo $noteid ?></a></div>
    <div class="author">
      <label>By:</label> <?php echo $author ?>
    </div>
    <div class="created">
      <?php echo $created ?>
    </div>
    <?php if ($links): ?>
      <div class="links">
        <?php echo $links ?>
      </div>
    <?php endif; ?>
  </div>
  <div class="body">
    <?php echo $body ?>
  </div>
</li>
