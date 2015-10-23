<?php
/**
 * @file
 * Implementation to present a Panels based layout.
 *
 * Available variables:
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout.
 * - $css_id: unique id if present.
 * - $panel_prefix: prints a wrapper when this template is used in a context,
 *   such as when rendered by Display Suite or other module.
 * - $panel_suffix: closing element for the $prefix.
 */
$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="panel-display penfield2 clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['penfield2_top']): ?>
  <div class="region-penfield2-top span-8">
    <div class="region-inner clearfix">
      <?php print $content['penfield2_top']; ?>
    </div>
  </div>
  <div class="clear"></div>
  <?php endif; ?>
  <div id="wb-main" role="main">
    <div id="wb-main-in">
      <div class="region-penfield2-content-header span-6">
        <div class="region-inner clearfix">
          <?php print $content['penfield2_content_header']; ?>
        </div>
      </div>
      <div class="clear"></div>
      <div class="region-penfield2-first span-3">
        <div class="region-inner clearfix">
          <?php print $content['penfield2_first']; ?>
        </div>
      </div>
      <div class="region-penfield2-second span-3">
        <div class="region-inner clearfix">
          <?php print $content['penfield2_second']; ?>
        </div>
      </div>
      <div class="clear"></div>     
    </div>
  </div>
  <div id="wb-sec">
    <div id="wb-sec-in">
      <div class="region-penfield2-third span-2 row-start">
        <div class="region-inner clearfix">
          <nav role="navigation" id="wb-sec">
            <h2 id="wb-side-nav"><?php print t('Section menu'); ?></h2>
            <div class="wb-sec-def">
              <?php print $content['penfield2_third']; ?>
            </div>
          </nav>
        </div>
      </div>
    </div>
  </div>
  <?php if ($content['penfield2_bottom']): ?>
  <div class="region-penfield2-bottom span-8">
    <div class="region-inner clearfix">
      <?php print $content['penfield2_bottom']; ?>
    </div>
  </div>
  <div class="clear"></div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
