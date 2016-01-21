<div class="" id="wm-toolbar">
	<div class="wm-toggle">
		<div class="wm-toggle-button"></div>
		<div class="wm-shorcuts">
			<?php print $shortcuts;?>
		</div>
	</div>
	<div class="wm-blocks">
		<div class="non-scroll-pane">
			<div class="wm-profile-wrapper">
				<?php print $picture; ?><span class="item-text"><?php print $profile;?></span><span class="item-text" style="margin-left: 20px"><?php print $logout_url;?></span>
      </div>
			<?php if(isset($status)) print $status;?>
			<ul class="wm-filter-wrapper">
				<li class="wm-filter">
					<input type="text" value="<?php print t('Filter'); ?>" id="filter"/>
				</li>
			</ul>
		</div>
		<div class="scroll-pane">
			<?php
      foreach ($scollables as $scollable) {
        print $scollable;
      }
			?>
		</div>
	</div>
</div>