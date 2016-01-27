<?php

/**
 * @file
 * Default theme implementation to present google chat.
 */
?>

<div id="googlechat-wrapper">
  <div id="googlechat">
    <div class="googlechat_notification_sound_panel googlechat-corner-all">
      <ul>
      <?php foreach (array('sound_on' => 1, 'sound_off' => 2) as $key => $val):?>
        <?php $class = ($googlechatpanelsoundstatus == $val) ? 'googlechat-icon-check' : 'clear_background_image'; ?>
        <?php $text = ($key == 'sound_on') ? 'Yes' : 'No'; ?>
        <li class="<?php print $key;?>">
          <a href="javascript:;">
            <span class="googlechat-icon-hover <?php print $class;?>"></span><?php print $text;?>
          </a>
        </li>
      <?php endforeach;?>
      </ul>
    </div>
    <div class="status_indicator_panel googlechat-corner-all">
      <ul class="status">
        <li><img class="J-N-JX googlechatstatus_1 googlechatstatus" src="<?php echo base_path() . drupal_get_path('module', 'googlechat');?>/images/cleardot.gif" alt="">Available</li>
        <li><img class="J-N-JX googlechatstatus_2 googlechatstatus" src="<?php echo base_path() . drupal_get_path('module', 'googlechat');?>/images/cleardot.gif" alt="">Busy</li>
        <li><img class="J-N-JX googlechatstatus_4 googlechatstatus" src="<?php echo base_path() . drupal_get_path('module', 'googlechat');?>/images/cleardot.gif" alt="">Invisible</li>
      </ul>
    </div>
    <div class="googlechat_subpanel googlechat-box-all googlechat-widget-content">
	    <div class="googlechat_subpanel_title googlechat-corner-all">
	      <div style="height:100%; margin: 0px;">
	        <img src="<?php print $googlechatpanelimage; ?>" alt="<?php print $googlechatpanelname; ?>" width="28">
	      </div>
	      <div class="status_indicator">
	        <img style="float:left;margin-top:2px;" class="googlechatstatus googlechatstatus_<?php print $googlechatpanelstatus;?>" src="<?php echo base_path() . drupal_get_path('module', 'googlechat');?>/images/cleardot.gif" alt="Online">
	        <a href="javascript:;"  style="height: 16px;float:right;">
            <span class="googlechat-icon googlechat-icon-arrow-down"></span>
	        </a>
	      </div>

	      <div class="googlechat_titlebar_min">
	        <a href="javascript:;" style="float: left;height: 16px;">
	          <span class="googlechat-icon googlechat-icon-minus"></span>
	        </a>
	      </div>
	      <div class="googlechat_setting_wrapper">
	        <a class="googlechat_notification_sound" href="javascript:;" title="Play Sound" style="float: left;height: 16px;">
	          <span class="googlechat-icon googlechat-icon-wrench"></span>
	        </a>
	      </div>
	    </div>


  	  <div id="googlechatbuddylist"><?php print $googlechatpanels; ?></div>
      <div id="googlechat_panel_footer"><em>Chat (<span><?php print $googlechatpanelbuddycount;?></span>)</em></div>
  	</div>
  </div>
</div>
