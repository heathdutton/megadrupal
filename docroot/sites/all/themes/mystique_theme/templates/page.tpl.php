<?php 
/**
 * Main template file
 */

print render($page['page_top']); ?>
    <div id="page">
      <div class="page-content header-wrapper">
        <div id="header" class="bubbleTrigger">
          <div id="site-title" class="clearfix">
            <?php
            global $base_path;
            print '<h1 id="logo"><a href="'. $base_path .'">';
            print $site_name;
            print '</a></h1>';
            if ($site_slogan) {
              print '<p class="headline">'. $site_slogan .'</p>';
            }
            if ($logo) {
              print '<div class="logo"><a href="'. $base_path .'"><img src="'. check_url($logo) .'" alt="'. $site_name .'" /></a></div>';
            }
            ?>
             <div class="top-right">
              <?php if ($page['top_right']): print render($page['top_right']); endif; ?>
            </div> 
          </div>
          <div class="shadow-left">
            <div class="shadow-right clearfix">
              <p class="nav-extra">
                <?php
                print '<a href="'. url('rss.xml') .'"  class="nav-extra rss" title="RSS Feeds">';
                ?>
                  <span>
                    RSS Feeds
                  </span>
                </a>
                <a href="http://www.twitter.com/drupal" class="nav-extra twitter" title="Follow me on Twitter!">
                  <span>
                    Follow me on Twitter!
                  </span>
                </a>
              </p>
              <!--<?php 
                if($main_menu) {
                  print theme('links__system_main_menu', array(
		          'links' => $main_menu,
		          'attributes' => array(
		          'class' => array('navigation', 'links', 'clearfix'),
		          ),
		          'heading' => array(
		            'text' => t('Main menu'),
		            'level' => 'h2',
		            'class' => array('element-invisible'),
		          ),
		        ));
                }
                else {
                  print '<ul class="navigation">
                        <li>
                          <a class="fadeThis" href="'. $base_path .'">
                            <span class="title">Home</span>
                            <span class="pointer"></span>
                            <span style="opacity: 0;" class="hover">
                            </span>
                          </a> 
                        </li>
                      </ul>';
                }
              ?>-->
              <div id="nav" >
              <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
              <?php //if ($page ['primarynav']): ?>
                <!--<div class="primarynav prefix-1 grid-7">
                  <?print render($page['primarynav']); ?> 
                </div>-->
              <?php //endif; ?>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div class="shadow-left page-content main-wrapper">
        <div class="shadow-right">
          <div id="main">
                    <div class="top-main">
          
             <div class="top-main-left">
               <?php if ($page['top_main_left']): print render($page['top_main_left']); endif; ?>
             </div>
             <div class="top-main-right">
              <?php if ($search_box): ?><?php print $search_box; ?><?php endif; ?> 
             </div>          
         
          </div>
            <div id="main-inside" class="clearfix">
              <div id="primary-content">
                <div class="blocks">
                  
                  <?php if ($messages): ?>
                    <div id="messages">
                      <?php print $messages; ?>
                    </div>
                  <?php endif; ?>                 
                   <?php if ($title): ?>
                    <h2 class="title" id="page-title">
                      <?php print $title; ?>
                    </h2>
                  <?php endif; ?>
  
                  <?php if (!empty($tabs)): ?>
                   <div id="tabs-wrapper" class="tabs-wrap clear-block">
                    	<?php print render($tabs); ?>
                    </div>
                  <?php endif; ?>
                  <?php print render($page['content']); ?>
                 
                </div>
              </div>
              <?php if ($page['sidebar_second']): ?>
                <div id="sidebar2">
                  <div class="sidebars">
                    <?php print render($page['sidebar_second']); ?>
                  </div>
                </div>
              <?php endif ;?>
              <?php if ($page['sidebar_first']): ?>
                <div id="sidebar">
                  <div class="sidebars">
                    <?php print render($page['sidebar_first']); ?>
                  </div>
                </div>
             <?php endif; ?> 
            </div>
          </div>
          <div id="footer">
            <div class="page-content">
              <div id="copyright">
                <div class="footer_content">
                  <?php print render($page['footer']); ?>
                </div>
                <div class="attribution">
                  Design by
                  <a href="http://digitalnature.ro">
                    digitalnature
                  </a>
                  | Theme by
                  <a href="http://www.zyxware.com">
                    Zyxware
                  </a>
                </div>
                <div class="footer_icon">
                  <!--<div class="style_rss_feed">-->
                <?php
                  print '<a title="RSS Feeds" href="'. url('rss.xml') .'" class="rss-subscribe">';
                ?>
                    RSS Feeds
                  </a>
                  <!--</div>
                  <div class="style_xml">-->
                  <a title="Valid XHTML" href="http://validator.w3.org/check?uri=referer" class="valid-xhtml">
                    XHTML 1.1
                  </a>
                  <!--</div>
                  <div class="style_gotop">-->
                  <a class="js-link" id="goTop">
                    Top
                  </a>
                  <!--</div>-->
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div id="pageControls">
      </div>
    </div>
    <?php print render($page['page_bottom']); ?>
