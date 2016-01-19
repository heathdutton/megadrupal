<?php
/**
 * @file
 * Renders the suggested content block
 */
 ?>
 <ul>
<?php foreach ($nodes as $link): ?>
<?php echo render($link); ?>
<?php endforeach; ?>
</ul>
