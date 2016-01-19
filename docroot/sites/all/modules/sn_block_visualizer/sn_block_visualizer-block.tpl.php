
<?php

/**
 * @file
 * This file is holds the presentation of the page.
 */
global $base_path;
?>
  
  <div id="sn-block-list" >
    <div id="sn-block-rel-div">
      <div id="sn-block-popup" class="sn-block-popup" style="display: block;">
        <div class="sn-block-heading">
            <span>
              Blocks 
              <span id="sn-block-min">
                <img src = "<?php echo $base_path.drupal_get_path('module', 'sn_block_visualizer'); ?>/icon_002.gif">
               </span>
            </span>
         
          </div>
        <div class="sn-block-data">
          <div id="-1" class="droppable-sortable">
            <?php print $blocks; ?>
          </div>
        </div>
      </div>  
    </div>
  </div>
