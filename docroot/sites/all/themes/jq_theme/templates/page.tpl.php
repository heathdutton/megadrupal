<?php
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head profile="http://gmpg.org/xfn/11">
        <?php // print $head ?>
        <title><?php // print $head_title ?></title>
        <?php // print $styles ?>
        <?php // print $scripts ?>
        <!--[if IE]>
        <style type="text/css">
            div.preview {margin-left:440px;margin-top:0px;margin-right:0px;margin-bottom:0px;}
            .comment-link {background:none;}
            #search-submit {margin: 10px 0 0 0; height: 28px;}
        </style>
        <![endif]-->
    </head>
    <body>
        <div id="outline">
            <div id="blog-line">
                <h1>
                    <?php
                        global $base_path;
                        print '<a href="'. $base_path .'">';
                        print $site_name;
                        print '</a>';
                        print '|'. $site_slogan;
                    ?>
                </h1>
            </div>
            <div id="nav" class="clearfix">
                <?/*php print theme('links__system_main_menu', array(
                          'links' => $main_menu,
                            'attributes' => array(
                            'id' => 'menu-primary-items',
                            'class' => array('links','inline', 'sf-js-enabled'),
                            ),
                            'heading' => array(
                            'text' => t('Main menu'),
                            'level' => 'h2',
                            'class' => array('element-invisible'),
                            ),
                          )); 
                */?>
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
            <div id="content" class="clearfix">
              <?php if (!empty($page['sidebar_first'])): ?>
                <div id="right">
                    <p id="sidebar_hide">
                        <a id="hide_s" href="#">
                            Sidebar →
                        </a>
                    </p>
                    <p id="font-resize">
                        <a id="default" href="#">
                            A
                        </a>
                        <a id="larger" href="#">
                            A+
                        </a>
                        <a id="largest" href="#">
                            A++
                        </a>
                    </p>
                   <!--<div class="app_widget">-->
                      <?php print render ($page['sidebar_first']); ?>
                    <!--</div>-->
                </div>
              <?php endif;?>
                <div id="left">
                    <p id="sidebar_show" style="display: none;">
                        <a id="show_s" href="#">
                            ← Sidebar
                        </a>
                    </p>
                    <?php
                    if ($is_front) { ?>
                        <div id="slideshow">
                            <?php print $slide_show; ?>
                        </div>
                    <?php } ?>
                    <?php // if ($mission): print '<div id="mission">'. $mission .'</div>'; endif; ?>
                    <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
                    <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
                    <?php if ($tabs): print render ($tabs) . '</div>'; endif; ?>
                    <?php // print $help; ?>
                    <?php print render($page['content']); ?>
                    <?php print $feed_icons ?>
                </div>
            </div>
            <div id="appendix" class="clearfix">
                <div class="footer_message">
                   <?php if ($page['footer']): ?>
                    <?php print render($page['footer']); ?>
                  <?php endif;?>
                </div>
                <div class="footer_bottom">
                    <div class="footer_js">
                        <a href="#" id="totop">To top</a>
                    </div>
                    <div class="attribution">
                        Design by
                        <a href="http://devolux.nh2.me">
                            devolux.nh2.me
                        </a>
                        | Theme by
                        <a href="http://www.zyxware.com">
                            Zyxware
                        </a>
                    </div>
                </div>
            </div>
        </div>
    </body>
</html>
