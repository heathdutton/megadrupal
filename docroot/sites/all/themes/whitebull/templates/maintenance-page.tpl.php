<?php 
 ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes; ?>>


    <div id="page-wrapper">
    
        <div id="header-wrapper">
            <div id="header">
                    
                <div id="authorize-and-feed">
                    
                    <div id="authorize">
                        <ul>
                            <?php global $user;
                                if ($user->uid != 0):
                                    print '<li class="first">' . t('logged in as ') . ' <a href="' . url('user/' . $user->uid) . '">' . $user->name . '</a></li>';
                                    print '<li class="last"> <a href="' . url('user/logout') . '">' . t('logout') . '</a></li>';
                                else:
                                    print '<li class="first">' . t('login is ') . ' <a href="' . url('user') . '">' . t('here') . '</a></li>';
                                    print '<li class="last"> <a href="' . url('user/register') . '">' . t('register') . '</a></li>';
                                endif;
                            ?>                
                        </ul>
                    </div><!--end of authorize-->
                    
                    <?php if ($feed_icons): ?>
                        <div class="feed-wrapper">
                            <?php print $feed_icons; ?>
                        </div>
                    <?php endif; ?><!--end of rss feed-->
                
                </div><!--end of authorize and feed icon-->
                
                <div style="clear:both"></div>
                
                <div id="menu-and-search">
                    
                    <div id="menu-left"></div>

                    <div class="menu-wrapper">
                                    
                                    <div id="home-link" style="float:left;"> <!--this is a home link with an image, you can disable it with delete this or change the image in images directory. just enjoy it:D -->
                                        <a href="<?php print $base_path  ?>" title="<?php print t('home') ?>" > <img src="<?php print $base_path . $directory; ?>/images/home.png" width="24" height="31" alt="<?php print t('home') ?>" /> </a>
                                    </div>
                                                                
                    </div>
                    
                    <div id="menu-right"></div>
                    
                </div><!--end of menu and search-box-->
                
                <div id="branding-wrapper">
                    
                    
                    <div id="branding">
                        
                            <?php if ($logo): ?>
                                <div class="logo"> 
                                    <a href="<?php print $base_path  ?>" title="<?php print t('home') ?>" > <img src="<?php print $logo  ?>" alt="<?php print t('home') ?>" /> </a>
                                </div>
                            <?php endif; ?><!--this is logo-->
                        
                        <div id="name-slogan-wrapper">
                            
                            <?php if ($site_name): ?>
                                <h1 class="site-name"><a href="<?php print $base_path; ?>" title="<?php print $site_name  ?>" ><?php print $site_name ?></a></h1>
                            <?php endif; ?>
                            
                            <?php if ($site_slogan): ?>
                                <span class="site-slogan"><?php print $site_slogan; ?></span>
                            <?php endif; ?>
                            
                        </div>    
                
                    </div><!--end of branding wrapper-->
                    
                </div><!--end of branding-->    
                
            </div>
        </div><!--end of header-->
        
        <div id="container-wrapper">
            
            <div id="container-outer">    
                    
                <div class="top-shadow"></div>        
                <div class="middle-shadow"><!--this is a background of container,consist of top-shadow middle-shadow and bottom-shadow, i create it for two other regions, in bottom-teaser and bottom-wrapper, so it's all connected, just edit this and two other will automatically changed, enjoy it:D-->
                <div class="middle-fix"><!--this is to fix container of middle shadow-->
                    <div id="main-content">
                        
                        <?php if ($show_messages): print $messages; endif;?>
                        <?php if ($content): ?><div class="mission"><?php print $content; ?></div><?php endif; ?>
                        
                    </div><!--this is main content, you can edit this if you want to change arange of main content-->
                </div><!--end of middle fix-->
                    <div style="clear:both"></div>
                </div><!--end of middle shadow-->
                <div class="bottom-shadow"></div>
                    
            </div>    

        </div><!--end of conteiner wrapper-->        
        <div id="footer">    
            <div id="notice">
                <div class="notice" style="font-size:11px;margin-bottom:3px;font-style: italic;line-height:1em;"><p>Theme by <a href="http://www.igoen.com">Gunawan Probo Swandono</a>.</p></div>
            </div><!--this is a notice,the banner of me, Gunawan Probo Swandono (a.k.a igoen)-->        
            <div style="clear:both"></div>
        </div><!--end of footer-->

    </div>
