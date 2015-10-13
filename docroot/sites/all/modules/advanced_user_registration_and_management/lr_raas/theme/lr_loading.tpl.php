<?php
drupal_add_css((drupal_get_path('module', 'lr_raas') .'/css/lr_loading.css'));
$loading_image = $GLOBALS['base_url'] . '/' . drupal_get_path('module', 'lr_social_login') . '/images/loading.gif';
?>
<div class="overlay" id="lr-loading" style="display: none;">
  <div class="circle">
    <div id="imganimation">
      <img src="<?php print $loading_image; ?>" alt="LoginRadius Processing"
           style="margin-top: -66px;margin-left: -73px;width: 150px;">
    </div>
  </div>
  <div></div>
</div>
