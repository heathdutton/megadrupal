
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
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
        <hr class="noscreen" />   
          
              
        <div id="navigation">
        <ul>
          <li><?php if (isset($main_menu)) { ?><?php print theme('links', $main_menu, array('class' =>'links', 'id' => 'navlist')) ?><?php } ?></li>
        </ul>
        </div>
        
        <hr class="noscreen" />  
    
      </div>
        <!--Add Highlighted Region */ -->
        <div id="highlight">
            <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
        </div>
        <div class="rbroundbox">
	         <div class="rbtop"><div>&nbsp;</div></div>
		          <div class="rbcontent">
          			
          		<div class="contents">
                          <?php print $messages; ?>
	  
	  <?php print  $breadcrumb; ?>
              </div>
             
          			
        		  </div>
	         <div class="rbbot"><div>&nbsp;</div></div>
        </div>
          
	<div id="main">
              <div class="mainpage">         
         <?php print render($page['content']);?>
	       </div>
              <div class="rightblock">
              <?php print render($page['sidebar_second']);?>
              </div>
        </div>
        </div>
       <?php if ($page['footer']): ?>
        <div id="footer">
        <div id="footer-inside">
            <?php print render($page['footer']); ?>
        <p id="copyright">Copyright (c) 2008 Themebot.com.</p>
        
        <!-- Please don't delete this. You can use this template for free and this is the only way that you can say thanks to me -->
          <!--<p id="dont-delete-this">Design by <a href="http://www.davidkohout.cz">David Kohout</a> | Our tip: <a href="http://www.goodmood.cz" title="Absinthe, Becherovka, Slivovitz store">Absinthe Store</a></p>-->
	<!-- Thank you :) -->
	</div><div style="clear: both;"></div></div><?php endif; ?>
       <?php print render($page['page_bottom']); ?>
  </body>
</html>
