<?php
?>
<div class="app_widget">
    <div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="widget clear-block block block-<?php print $block->module ?>">
        <?php if (!empty($block->subject)): ?>
        <h4>
            <a class="hide_widget" title="Toggle" href="#">
                <?php
                if ($block->module=="search") {
                    print "Find it!";
                }
                else { ?>
                <?php print $block->subject ?>
                <?php } ?>
            </a>
        </h4>
        <?php endif;?>
        <div class="content"><!--<?php print $block->content ?>--><?php print $content; ?>
        </div>
    </div>
</div>
