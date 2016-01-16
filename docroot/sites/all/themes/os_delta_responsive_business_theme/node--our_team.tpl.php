<div class="contTeam">
	<h5><?php print $title; ?></h5>
	<?php if(isset($node->field_position['und'][0]['value'])) print '<h5 class="position">'.$node->field_position['und'][0]['value'].'</h5>'; ?>
	<?php if(isset($node->field_description['und'][0]['value'])) print '<p class="desc">'.$node->field_description['und'][0]['value'].'</p>'; ?>
	<div class="iconsTeam">
		<?php 
			if(isset($node->field_twitter_url['und'][0]['value'])) print '<a class="fa fa-twitter" target="_blank" href="'.$node->field_twitter_url['und'][0]['value'].'"></a>';
			if(isset($node->field_facebook_url['und'][0]['value'])) print '<a class="fa fa-facebook" target="_blank" href="'.$node->field_facebook_url['und'][0]['value'].'"></a>';
			if(isset($node->field_flickr_url['und'][0]['value'])) print '<a class="fa fa-flickr" target="_blank" href="'.$node->field_flickr_url['und'][0]['value'].'"></a>';
			if(isset($node->field_linkedin_url['und'][0]['value'])) print '<a class="fa fa-linkedin" target="_blank" href="'.$node->field_linkedin_url['und'][0]['value'].'"></a>';
			if(isset($node->field_youtube_url['und'][0]['value'])) print '<a class="fa fa-youtube" target="_blank" href="'.$node->field_youtube_url['und'][0]['value'].'"></a></li>';
			if(isset($node->field_pinterest_url['und'][0]['value'])) print '<a class="fa fa-pinterest" target="_blank" href="'.$node->field_pinterest_url['und'][0]['value'].'"></a>';
			if(isset($node->field_google_url['und'][0]['value'])) print '<a class="fa fa-google-plus" target="_blank" href="'.$node->field_google_url['und'][0]['value'].'"></a>';
			if(isset($node->field_dribbble_url['und'][0]['value'])) print '<a class="fa fa-dribbble" target="_blank" href="'.$node->field_dribbble_url['und'][0]['value'].'"></a>';
			if(isset($node->field_vimeo_url['und'][0]['value'])) print '<a class="fa fa-vimeo-square" target="_blank" href="'.$node->field_vimeo_url['und'][0]['value'].'"></a>';
			if(isset($node->field_instagram_url['und'][0]['value'])) print '<a class="fa fa-instagram" target="_blank" href="'.$node->field_instagram_url['und'][0]['value'].'"></a>';
			if(isset($node->field_vk_url['und'][0]['value'])) print '<a class="fa fa-vk" target="_blank" href="'.$node->field_vk_url['und'][0]['value'].'"></a>';
		?>
	</div>
</div>
<img class="slImg" src="<?php print image_style_url('team', $node->field_imeges['und'][0]['uri']); ?>" alt="imgTeam" />