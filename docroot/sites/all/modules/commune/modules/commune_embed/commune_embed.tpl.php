<?php 
	//to use embedly OR embed directly?
	if($use_embedly) :
?>
<div class="commune-embed-embedly">
	<a class="embed-link" href="<?php print $embed->source_url; ?>"><?php print $embed->source_url; ?></a>
</div>
<?php 
	else:
?>
	<?php
	  $width = $embed->width? $embed->width:"100%";
	  $height = $embed->height? $embed->height:"100%";
	  $ar = $embed->height? $embed->width/$embed->height: 1.0;
	  $ar_mod = ( abs($ar-(4/3)) < abs($ar-(16/9)) ? 'embed-responsive-4by3' : 'embed-responsive-16by9');
	?>
	<!--- rich embed type -->
	<?php if($embed->type == 'rich') : ?>
		<div class="embed-responsive <?php print $ar_mod?>" 
				 style="height:auto">
				<?php print $embed->html ?>
		</div>
	<?php elseif($embed->type == 'video') :  ?>
		<?php
			 $html = preg_replace( '/(width|height)="\d*"\s/', "", $embed->html );
		?>
		<div class="embed-responsive <?php print $ar_mod?>" 
				 style="padding-bottom:<?php print (1/$ar)*100?>%">
				<?php print $html ?>
		</div>
	<?php elseif($embed->type == 'photo') :  ?>
		<div class="embed-responsive <?php print $ar_mod?>" 
				 style="padding-bottom:<?php print (1/$ar)*100?>%">
				<img src="<?php print $embed->source_url; ?>" />
		</div>
	<?php else: ?>
		<div class="commune-embed">
			<div class="left thumbnail">
				<img src="<?php print $embed->thumbnail_url; ?>" />
			</div>
			<div class="right">
				 <div class="title">
						<a href="<?php print $embed->source_url?>"><?php print $embed->title?></a>
				 </div>
				 <div class="description">
						<?php if(isset($embed->description)) print $embed->description; ?>
				 </div>
			</div>
		</div>
	<?php endif; ?>
<?php endif; ?>

