<div id="wrapper" class="container-12">
      <div class="header">
        <div class="grid-12">
          <div class="grid-4 alpha logo">
            <?php if ($site_name): ?>
                  <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
                    <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" />
                  </a>
            <?php endif; ?>
          </div>
          <?php if ($page ['login']): ?>
          <div class="grid-8 omega">
            <?php print render($page['login']); ?>
          </div>
          <?php endif; ?>
        </div>
        <?php if ($banner):  ?>                                        <!-- SLIDESHOW STARTS HERE -->
          <div id="slider" class="grid-12">
            <?php print $banner; ?>
          </div>
        <?php endif; ?>                                                <!-- SLIDESHOW ENDS HERE --> 
        
        <!--<?php if ($page ['slideshow']): ?>
          <div class="slider">
            <?php print render($page['slideshow']); ?>
          </div>
          <?php endif; ?>-->
      </div>
     <div class="grid-12  main_nav">
        <?php //if ($page ['primarynav']): ?>
        <div class="grid-8 alpha omega">
          <!--<div id="nav">-->
            <?php //print render ($page ['primarynav']); ?>
            <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
          <!--</div>-->  
        </div>
        <?php //endif; ?>
        <?php if ($page ['search']): ?>
        <div class="grid-3">
          <?php print render($page['search']); ?>
        </div>
        <?php endif; ?>
      </div>
      <div class="grid-12">
      <div class="content_outer">
        <?php if ($page ['boxone']): ?>
        <div class="grid-3 alpha omega">
          <?php print render($page['boxone']); ?>
        </div>
        <?php endif; ?>
        <?php if ($page ['boxtwo']): ?>
        <div class="grid-3 alpha omega">
          <?php print render($page['boxtwo']); ?>
        </div>
        <?php endif; ?>
        <?php if ($page ['boxthree']): ?>
        <div class="grid-3 alpha omega">
          <?php print render($page['boxthree']); ?>
        </div>
        <?php endif; ?>
        <?php if ($page ['boxfour']): ?>
        <div class="grid-3 alpha omega">
          <?php print render($page['boxfour']); ?>
        </div>
        <?php endif; ?> 
        </div>       
      </div>
      <div class="grid-12 article_outer">
        
          <?php if ($page ['content']): ?>
          <div class="article_leftwrapper">
            <?php print render($page['content']); ?>
            </div>
          <?php endif; ?>
        
      </div>
      <div class="grid-12 footer_top">
        <?php if ($page ['f-left']): ?>
        <div class="grid-3 leftmargin">
          <?php print render($page['f-left']); ?>
        </div>
        <?php endif; ?>
        <div class="grid-3">
        <?php if ($page ['f-middle']): ?>
        
          <div class="middle_top"></div>
          <div class="middle_middle"></div>
          <div class="middle_bottom"></div>
          <?php print render($page['f-middle']); ?>
        
        <?php endif; ?>
        </div>
        <div class="grid-5">
        <?php if ($page ['f-right']): ?>
          <?php print render($page['f-right']); ?>
        <?php endif; ?>       
        </div>
        <div class="footer_zyxware">Theme by <a href="http://www.zyxware.com/" title="Zyxware" target="_blank">Zyxware</a></div> 
      </div>
      <div class="grid-12 footer_bottom">
        <?php if ($page ['copyright']): ?>
          <?php print render($page['copyright']); ?>
        <?php endif; ?> 
      </div> 
    </div>
