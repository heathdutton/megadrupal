<?php
/**
 * @file
 * The theme system, which controls the output of Drupal.
 *
 * The theme system allows for nearly all output of the Drupal system to be
 * customized by user themes.
 */

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head profile="http://gmpg.org/xfn/11">
        
        <!--[if IE]>
        <link rel="stylesheet" href="<?php $base_path; ?>sites/all/themes/icandy/style-ie.css" type="text/css" media="screen" />
        <![endif]-->
        <?php global $theme_path ; ?> 
        <style type="text/css">
            body { background-color: #111111; }
            /* two column categories */
            #sidebar ul li .cat-item { float: left; width: 43%; }
            #sidebar ul ul .cat-item .children { display: none; }
            #sidebar { width: 270px; }
            #container { width: 590px; }
            
            #banner {
              background: url(<?php print $base_path . $theme_path .'/images/header1.png'?>) bottom no-repeat;
              height: 180px;
              width: 900px;
            }
            #banner h1 { color:#eee; }
            #banner h1 a { color:#eee; }
            #banner #description { color:#eee; }
        </style>
        <script type="text/javascript">
            ddsmoothmenu.init({
                mainmenuid: "smoothmenu1", //menu DIV id
                orientation: 'h', //Horizontal or vertical menu: Set to "h" or "v"
                classname: 'ddsmoothmenu', //class added to menu's outer DIV
                contentsource: "markup" //"markup" or ["container_id", "path_to_menu_file"]
            })
      </script>
    </head>
    <body<?php //print phptemplate_body_class($left, $right); ?>>
        <div id="wrapper">
            <!--<div id="smoothmenu1" class="ddsmoothmenu">
                <?php // print $breadcrumb; ?>
                  <?php 
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
              ?>
               <div class="clear"></div>
            </div>-->
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
            <div id="banner">
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
                  if ($site_fields) {
                    $site_fields[0] = '<span>'. $site_fields[0] .'</span>';
                  }
                  $site_html = implode(' ', $site_fields);

                  if ($logo || $site_title) {
                    print '<h1><a href="'. check_url($front_page) .'" title="'. $site_title .'">';
                    if ($logo) {
                      //print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" id="logo" />';
                    }
                    print $site_html .'</a></h1>';
                  }
                    ?>
            </div>
            <div id="searchpanel">
                <div class="feed">
                    <?php
                    print '<a href="'. url('rss.xml') .'" target="_blank">';
                    ?>
                        <span>Feed</span>
                    </a>
                </div>
                <div class="search">
                
                    <?php if ($page['search_box']): ?><div class="block block-theme"><?php print render($page['search_box']); ?></div><?php endif; ?>
                 </div>
                <div class="clear"></div>
            </div>
            <div id="container">
              <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
              <?php if ($title):print '<div class="with-tabs"><h2'. ($tabs ? ' class="with-tabs"' : ' class="with-tabs"') .'>'. $title .'</h2></div>'; endif; ?>
              <?php if ($tabs): print render( $tabs ).'</div>'; endif; ?>
              <?php print render($page['content']); ?>
              <?php if ($show_messages && $messages): print $messages; endif; ?>
              <?php // print $help; ?>
              <?php // print $content ?>
              <?php print $feed_icons ?>
            </div>
            <div id="sidebar">
                <?php print render ($page['sidebar_first']); ?>
            </div>
            <div class="clear"></div>
            </div>
            <div id="footer">
              <div class="footer-wrapper">
                <div class="footer-content">
                <?php if ($page['footer']): ?>
                  <?php print render($page['footer']); ?>
                <?php endif;?>
                <div class="footer_zyxware">Theme by <?php print l('Zyxware Technologies','http://www.zyxware.com');?></div>            
                </div>
              </div><!--closing of footer wrapper-->
            </div>
        </div>
    <?php // print $closure ?>
    </body>
</html>
