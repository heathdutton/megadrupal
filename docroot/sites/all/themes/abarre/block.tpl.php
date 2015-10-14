<?php
// $Id:$
?>

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?>">  
    <h2 class="title"> <?php print $block->subject; ?> </h2>
    <div class="content">
      <?php print render($content); ?>
    </div>    
</div>
