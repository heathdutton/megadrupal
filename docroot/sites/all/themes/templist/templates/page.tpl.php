<?php
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head profile="http://gmpg.org/xfn/11">
        <?php print render($page['header']); ?>
        
        <?php
        global $base_path;
        global $theme_path;
        print '<!--[if IE 6]><link rel="stylesheet" href="'. $base_path . $theme_path .'/css/template.ie6.css" type="text/css" media="screen" /><![endif]-->';
        print '<!--[if IE 7]><link rel="stylesheet" href="'. $base_path . $theme_path .'/css/template.ie7.css" type="text/css" media="screen" /><![endif]-->';
        ?>
    </head>
    <body>
        <div id="art-main">
            <div class="art-Sheet">
                <div class="art-Sheet-tl"></div>
                <div class="art-Sheet-tr"></div>
                <div class="art-Sheet-bl"></div>
                <div class="art-Sheet-br"></div>
                <div class="art-Sheet-tc"></div>
                <div class="art-Sheet-bc"></div>
                <div class="art-Sheet-cl"></div>
                <div class="art-Sheet-cr"></div>
                <div class="art-Sheet-cc"></div>
                <div class="art-Sheet-body">
                    <div class="art-Header">
                        <div class="art-Header-png"></div>
                        <div class="art-Header-jpeg"></div>
                        <div class="art-Logo">
                            <?php
                                global $base_path;
                                print '<h1 id="logo" class="art-Logo-name"><a href="'. $base_path .'">';
                                print $site_name;
                                print '</a></h1>';
                                if ($site_slogan) {
                                    print '<div class="description">'. $site_slogan .'</div>';
                                }
                            ?>
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
			                                    'class' => array('art-menu'),
		                                        ),
		                                      'heading' => array(
		                                      'text' => t('Main menu'),
		                                      'level' => 'h2',
		                                      'class' => array('element-invisible'),
		                                        ),
		                                      )); ?>
		                  <?php } */?>   
		                  
		                  <?php if ($page ['primary_nav']): ?>
		                    <?php // print $main_menu; ?>
		                    <?php 
                          if (module_exists('i18n')) {
                            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
                          } 
                          else {
                            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
                          }
                          print drupal_render($main_menu_tree);
                        ?>
                      <?php endif; ?>
                        
                    </div>
                    <div class="art-contentLayout">
                        <?php if ($page['sidebar_first']): ?>
                          <div class="art-sidebar1">
                            <?php print render($page['sidebar_first']); ?>
                          </div>
                        <?php endif; ?>
                        <div class="<?php print (($page['sidebar_first']) ? 'art-content' : 'art-contentfull') ?>">
                            <?php if(isset($breadcrumb)) { ?>
                            <div class="art-Post">
                                <div class="art-Post-body">
                                    <div class="art-Post-inner">
                                        <div class="art-PostContent">
                                            <?php print $breadcrumb; ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <?php } ?>
                            <div class="art-Post">
                                <div class="art-Post-body">
                                    <div class="art-Post-inner">
                                        <div id="mission"> </div>
                                        <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
                                        <?php if ($title):
                                            print '<div class="art-PostMetadataHeader">
                                            <h2'. ($tabs ? ' class="art-PostHeader with-tabs"' : ' class="art-PostHeader"') .'>'. $title .'</h2>
                                            </div>'; endif; ?>
                                        <?php if ($tabs): print render( $tabs ).'</div>'; endif; ?>

                                        <?php if ($show_messages && $messages): print $messages; endif; ?>
                                        
                                        <?php if ($page['content']): ?>
                                          <div class="art-PostContent">
                                            <?php print render($page['content']); ?>
                                          </div>
                                        <?php endif; ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="clearfix"></div>
                    <div class="art-Footer">
                        <div class="art-Footer-inner">
                            <?php
                            /*print '<a title="Feed Entries" class="art-rss-tag-icon" href="'. url('rss.xml') .'">';
                            print '<img alt="feed-image" src="'. $base_path . $theme_path .'/images/livemarks.png" />';
                            print '</a>';*/
                            print '<div class="art-rss-tag-icon" title="Feed Entries">';
                            print $feed_icons;
                            print '</div>';
                            ?>
                            <?php if ($page['footer']): ?>
                              <div class="art-Footer-text">
                                <?php print render($page['footer']); ?>
                              </div>
                            <?php endif; ?>
                        </div>
                        <div class="art-Footer-background"></div>
                    </div>
                    <div class="cleared"></div>
                </div>
            </div>
            <p class="art-page-footer">
                Design by
                <a href="http://www.ablewebpro.com">Ablewebpro</a>
                | Theme by
                <a href="http://www.zyxware.com">Zyxware</a>
            </p>
        </div>
        <div class="cleared"></div>
        <?php print render($page['closure']); ?>
    </body>
</html>
