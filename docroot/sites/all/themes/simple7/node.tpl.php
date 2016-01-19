<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php if (!$page): ?>
   
    <h2><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    
  <?php endif; ?>
    
  <em class="submitted"><?php print $date; ?> | <?php print $name; ?></em>
  
  <article <?php print $content_attributes ?>>
  
  	<?php print render($content) ?>
  	
  </article>

</div>
