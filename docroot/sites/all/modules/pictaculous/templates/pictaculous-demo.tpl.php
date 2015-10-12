<?php
/**
 * Template for pictaculous demo content.
 */
?>
<?php if (!empty($object)): ?>
	<div id="pictaculous-demo" class="">
			<?php if (!empty($object['info'])): ?>
				<fieldset class="form-wrapper" id="pictaculous-info">
					<legend>
						<span class="fieldset-legend">Pictaculous Info</span>
					</legend>
					<div class="fieldset-wrapper">
						<?php
							$output = '<div class="theme-wrapper" style="display:inline-block;margin-right:20px;padding:10px;border:1px solid #666;">';
							$output .= '<div class="thumbnail" style="text-align:center;box-sizing:border-box;padding:10px;">';
							$output .= '<a href="'.$object['info']->url.'"><img src="'.$object['info']->url.'"></a>';
							$output .= '</div>';

							$output .= '<div class="colors">';
							$output .= '<h3>'.l($object['title'], $object['info']->url).'</h3>';
							$output .= '<ul>';
							foreach ($object['info']->colors as $color) {
								$output .= '<li><span style="background-color:#'.$color.';width:20px;height:20px;padding-left:20px;"></span> '.$color.'</li>';
							}
							$output .= '</ul>';
							$output .= '</div>';
							$output .= '</div>';
							print $output;
						?>
					</div>
				</fieldset>
			<?php endif; ?>
			<?php if (!empty($object['kuler_themes'])): ?>
				<fieldset class="form-wrapper" id="pictaculous-kuler">
					<legend>
						<span class="fieldset-legend">Adobe Kuler Themes</span>
					</legend>
					<div class="fieldset-wrapper">
						<div class="fieldset-description">Suggested themes by Adobe Kuler based on the image.</div>
						<?php
							foreach ($object['kuler_themes'] as $key => $value) {
								$output = '<div class="theme-wrapper" style="display:inline-block;margin-right:20px;padding:10px;border:1px solid #666;">';
								$output .= '<div class="thumbnail" style="text-align:center;box-sizing:border-box;padding:10px;">';
								$output .= '<a href="'.$value->url.'"><img src="'.$value->thumb.'"></a>';
								$output .= '</div>';

								$output .= '<div class="colors">';
								$output .= '<h3>'.l($value->title, $value->url).'</h3>';
								$output .= '<ul>';
								foreach ($value->colors as $color) {
									$output .= '<li><span style="background-color:#'.$color.';width:20px;height:20px;padding-left:20px;"></span> '.$color.'</li>';
								}
								$output .= '</ul>';
								$output .= '</div>';
								$output .= '</div>';
								print $output;
							}
						?>
					</div>
				</fieldset>
			<?php endif; ?>
			<?php if (!empty($object['cl_themes'])): ?>
				<fieldset class="form-wrapper" id="pictaculous-colourlovers">
					<legend>
						<span class="fieldset-legend">COLURLovers Themes</span>
					</legend>
					<div class="fieldset-wrapper">
						<div class="fieldset-description">Suggested themes by COLOURLovers based on the image.</div>
						<?php
							foreach ($object['cl_themes'] as $key => $value) {
								$output = '<div class="theme-wrapper" style="display:inline-block;margin-right:20px;padding:10px;border:1px solid #666;">';
								$output .= '<div class="thumbnail" style="text-align:center;box-sizing:border-box;padding:10px;">';
								$output .= '<a href="'.$value->url.'"><img src="'.$value->thumb.'"></a>';
								$output .= '</div>';

								$output .= '<div class="colors">';
								$output .= '<h3>'.l($value->title, $value->url).'</h3>';
								$output .= '<ul>';
								foreach ($value->colors as $color) {
									$output .= '<li><span style="background-color:#'.$color.';width:20px;height:20px;padding-left:20px;"></span> '.$color.'</li>';
								}
								$output .= '</ul>';
								$output .= '</div>';
								$output .= '</div>';
								print $output;
							}
						?>
					</div>
				</fieldset>
			<?php endif; ?>
	</div>
<?php endif; ?>
