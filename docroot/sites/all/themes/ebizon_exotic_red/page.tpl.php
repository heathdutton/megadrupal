<?php
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
   <head>
<?php global $base_url;?>
   
  </head>
  <body>
      <?php print render($page['page_top']); ?>
    <div id="layout">
   

      <div id="header">
       
        	<div id="logo">
              
        <?php
          // Prepare header
          $site_fields = array();
          if ($site_name) {
            $site_fields[] = check_plain($site_name);
          }
          if ($site_slogan) {
            $site_fields[] = check_plain($site_slogan);
          }
          $site_title = implode(' ', $site_fields);
          $site_fields[0] = '<span>'. $site_fields[0] .'</span>';
          $site_html = implode(' ', $site_fields);

          if ($logo || $site_title) {
            print '<h1><a href="'. check_url($base_path) .'" title="'. $site_title .'">';
            if ($logo) {
              //print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" id="logo" />';
            }
            print $site_html .'</a></h1>';
          }
        ?>
       


 
       

        </div>

          <?php print render($page['header']); ?>
        <hr class="noscreen"/>   
        
        <div id="nav" class="box">
        <ul> <li><?php if (isset($main_menu)) { ?><?php print theme('links', $main_menu, array('class' =>'links', 'id' => 'navlist')) ?><?php } ?></li></ul>
        <hr class="noscreen"/>
      </div> 

        
      </div>
  <?php print render($page['highlighted']); ?>
      <hr class="nosreen"/>

        <div id="container" class="box"> 

                  <div id="panel-left" class="box panel">
                       <div id="sidebar-first" class="in">
                          <?php print render($page['sidebar_first']);?>
        
                       </div>
                  </div>


            
	  <div id="obsah" class="contents box">
	      <?php print $messages;?>
              <?php print $breadcrumb; ?>
          <div class="in">
              
		<?php print render($page['content']);?>
 
          </div>
        </div>
   
       
      
      </div>
     
      <div id="footer">
          <?php print render($page['footer']); ?>
         <span class="f-left">Copyright (c) Themebot.com </span>
        <span class="f-right">All rights reserved.</span>
      </div>
    </div>
<?php print render($page['page_bottom']); ?>
  </body></html>
