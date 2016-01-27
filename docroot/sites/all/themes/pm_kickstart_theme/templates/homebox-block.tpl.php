<?php

/**
 * @file
 * homebox-block.tpl.php
 * Default theme implementation each homebox block.
 */
?>
<div id="homebox-block-<?php print $block->key; ?>" class="pmdash-homebox-block <?php print $block->homebox_classes ?> clearfix block block-<?php print $block->module ?>">
  <div class="homebox-portlet-inner panel panel-default">
    <div class="panel-heading">
      <h3 class="portlet-header panel-title clearfix">
        <div class="btn-group btn-group-xs pull-right" role="group" aria-label="...">
          <?php if ($block->closable): ?>
            <a class="btn btn-default portlet-close"><i class="fa fa-close"></i></a>
          <?php endif; ?>
          <a class="btn btn-default portlet-maximize"><i class="fa fa-expand hide"></i><i class="fa fa-compress hide"></i></a>
          <a class="btn btn-default portlet-minus"><i class="fa fa-minus hide"></i><i class="fa fa-plus hide"></i></a>
          <?php if ($page->settings['color'] || isset($block->edit_form)): ?>
           <a class="btn btn-default portlet-settings"><i class="fa fa-wrench"></i></a>
          <?php endif; ?>
        </div>
        <span class="portlet-title"><?php print $block->subject ?></span>
      </h3>
      <div class="portlet-config">
        <?php if ($page->settings['color']): ?>
          <div class="clearfix"><div class="homebox-colors">
            <span class="homebox-color-message"><?php print t('Select a color') . ':'; ?></span>
            <?php for ($i=0; $i < HOMEBOX_NUMBER_OF_COLOURS; $i++): ?>
              <span class="homebox-color-selector" style="background-color: <?php print $page->settings['colors'][$i] ?>;">&nbsp;</span>
            <?php endfor ?>
          </div></div>
        <?php endif; ?>
        <?php if (isset($block->edit_form)): print $block->edit_form; endif; ?>
      </div>
    </div>
     <div class="portlet-content content"><?php if (is_string($block->content)){ print $block->content; } else { print drupal_render($block->content); } ?></div>
  </div>
</div>
