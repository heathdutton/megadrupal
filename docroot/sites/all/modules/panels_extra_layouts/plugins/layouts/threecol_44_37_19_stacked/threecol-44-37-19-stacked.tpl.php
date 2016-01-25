<?php
/**
 * @file   threecol-44-37-19-stacked.tpl.php
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Tue Dec 18 9:29:00 2012
 *
 * @brief  Template for the 44/37/19 panels layout.
 *
 *
 */
?>

<div class="panel-display panel-threecol-44-37-19-stacked clear-block" <?php if (!empty($css_id)) { print "id=\"$css_id\"";} ?>>

  <div class="panel-panel line">
    <div class="panel-panel unit left">
      <div class="inside">
        <?php print $content['left']; ?>
      </div>
    </div>

    <div class="panel-panel unit right lastUnit">
      <div class="panel-panel line">
        <div class="panel-panel unit top-middle">
          <div class="inside">
            <?php print $content['top_middle']; ?>
          </div>
        </div>

        <div class="panel-panel unit top-right lastUnit">
          <div class="inside">
            <?php print $content['top_right']; ?>
          </div>
        </div>
      </div>

      <div class="inside">
        <?php print $content['right']; ?>
      </div>
    </div>
  </div>

  <div class="panel-panel footer">
    <div class="inside">
      <?php print $content['footer']; ?>
    </div>
  </div>
</div>
