<?php

/**
 * @file
 * maestro-task-set-process-variable.tpl.php
 */

?>

<div class="maestro_task">
  <div class="t"><div class="b"><div class="r"><div class="l"><div class="bl"><div class="br"><div class="tl-gry"><div class="tr-gry">
    <div>
    <?php if ($ti->get_regen_flag() === TRUE) { ?>
      <div id="task_title<?php print $tdid; ?>" class="tm-gry maestro_task_title" TITLE="<?php print $ti->get_regen_mode(); ?>"><span class="small_red_circle"></span>
    <?php } else { ?>
      <div id="task_title<?php print $tdid; ?>" class="tm-gry maestro_task_title">
    <?php } ?>
    <?php print filter_xss($taskname); ?>
    </div>
    </div>
    <div class="maestro_task_body">
      <?php print t('Fire Trigger Task'); ?>
    </div>

  </div></div></div></div></div></div></div></div>
</div>
