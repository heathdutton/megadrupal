<?php

/**
 * @file
 * maestro-task-title.tpl.php
 */

 ?>

<?php if ($regen_flag === TRUE) { ?>
    <div id="task_title<?php print $tdid; ?>" class="tm-bl maestro_task_title" TITLE="<?php print $regen_mode; ?>"><span class="small_red_circle"></span>
<?php } else { ?>
      <div id="task_title<?php print $tdid; ?>" class="tm-bl maestro_task_title">
<?php } ?>
<?php print $taskname; ?>
    </div>
