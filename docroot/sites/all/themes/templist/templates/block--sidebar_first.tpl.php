<?php
?>
<div class="art-Block">
    <div class="art-Block-tl"></div>
    <div class="art-Block-tr"></div>
    <div class="art-Block-bl"></div>
    <div class="art-Block-br"></div>
    <div class="art-Block-tc"></div>
    <div class="art-Block-bc"></div>
    <div class="art-Block-cl"></div>
    <div class="art-Block-cr"></div>
    <div class="art-Block-cc"></div>
    <div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="art-Block-body clear-block block block-<?php print $block->module ?>">
        <?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
        <div class="art-BlockHeader">
            <div class="l"></div>
		        <div class="r"></div>
            <div class="art-header-tag-icon">
                <div class="t" >
                    <?php print $block->subject ?>
                </div>
            </div>
        </div>
        <?php endif;?>
        <?php print render($title_suffix); ?>
        <div class="art-BlockContent">
            <div class="art-BlockContent-tl"></div>
		    <div class="art-BlockContent-tr"></div>
		    <div class="art-BlockContent-bl"></div>
		    <div class="art-BlockContent-br"></div>
		    <div class="art-BlockContent-tc"></div>
		    <div class="art-BlockContent-bc"></div>
		    <div class="art-BlockContent-cl"></div>
		    <div class="art-BlockContent-cr"></div>
		    <div class="art-BlockContent-cc"></div>
            <div class="art-BlockContent-body content">
           <?php print render($title_suffix); ?>

           <div class="content"<?php print $content_attributes; ?>>
                <?php print $content ?>
           </div>
            </div>
        </div>
        <div class="clearfix"></div>
    </div>
</div>
