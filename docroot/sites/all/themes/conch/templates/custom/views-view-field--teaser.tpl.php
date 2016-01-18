<?php

/**
 * @file views-view-field--teaser.tpl.php
 */
 
  if ($row->node_data_field_image_field_image_fid) {
    print '<div class="grid_6 omega">' . $output . '</div>';
  }
  else { 
    print $output;
  }
