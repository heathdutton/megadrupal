<?php

/**
 * @file
 * maestro-task-content-type.tpl.php
 */

?>

<div class="maestro_task">
  <div class="t"><div class="b"><div class="r"><div class="l"><div class="bl"><div class="br"><div class="tl-bl"><div class="tr-bl">
    <div>
    <?php if ($ti->get_regen_flag() === TRUE) { ?>
      <div id="task_title<?php print $tdid; ?>" class="tm-bl maestro_task_title" TITLE="<?php print $ti->get_regen_mode(); ?>"><span class="small_red_circle"></span>
    <?php } else { ?>
      <div id="task_title<?php print $tdid; ?>" class="tm-bl maestro_task_title">
    <?php } ?>
    <?php print filter_xss($taskname); ?>
    </div>
    </div>
    <div class="maestro_task_body">
      <?php print t('Content Type Task'); ?><br />
      <div id="task_assignment<?php print $tdid; ?>"><?php print $ti->getAssignmentDisplay(); ?></div>
    </div>

  </div></div></div></div></div></div></div></div>
</div>
