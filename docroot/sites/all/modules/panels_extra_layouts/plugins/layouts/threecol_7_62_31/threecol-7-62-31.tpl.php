<?php
/**
 * @file   threecol-7-62-31.tpl.php
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Tue Dec 18 9:15:00 2012
 *
 * @brief  Template for the 7/62/31 panels layout.
 *
 *
 */
?>

<div class="panel-display panel-threecol-7-72-31-stacked clear-block" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="panel-panel line">
    <div class="inside">
      <div class="panel-panel unit panel-col-seven firstUnit">
        <?php print $content['left']; ?>
      </div>
    </div>

    <div class="panel-panel unit panel-col-sixtytwo">
      <div class="inside">
        <?php print $content['middle']; ?>
      </div>
    </div>

    <div class="panel-panel unit panel-col-thirtyone lastUnit">
      <div class="inside">
        <?php print $content['right']; ?>
      </div>
    </div>
  </div>
</div>
