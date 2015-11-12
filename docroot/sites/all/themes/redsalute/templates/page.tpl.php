<?php
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
    <head profile="http://gmpg.org/xfn/11">
    <?php print render($page['header']); ?>
        <?php
        global $base_path;
        global $theme_path;
        ?>
        <!--[if IE 6]>
        <?php
        print '<link rel="stylesheet" href="'. $base_path . $theme_path .'css/template.ie6.css" type="text/css" media="screen" />';
        ?>
        <![endif]-->
        <!--[if IE 7]>
        <?php
        print '<link rel="stylesheet" href="'. $base_path . $theme_path .'css/template.ie7.css" type="text/css" media="screen" />';
        ?>
        <![endif]-->
    </head>
    <body>
        <div class="PageBackgroundGradient"></div>
        <div class="PageBackgroundGlare">
            <div class="PageBackgroundGlareImage"></div>
        </div>
        <div class="Main">
            <div class="Sheet">
                <div class="Sheet-tl"></div>
                <div class="Sheet-tr"></div>
                <div class="Sheet-bl"></div>
                <div class="Sheet-br"></div>
                <div class="Sheet-tc"></div>
                <div class="Sheet-bc"></div>
                <div class="Sheet-cl"></div>
                <div class="Sheet-cr"></div>
                <div class="Sheet-cc"></div>
                <div class="Sheet-body">
                    <div class="Header">
                        <div class="Header-png"></div>
                        <div class="Header-jpeg"></div>
                        <div class="logo">
                            <?php
                            global $base_path;
                            print '<h1 id="name-text" class="logo-name"><a href="'. $base_path .'">';
                            print $site_name;
                            print '</a></h1>';
                            if ($site_slogan) {
                                print '<div id="slogan-text" class="logo-text">'. $site_slogan .'</div>';
                            }
                            ?>
                        </div>
                        <div class="banner">
                          <?php if ($page['banner']): ?>
                            <?php print render($page['banner']); ?>
                          <?php endif; ?>
                        </div>
                    </div>
                    <div id="nav">
                        <div class="l"></div>
                        <div class="r"></div>
                        <?/*php if (isset($main_menu)) { ?>
		                    <?php print theme('links__system_main_menu', array(
		                                      'links' => $main_menu,
		                                      'attributes' => array(
		                                      'id' => 'front_menu',
			                                    'class' => array('artmenu'),
		                                        ),
		                                      'heading' => array(
		                                      'text' => t('Main menu'),
		                                      'level' => 'h2',
		                                      'class' => array('element-invisible'),
		                                        ),
		                                      )); ?>
		                  <?php }*/?>
		                  
		                   <?php if ($page ['primary_nav']): ?>
		                      <?/*php print $main_menu; */?>
		                      <?php 
          if (module_exists('i18n')) {
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
            
          }
          print drupal_render($main_menu_tree);
        ?>
                      <?php endif; ?>
		                  <?php if ($page['primary_nav']): ?>
		                      <?/*php print render($page['primary_nav']); */?>
                      <?php endif; ?>

                    </div>
                    <div class="content-top">
                      <?php if ($page['content_top']): ?>
                        <?php print render($page['content_top']); ?>
                      <?php endif; ?>
                    </div> 
                    <div class="contentLayout">
                        <div class="<?php print (($page['sidebar_second']) ? 'contents' : 'contentfull') ?>">
                            <?php if (isset($breadcrumb)) { ?>
                            <div class="Post">
                                <div class="Post-body">
                                    <div class="Post-inner">
                                        <div class="PostContent">
                                            <?php print $breadcrumb; ?>
                                        </div>
                                        <div class="clearfix"></div>
                                    </div>
                                </div>
                            </div>
                            <?php } ?>
                            <?//php if ($show_messages && $messages): print $messages; endif; ?>
                            <?php print render($title_prefix); ?>
                            <?php if ($title): ?>
                              <div class="Post">
                                <div class="Post-body">
                                  <div class="Post-inner">
                            <?php endif; ?>
                            <?php if ($tabs): ?>
                              <div id="tabs-wrapper" class="clear-block">
                            <?php endif; ?>
                            <?php if ($title):?>
                                                            <h2 class="PostHeaderIcon-wrapper">
                    <img width="26" height="26" alt="PostHeaderIcon" src="<?php print base_path( ) ?><?php print  $theme_path ?>/images/PostHeaderIcon.png">
                    <a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a>
                </h2>
                            <?php endif; ?>
                            <?php if ($tabs):?>
                            <?php print render($tabs) ;?>
                            </div>
                            <?php endif; ?>
                            <?/*php if ($tabs2): print '<ul class="tabs secondary">'. render($tabs) .'</ul>'; endif; */?>
                            <?php if ($page['content']): ?>
                              <?php print render($page['content']); ?>
                            <?php endif; ?>
                            <?php if ($title):?>
                                  </div>
                                </div>
                             </div>
                            <div class="clearfix"></div>
                            <?php endif; ?>
                        </div>
                        <?php if ($page['sidebar_second']): ?>
                          <div class="sidebar1">
                            <?php print render($page['sidebar_second']); ?>
                          </div>
                        <?php endif; ?>
                    </div>                    
                    <div class="content-bottom">
                      <?php if ($page['content_bottom']): ?>
                        <?php print render($page['content_bottom']); ?>
                      <?php endif; ?>
                    </div>                      
                    <div class="clearfix"></div>
                    <div class="Footer">
                        <div class="Footer-inner">
                            <?php
                            print '<a title="Feed Entries" class="rss-tag-icon" href="'. url('rss.xml') .'">';
                            print '<img alt="feed-image" src="'. $base_path . $theme_path .'/images/livemarks.png" />';
                            print '</a>';
                            ?>
                            <div class="Footer-text">
                                <?php print render($page['footer']); ?>
                            </div>
                        </div>
                        <div class="Footer-background"></div>
                    </div>
                </div>
            </div>
            <div class="clearfix"></div>
            <p class="page-footer">
                Design by
                <a href="http://www.ablewebpro.com/">
                    ablewebpro
                </a>
                | Theme by
                <a href="http://www.zyxware.com">
                    Zyxware
                </a>
            </p>
        </div>
    </body>
</html>
