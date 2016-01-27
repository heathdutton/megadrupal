<<?php print $type;
       print (isset($class) && count($class) ? ' class="'.implode(' ', $class).'"' : '');
       print (isset($title) ? ' title="'.$title.'"' : ''); ?>><?php
  print $markup; ?>
</<?php print $type; ?>>