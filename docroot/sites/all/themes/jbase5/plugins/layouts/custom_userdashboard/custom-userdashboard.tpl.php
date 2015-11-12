<?php
/**
 * @file
 * Template for a 2 column panel layout.
 *
 * This template provides a two column panel display layout, with
 * each column roughly equal in width.
 *
 * Variables:
 * - $id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout. This layout supports the following sections:
 *   - $content['left']: Content in the left column.
 *   - $content['right']: Content in the right column.
 */
?>

<div class="panel-display panel-custom-userdashboard clear-block clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="panel-inner">

    <?php if ($content['content_top']): ?>
    <div class="panel-content-top panel-column">
      <div class="inside">
        <?php print $content['content_top']; ?>
      </div>
    </div><!-- /panel-content-top -->
    <?php endif; ?>

    <?php if ($content['header_top_col_first'] || $content['header_top_col_middle'] || $content['header_top_col_last'] || $content['header_row'] || $content['header_bottom_col_first'] || $content['header_bottom_col_last']): ?>
    <div class="panel-header">
      <div class="panel-header-inner clearifx">

        <div class="corner-wrapper corner-gray-bg">
          <div class="corner-inner">
            <div class="corner-top"><div class="corner-top-right corner"></div><div class="corner-top-left corner"></div></div><!-- /corner-top -->
            <div class="corner-content clearfix">
              <div class="corner-content-inner">
        
                <?php if ($content['header_top_col_first'] || $content['header_top_col_middle'] || $content['header_top_col_last']): ?>
                <div class="panel-header-top">
                  <div class="panel-header-top-inner clearfix">
                    <?php if ($content['header_top_col_first']): ?>
                    <div class="panel-header-top-col-first panel-column">
                      <div class="inside">
                        <?php print $content['header_top_col_first']; ?>
                      </div>
                    </div><!-- /panel-header-top-col-first -->
                    <?php endif; ?>
                
                    <?php if ($content['header_top_col_middle']): ?>
                    <div class="panel-header-top-col-middle panel-column">
                      <div class="inside">
                        <?php print $content['header_top_col_middle']; ?>
                      </div>
                    </div><!-- /panel-header-top-col-middle -->
                    <?php endif; ?>
                
                    <?php if ($content['header_top_col_last']): ?>
                    <div class="panel-header-top-col-last panel-column">
                      <div class="inside">
                        <?php print $content['header_top_col_last']; ?>
                      </div>
                    </div><!-- /panel-header-top-col-last -->
                    <?php endif; ?>
                  </div><!-- /panel-header-top-inner -->
                </div><!-- /panel-header-top -->
                <?php endif; ?>
        
                <?php if ($content['header_row']): ?>
                <div class="panel-header-row clearfix">
                  <div class="inside">
                    <?php print $content['header_row']; ?>
                  </div>
                </div><!-- /panel-header-row -->
                <?php endif; ?>
        
                <?php if ($content['header_bottom_col_first'] || $content['header_bottom_col_last']): ?>
                <div class="panel-header-bottom">
                  <div class="panel-header-bottom-inner clearfix">
                    <?php if ($content['header_bottom_col_first']): ?>
                    <div class="panel-header-bottom-col-first panel-column">
                      <div class="inside">
                        <?php print $content['header_bottom_col_first']; ?>
                      </div>
                    </div><!-- /panel-header-bottom-col-first -->
                    <?php endif; ?>
                
                    <?php if ($content['header_bottom_col_last']): ?>
                    <div class="panel-header-bottom-col-last panel-column">
                      <div class="inside">
                        <?php print $content['header_bottom_col_last']; ?>
                      </div>
                    </div><!-- /panel-header-bottom-col-last -->
                    <?php endif; ?>
                  </div><!-- /panel-header-bottom-inner -->
                </div><!-- /panel-header-bottom -->
                <?php endif; ?>
         
              </div><!-- /corner-content-inner -->
            </div><!-- /corner-content -->
          <div class="corner-bottom"><div class="corner-bottom-right corner"></div><div class="corner-bottom-left corner"></div></div><!-- /corner-bottom -->
          </div><!-- /corner-inner -->
        </div><!-- /corner-wrapper --> 

      </div><!-- /panel-header-inner -->
    </div><!-- /panel-header -->
    <?php endif; ?>

    <?php if ($content['content_bottom']): ?>
    <div class="panel-content-bottom clearfix">
      <div class="inside">
        <?php print $content['content_bottom']; ?>
      </div>
    </div><!-- /panel-content-bottom -->
    <?php endif; ?>
    
  </div><!-- /panel-inner -->
</div><!-- /panel-display -->