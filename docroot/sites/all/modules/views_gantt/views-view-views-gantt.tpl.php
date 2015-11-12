<?php
/**
 * @file
 * Defines the templated used to produce the the Gantt Chart.
 */
?>
<?php
	$height = isset($view->style_options['height']) && $view->style_options['height'] ? $view->style_options['height'] : '300';
	$style = "style='height:{$height}px; width: 100%;'";
?>
<div class="gantt-wrapper default">
	<div class="controls_bar">
		<strong>Scaling: &nbsp;</strong>
		<label><input type="radio" id="scale1" name="scale" value="1" /><span>Hours</span></label>
		<label><input type="radio" id="scale2" name="scale" value="2" /><span>Days</span></label>
		<label><input type="radio" id="scale3" name="scale" value="3" /><span>Months</span></label>
	</div>
	<div <?php print $style; ?> id="GanttDiv" class="gantt-div">
	</div>	
	<a class="gantt-fullscreen" data-mode="full">Fullscreen</a>
</div>
<div class="gantt-wrapper full" style="display:none;">
	<div class="controls_bar">
		<strong>Scaling: &nbsp;</strong>
		<label><input type="radio" id="scale4" name="scale2" value="1" /><span>Hours</span></label>
		<label><input type="radio" id="scale5" name="scale2" value="2" /><span>Days</span></label>
		<label><input type="radio" id="scale6" name="scale2" value="3" /><span>Months</span></label>
	</div>
	<div id="GanttDivFullscreen" class="gantt-div">
	</div>	
	<a class="gantt-fullscreen" data-mode="default">Exit Fullscreen</a>
</div>
