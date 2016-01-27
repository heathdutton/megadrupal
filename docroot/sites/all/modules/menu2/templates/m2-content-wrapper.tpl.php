<<?php print $type;
       print (isset($class) && count($class) ? ' class="'.implode(' ', $class).'"' : '');
       print (isset($rel) ? ' rel="'.$rel.'"' : ''); ?>><?php
  print $markup; ?>
</<?php print $type; ?>>