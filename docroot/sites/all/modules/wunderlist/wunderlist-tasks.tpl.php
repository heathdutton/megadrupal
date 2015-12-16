<?php
/**
 * @file
 * Template file to display tasks.
 */
?>
<div class="wunderlist-tasks">
<?php foreach ($tasks as $task) : ?>
  <div class="wunderlist-task">
    <?php print $task['avatar']; ?>
    <?php if ($task['completed']) : ?>
      <span class="task-completed">V</span>
    <?php endif; ?>
    <h3><?php print $task['title']; ?></h3>
    created at: <?php print $task['created_at']; ?>
  </div>
<?php endforeach; ?>
</div>
