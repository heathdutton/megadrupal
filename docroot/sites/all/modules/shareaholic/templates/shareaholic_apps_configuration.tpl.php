<div id='app_settings'>
  <?php $page_types = ShareaholicUtilities::page_types(); ?>
  <?php $settings = ShareaholicUtilities::get_settings(); ?>

  <fieldset class="app">
    <legend><h2><i class="icon icon-recommendations"></i><?php print t('Related & Promoted Content'); ?></h2></legend>

    <span class="helper"><i class="icon-star"></i> <?php print t('Pick where you want Related Content to be displayed. Click "Customize" to customize look & feel, themes, block lists, etc.'); ?></span>
    <?php foreach($page_types as $key => $page_type) { ?>
      <?php if (isset($settings['location_name_ids']['recommendations']["{$page_type->type}_below_content"])) { ?>
        <?php $location_id = $settings['location_name_ids']['recommendations']["{$page_type->type}_below_content"] ?>
      <?php } else { $location_id = ''; } ?>
      <fieldset id='recommendations'>
        <legend><?php echo ucwords($page_type->name) ?></legend>
          <div>
            <input type="checkbox" id="recommendations_<?php echo "{$page_type->type}_below_content" ?>" name="recommendations[<?php echo "{$page_type->type}_below_content" ?>]" class="check"
            <?php if (isset($settings['recommendations']["{$page_type->type}_below_content"])) { ?>
              <?php echo ($settings['recommendations']["{$page_type->type}_below_content"] == 'on' ? 'checked' : '') ?>
            <?php } ?>>
            <input type="hidden" id="recommendations_<?php echo "{$page_type->type}_below_content_location_id" ?>" name="recommendations[<?php echo "{$page_type->type}_below_content_location_id" ?>]" value="<?php echo $location_id ?>"/>
            <label for="recommendations_<?php echo "{$page_type->type}_below_content" ?>">Below Content</label>
            <button data-app='recommendations'
                    data-location_id='<?php echo $location_id ?>'
                    data-href="recommendations/locations/{{id}}/edit"
                    class="mll btn btn-success">
            <?php print t('Customize'); ?></button>
          </div>
      </fieldset>
    <?php } ?>

    <div class="fieldset-footer">
      <span class="helper"><i class="icon-star"></i> Re-crawl your content, exclude certain pages from being recommended, etc.</span>
      <button class='app_wide_settings btn' data-href='recommendations/edit'><?php print t('Edit Related & Promoted Content Settings'); ?></button>
      <div class="app-status">
        &nbsp;&nbsp;&nbsp;&nbsp;<strong><?php print t('Status:'); ?></strong>
        <?php
          $status = ShareaholicUtilities::recommendations_status_check();
          if ($status == 'processing' || $status == 'unknown'){
            echo '<img class="shrsb_health_icon" align="top" src="' . SHAREAHOLIC_ASSET_DIR . '/img/circle_yellow.png" />' . t('Processing');
          } else {
            echo '<img class="shrsb_health_icon" align="top" src="' . SHAREAHOLIC_ASSET_DIR . '/img/circle_green.png" />' . t('Ready');
          }
        ?>
      </div>
    </div>
  </fieldset>

  <div class='clear'></div>

  <fieldset class="app">
    <legend><h2><i class="icon icon-share_buttons"></i><?php print t('Share Buttons'); ?></h2></legend>

    <span class="helper"><i class="icon-star"></i> <?php print t('Pick where you want your buttons to be displayed. Click "Customize" to customize look & feel, themes, share counters, alignment, etc.'); ?></span>

    <?php foreach($page_types as $key => $page_type) { ?>
    <fieldset id='sharebuttons'>
      <legend><?php echo ucwords($page_type->name) ?></legend>
      <?php foreach(array('above', 'below') as $position) { ?>
        <?php if (isset($settings['location_name_ids']['share_buttons']["{$page_type->type}_{$position}_content"])) { ?>
          <?php $location_id = $settings['location_name_ids']['share_buttons']["{$page_type->type}_{$position}_content"] ?>
        <?php } else { $location_id = ''; } ?>
          <div>
            <input type="checkbox" id="share_buttons_<?php echo "{$page_type->type}_{$position}_content" ?>" name="share_buttons[<?php echo "{$page_type->type}_{$position}_content" ?>]" class="check"
            <?php if (isset($settings['share_buttons']["{$page_type->type}_{$position}_content"])) { ?>
              <?php echo ($settings['share_buttons']["{$page_type->type}_{$position}_content"] == 'on' ? 'checked' : '') ?>
            <?php } ?>>
            <input type="hidden" id="share_buttons_<?php echo "{$page_type->type}_{$position}_content_location_id" ?>" name="share_buttons[<?php echo "{$page_type->type}_{$position}_content_location_id" ?>]" value="<?php echo $location_id ?>"/>
            <label for="share_buttons_<?php echo "{$page_type->type}_{$position}_content" ?>"><?php echo ucfirst($position) ?> Content</label>
            <button data-app='share_buttons'
                    data-location_id='<?php echo $location_id ?>'
                    data-href='share_buttons/locations/{{id}}/edit'
                    class="mll btn btn-success">
            <?php print t('Customize'); ?></button>
          </div>
      <?php } ?>
    </fieldset>
    <?php } ?>
    <div class='fieldset-footer'>
      <span class="helper"><i class="icon-star"></i> Brand your shares with your @Twitterhandle, pick your favorite URL shortener, share buttons for images, etc.</span>
      <button class='app_wide_settings btn' data-href='share_buttons/edit'>Edit Share Button Settings</button>
    </div>
  </fieldset>

  <fieldset class="app">
    <legend><h2><i class="icon icon-affiliate"></i><?php echo t('Monetization'); ?></h2></legend>
    <span class="helper"><i class="icon-star"></i> <?php echo t('Configure your  monetization settings from one place, including Promoted Content, Affiliate Links App, etc.'); ?></span>
    <button class='app_wide_settings btn' data-href='monetizations/edit'><?php echo t('Edit Monetization Settings'); ?></button>
  </fieldset>
</div>

<div class='clear'></div>

<div class="row" style="padding-top:20px; padding-bottom:35px;">
  <div class="span2">
    <?php print $variables['shareaholic_apps_configuration']['hidden'] ?>
    <?php print $variables['shareaholic_apps_configuration']['submit'] ?>
  </div>
</div>
