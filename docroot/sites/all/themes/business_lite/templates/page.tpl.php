  <div id="wrapper">
    <div id="header">                                         <!--START OF HEADER!-->
      <div class="container-12">
        <div class="grid-4">
          <?php if ($logo): ?>
            <div id="logo">
              <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">          
              <img src="<?php print $logo;?>" alt="<?php print t('Home'); ?>" />
              </a>
            </div>
          <?php endif; ?>
        </div>
        <?php if ($page ['primarynav']): ?>
          <div class="grid-8 clearfix omega">
            <div id="nav">
              <?php // print render ($page ['primarynav']); ?> 
              
            <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
            </div>        
          </div>
        <?php endif; ?>
       </div>
    </div>                                                          <!--END of HEADER!--> 
    <div class="container-12 clearfix" style="margin-top:20px;">    <!--START of CONTENT!--> 
      <div id="leftsection" class="<?php print (($page['sidebar_second']) ? 'grid-8' : 'grid-12') ?>">
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
            <h1 class="title">
              <?php print $title ?>
            </h1>
          <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php print $messages; ?>
          <?php if ($tabs): ?>
            <?php print render($tabs); ?>
          <?php endif; ?>
          <?php if ($page['content']): ?>
            <?php print render($page['content']); ?>
          <?php endif; ?>
          <?php if ($feed_icons): ?>
            <?php print $feed_icons; ?>
          <?php endif; ?>
      </div>
      <?php if ($page['sidebar_second']): ?>
        <div class="grid-4">
          <?php print render($page ['sidebar_second']); ?>
        </div>  
       <?php endif; ?>
   </div>                                                             <!--END of CONTENT!--> 
  
   <div id="footer">                                                  <!--START of FOOTER!-->                   
      <div class="container-12 clearfix">
        <div class="grid-12 clearfix">
          <?php if ($page['footer_firstcolumn']): ?>
            <div class="grid-3 clearfix alpha footer-widget-title">
              <?php print render($page ['footer_firstcolumn']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['footer_secondcolumn']): ?>
            <div class="grid-3 clearfix footer-widget-title">
              <?php print render($page ['footer_secondcolumn']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['footer_thirdcolumn']): ?>
            <div class="grid-3 clearfix footer-widget-title">
              <?php print render($page ['footer_thirdcolumn']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['footer_fourthcolumn']): ?>
            <div class="grid-3 clearfix omega footer-widget-title">
              <?php print render($page ['footer_fourthcolumn']); ?>
            </div>
          <?php endif; ?>
        </div>
       </div>
    </div>                                                            <!--END of FOOTER!-->  
    
    <div id="afterfooter">                                            <!--START of AFTERFOOTER!-->
      <div  class="container-12 clearfix">
        <div class="grid-12 clearfix">      
          <div id="credit" class="grid-6 clearfix">         
          </div>       
          <div class="grid-5 clearfix" style="text-align:right; margin-top:20px;">
             Powered by Drupal<br/>
             Developed by <a href="http://www.zyxware.com/">Zyxware</a>
          </div>
        </div>
      </div>
    </div>                                                               <!--END of AFTERFOOTER!-->
  </div>
