<?php

/**
 * @file
 * Override of Bootstrap block.tpl.php.
 */
?>
<li id="<?php print $block_html_id; ?>" class="<?php print $classes; ?> dropdown"<?php print $attributes; ?>>
  <a href="#" class="dropdown-toggle" data-toggle="dropdown"> <?php print $title; ?> <b class="caret"></b> </a>
  <ul class="dropdown-menu">
    <li>
      <div class="yamm-content">
        <?php print $content ?>
      </div>
    </li>
  </ul>
</li> <!-- /.block -->
