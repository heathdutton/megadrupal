<div id="wrap">

  <div id="top"></div>
  
  <div id="content">
  
    <div class="header">
      <?php print render($page['header']) ?>
      <h1><a href="<?php print $base_path ?>"><?php print $site_name ?></a></h1>
      <h2><?php print $site_slogan ?></h2>
    </div>
    
    <div class="nav">
  		<?php if ($main_menu): ?>
    	  <?php print theme('links__system_main_menu', array('links' => $main_menu )); ?>
  	  <?php endif; ?>
    </div>
    
    <div class="middle">
      <?php if($tabs): ?><?php print render($tabs) ?><?php endif ?>
      <?php if($title): ?><h2><?php print $title ?></h2><?php endif ?>
      <?php print render($page['help']) ?>
      <?php if($messages): ?><?php print $messages ?><?php endif ?>
      <?php print render($page['content']) ?>
    </div>
    		
    <div class="right">
    		
      <?php print render($page['sidebar']) ?>
    		
    </div>
    
    <div id="clear"></div>
    
  </div>
  
  <div id="bottom"></div>

</div>

<div id="footer">
  <?php print render($page['footer']) ?>
  <p>Design by <a href="http://www.minimalistic-design.net">Minimalistic Design</a></p>
</div>