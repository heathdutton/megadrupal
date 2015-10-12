<div class="node-<?php print $node->nid; ?> node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clearfix thing"><div class="node-inner">

  <?php if ($teaser): ?>
    
    <div class="teaser"><div class="teaser-inner clearfix">  
      
      <?php if ($content['field_image']): ?>
        <div class="field-image">
          <?php print render($content['field_image']); ?>
        </div>
      <?php endif; ?>      
      
      <?php print render($title_prefix); ?>
      <?php if (!$page): ?>
        <h2<?php print $title_attributes; ?> class="title">
          <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
        </h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php if ($display_submitted): ?>
        <div class="submitted">
          <?php print $date; ?> by <?php print $name; ?>
        </div>  
      <?php endif; ?>   

      <div class="content"<?php print $content_attributes; ?>>
        <?php
        // We hide the comments and links now so that we can render them later.
    		hide($content['field_tags']);
	    	hide($content['field_image']);
        hide($content['comments']);
        hide($content['links']);
        print render($content);
        ?>
      </div>

      <?php if ($content['links']): ?>
        <div class="links">
          <?php print render($content['links']); ?>
        </div>
      <?php endif; ?>
      
      <?php if ($content['field_tags']): ?>
        <div class="taxonomy">
	      <?php print render($content['field_tags']); ?>
        </div>
	    <?php endif; ?>
      
    </div></div>
    
  <?php endif; ?>
  
  <?php if ($page == 1 || $page == 0 && $teaser == 0): ?>
    
    <div class="full-node clearfix">
    
      <?php print render($title_prefix); ?>
        <h1<?php print $title_attributes; ?> class="title">
          <?php print $title; ?>
        </h1>
      <?php print render($title_suffix); ?>    

      <?php if ($display_submitted): ?>
        <div class="submitted">
          <?php print $date; ?> by <?php print $name; ?>
        </div>  
      <?php endif; ?>
    
      <?php print $user_picture; ?>   

      <div class="content"<?php print $content_attributes; ?>>
         <?php
        // We hide the comments and links now so that we can render them later.
		    hide($content['field_tags']);
        hide($content['comments']);
        hide($content['links']);
        print render($content);
      ?>
      </div>
      
      <?php if ($content['field_tags']): ?>
        <div class="taxonomy">
	      <?php print render($content['field_tags']); ?>
        </div>
	  <?php endif; ?>      

      <?php if ($content['links']): ?>
        <div class="links">
          <?php print render($content['links']); ?>
        </div>
      <?php endif; ?>
    
    </div>
  
      <?php print render($content['comments']); ?>  
  
  <?php endif; ?>
  
</div></div> <!-- /node-inner, /node -->
