<?php
/**
* @file
* This the page.tpl.php template. 
* 
* This template outputs the content between and excluding body tags. 
*
*
* 
*/
?>

<div id="container">
	<div class="left" id="main_left">
		 <div id="header"><?php print $site_name; ?></div>
       
	 
	   
	   
<div class="right" id="main">	
		<div class="padded">
			<?php if ($title): ?>
       <h1><?php print $title; ?></h1>
    <?php endif; ?>
			<?php print render($page['content']) ?>
			</div>
		</div>
		
		
	<div class="left" id="subnav">
		<?php print render($page['left']) ?>
		</div>
	
	
	</div>
	
    <div class="right" id="main_right">

		<div class="padded">
			<?php print render($page['right']) ?>
		</div>
	</div>
			
	
	<div class="clearer">&nbsp;</div>
	
	<div id="footer">
		<span class="left"><?php print render($page['footer_left']) ?> </span>
		<span class="right"><?php print render($page['footer_right']) ?> </span>
		
		<div class="clearer">&nbsp;</div>						
	</div>
	
</div>
