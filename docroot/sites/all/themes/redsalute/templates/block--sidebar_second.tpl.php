<?php
?>
<div class="Block">
    <div class="Block-body">
        <div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module; ?>">
            <?php if ($block->subject): ?>
            <div class="BlockHeader">
                <div class="l"></div>
                <div class="r"></div>
                <div class="header-tag-icon">
                    <div class="t"><?php print $block->subject; ?></div>
                </div>
            </div>
            <?php endif;?>
            <div class="BlockContent">
                <div class="BlockContent-tl"></div>
                <div class="BlockContent-tr"></div>
                <div class="BlockContent-bl"></div>
                <div class="BlockContent-br"></div>
                <div class="BlockContent-tc"></div>
                <div class="BlockContent-bc"></div>
                <div class="BlockContent-cl"></div>
                <div class="BlockContent-cr"></div>
                <div class="BlockContent-cc"></div>
                <div class="BlockContent-body">
                  <?php print $content; ?>
                </div>
            </div>
        </div>
    </div>
</div>
