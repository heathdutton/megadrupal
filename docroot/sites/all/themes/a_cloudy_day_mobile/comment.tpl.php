<?php // $Id$ ?>
<div class="comment-node-wrapper">
	<div class="comment<?php print ($new) ? ' comment-new' : ''; print ' '. $status; print ' '. $zebra; ?>">
		<div class="clear-block">
	      <span class="submitted"><?php print t('Submitted by !username on !datetime', array('!username' => $author, '!datetime' => $created)); ?></span>
			<?php if ($new) : ?>
	  		<span class="new"><?php print drupal_ucfirst($new) ?></span>
			<?php endif; ?>
	
			<?php if($picture) { ?>
				<div id="userpicture">
					<?php print $picture ?>
				</div>
			<?php } ?>
		
			<div class="commentcontent">
		    <h3<?php print $title_attributes; ?>><?php print $title ?></h3>
		  	<div class="content">
			    <?php
			    hide($content['links']);
			    print render($content);
			    ?>
		      <?php if($signature): ?>
		        <div class="clear-block">
		          <div>—</div>
		      	  <?php print $signature ?>
		    	  </div>
		      <?php endif; ?>
		    </div>
		  </div>
		  <?php if($content['links']): ?>
		    <div class="links"><?php print render($content['links']) ?></div>
		  <?php endif; ?>
	  </div>
	</div>
</div>