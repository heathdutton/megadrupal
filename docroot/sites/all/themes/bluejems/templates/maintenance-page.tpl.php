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
    <div id="main-body">
    <div id="background-top"style="height: 33px; margin-bottom: -5px;"></div>
    <div id="background">
        <div id="header-wrapper">
            <div id="header">
                
                <?php if($is_front): ?> 
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
                <div style="clear:both"></div>
                </div>
                
                <div id="branding-wrapper">
                    
                    <?php if($is_front): ?>
                    
                    <div id="branding">
                        
                            <?php if ($logo): ?>
                                <div class="logo"> 
                                    <a href="<?php print $base_path ?>" title="<?php print t('home') ?>" > <img src="<?php print $logo ?>" alt="<?php print t('home') ?>" /> </a>
                                </div>
                            <?php endif; ?><!--this is logo-->
                        
                        <div id="name-slogan-wrapper">
                            
                            <?php if ($site_name): ?>
                                <h1 class="site-name"><a href="<?php print $base_path; ?>" title="<?php print $site_name ?>" ><?php print $site_name ?></a></h1>
                            <?php endif; ?>
                            
                            <?php if ($site_slogan): ?>
                                <span class="site-slogan"><?php print $site_slogan; ?></span>
                            <?php endif; ?>
                            
                        </div>    
                
                    </div><!--end of branding wrapper-->
                    <?php endif; ?>
                    
                </div><!--end of branding-->    
                <?php endif; ?>
                <div id="menu-and-search">
                    <div class="menu-wrapper">

                    </div>
                </div><!--end of menu and search-box-->
                    
                
            </div>
        </div><!--end of header-->
   
        <div id="container">
        <div id="container-wrapper">
            <div id="container-outer">
                    
                    <div id="main-content">
                        <?php if ($show_messages): print $messages; endif;?>
                        <?php if ($content): ?><div class="mission"><?php print $content; ?></div><?php endif; ?>
                    </div><!--this is main content, you can edit this if you want to change arange of main content-->
            </div>    
            
            <div style="clear:both"></div>
        </div><!--end of container wrapper-->        

        <div style="clear:both"></div>
        
        <div id="footer">
            <div style="clear:both"></div>
            <div id="notice">
                <div class="notice" style="font-size:11px;text-align: center;margin-left: 2px;"><p>Theme by <a href="http://www.igoen.com">Gunawan Probo Swandono</a>.</p></div>
            </div><!--this is a notice,the banner of me, Gunawan Probo Swandono (a.k.a igoen)-->
            
            <div style="clear:both"></div>
            
        </div><!--end of footer-->
    </div>
    </div>
    </div>
    <div id="background-bottom" style="height: 50px; margin-top: -2px;"></div>
    </div>
