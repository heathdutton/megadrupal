<?php
/**
 * @file
 * Outputs the view.
 */

$base_path = base_path();

if (!$options['views_jqfx_supersized']['ss_theme'] != 'none') :
    // Supersized Shutter theme.
?>

	<div id="views-jqfx-supersized-<?php print $id; ?>">

		<!--Thumbnail Navigation-->
		<div id="prevthumb"></div>
		<div id="nextthumb"></div>

		<!--Arrow Navigation-->
		<a id="prevslide" class="load-item"></a>
		<a id="nextslide" class="load-item"></a>

		<div id="thumb-tray" class="load-item">
			<div id="thumb-back"></div>
			<div id="thumb-forward"></div>
		</div>

		<!--Time Bar-->
		<?php if ($options['views_jqfx_supersized']['theme']['progress_bar'] == '1') : ?>
		<div id="progress-back" class="load-item">
			<div id="progress-bar"></div>
		</div>
	<?php endif; ?>

	<!--Control Bar-->
	<div id="controls-wrapper" class="load-item">
		<div id="controls">

			<a id="play-button"><img id="pauseplay" src="<?php print $base_path; ?>sites/all/libraries/supersized/slideshow/img/pause.png"/></a>

			<!--Slide counter-->
			<div id="slidecounter">
				<span class="slidenumber"></span> / <span class="totalslides"></span>
			</div>

			<!--Slide captions displayed here-->
			<div id="slidecaption"></div>

			<!--Thumb Tray button-->
			<a id="tray-button"><img id="tray-arrow" src="<?php print $base_path; ?>sites/all/libraries/supersized/slideshow/img/button-tray-up.png"/></a>

			<!--Navigation-->
			<ul id="slide-list"></ul>

		</div>
	</div>
</div>

<?php endif; ?>
