<?php // $Id: block-header.tpl.php,v 1.2 2009/02/16 16:18:13 njt1982 Exp $ ?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?> block-header">
  <div class="content">
    <?php if ($block->subject): print '<h2>'. $block->subject .'</h2>'; endif;?>
    <?php print $block->content ?>
  </div>
</div>
