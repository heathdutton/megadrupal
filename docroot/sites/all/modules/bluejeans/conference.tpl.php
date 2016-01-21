<div class="<?php print implode(' ', $classes_array); ?>" data-meetingId="<?php print $conference->meeting_id ?>">

	<header>
		<h4 class="bluejeans-conference-start">
			<span class="bluejeans-conference-start-date"><?php print $start_date; ?></span>
			<span class="bluejeans-conference-start-time"><?php print $start_time; ?></span>
		</h4>
		<div class="bluejeans-conference-title"><?php print $title ?></div>
		<div class="bluejeans-conference-messages">
			<?php // to be filled by front-end ?>
		</div>
	</header>

	<?php if ($description) : ?>
	  <div class="bluejeans-conference-description"><?php print $description ?></div>
	<?php endif ?>

	<div class="bluejeans-conference-status-wrapper">
		<span class="bluejeans-conference-indicator">

		</span>
		<span class="bluejeans-conference-actions-wrapper">
			<ul class="bluejeans-conference-actions">
		  		<?php // to be filled by front-end ?>
			</ul>
		</span>
		<span class="bluejeans-conference-status"><?php // to be filled by front-end ?></span>
		<span class="bluejeans-conference-duration"><?php print $duration ?></span>
	</div>

</div>
