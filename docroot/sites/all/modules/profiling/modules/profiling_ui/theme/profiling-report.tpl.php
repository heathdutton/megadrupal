<?php

/**
 * @file
 * Profiling report default template.
 * 
 * Please don't override unless you really need it because of a custom theme
 * incompatiblity.
 */
?>
<ol>
<?php foreach ($items_index as $name => $title): ?>
  <li><a href="<?php print '#' . $name; ?>"><?php print $title; ?></a></li>
<?php endforeach; ?>
</ol>
<?php foreach ($items as $name => $item): ?>
<a name="<?php print $name; ?>"></a>
<?php print render($item); ?>
<?php endforeach; ?>
