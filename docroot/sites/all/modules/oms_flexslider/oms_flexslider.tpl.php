<?php
/**
 * @file
 * Orbit Media Flexslider Block template.
 */
?>
<li >
<img src="<?php
if($slide->field_oms_slider_image){
  echo file_create_url($slide->field_oms_slider_image[$slide->language][0]['uri']);
}
?>">
  <div class="flexslider-text">
    <?php
        if(isset($slide->field_oms_slider_text[0])){
          echo check_plain($slide->field_oms_slider_text[$slide->language][0]['value']);
        }
    ?>
  </div>
</li>
