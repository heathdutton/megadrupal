<?php 
 ?>
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
                    
                    <?php if ($feed_icons): ?>
                        <div class="feed-wrapper">
                            <?php print $feed_icons; ?>
                        </div>
                    <?php endif; ?><!--end of rss feed-->
                
                </div><!--end of authorize and feed icon-->
                
                <div style="clear:both"></div>
                
                
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
                                    
                                    <?php if ($main_menu || $page['superfish_menu']): ?>
                                        <div id="<?php print $main_menu ? 'nav': 'superfish'; ?>">
                                             <?php 
                                                if ($main_menu):
                                                print theme('links__system_main_menu', array('links' => $main_menu));
                                                elseif (!empty($page['superfish_menu'])):
                                                print render($page['superfish_menu']);
                                                endif;
                                             ?>
                                        </div><!--end of menu-->
                                    <?php endif; ?>
                                        
                                <?php if ($page['search_box']): ?>
                                    <div id="search-box">
                                        <?php print render($page['search_box']); ?>
                                    </div>
                                <?php endif; ?><!--this is a search box-->
                                                                
                    </div>
                </div><!--end of menu and search-box-->
                    
                
            </div>
        </div><!--end of header-->
   
        <div id="container">
        <div id="container-wrapper">

                    <?php if ($breadcrumb): ?>
                        <?php if(!$is_front): ?>
                            <div id="breadcrumb">
                                <?php print $breadcrumb; ?>
                            </div>
                        <?php endif; ?>
                    <?php endif; ?><!--end of breadcrumb-->        
                <div style="clear:both"></div>
                
            <div id="container-outer">
                    
                    <div id="main-content">
                        
                        <?php if ($page['content_top']): ?><div id="content-top"><?php print render($page['content_top']); ?></div><?php endif; ?>
                        <?php if ($show_messages): print $messages; endif;?>
                        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
                        <?php if ($title): ?><h1 class="title"><?php print render($title_suffix); ?></h1><?php endif; ?>
                        <?php print render($page['help']); ?>
                        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
                        <?php if ($page['content']): ?><div id="content-middle"><?php print render($page['content']); ?></div><?php endif; ?>
                        <?php if ($page['content_bottom']): ?><div id="content-bottom"><?php print render($page['content_bottom']); ?></div><?php endif; ?>

                    </div><!--this is main content, you can edit this if you want to change arange of main content-->
                        
                        
                    <?php if ($page['sidebar_second']): ?>
                        <div id="sidebar-right-wrapper">
                                <div id="sidebar-right" class="sidebar">
                                    <?php print render($page['sidebar_second']); ?>
                                </div>    
                        </div>
                    <?php endif; ?><!--end of sidebar right-->
                    
            </div>    
            
            <div style="clear:both"></div>
        </div><!--end of container wrapper-->        

        
            <?php if($page['bottom_1'] || $page['bottom_2'] || $page['bottom_3'] || $page['bottom_note']): ?>
        <div id="bottom-wrapper">
            
                <div class="triplet-wrapper in<?php print (bool) $page['bottom_1'] + (bool) $page['bottom_2'] + (bool) $page['bottom_3']; ?>">
                <div class="separator-fix">
                <?php if($page['bottom_1']): ?>
                    <div class="column A">
                        <?php print render($page['bottom_1']); ?>
                    </div>
                <?php endif; ?>
          
                <?php if($page['bottom_2']): ?>
                    <div class="column B">
                        <?php print render($page['bottom_2']); ?>
                    </div>
                <?php endif; ?>
          
                <?php if($page['bottom_3']): ?>
                    <div class="column C">
                        <?php print render($page['bottom_3']); ?>
                    </div>
                <?php endif; ?> 
                <div style="clear:both"></div>
                </div>
                </div>
                
                <?php if(($page['bottom_1'] || $page['bottom_2'] || $page['bottom_3']) && ($page['bottom_note'])): ?>
                    <div id="bottom-note-separator"></div>
                <?php endif; ?>
                
                <?php if($page['bottom_note']): ?>
                    <div class="bottom-note">
                        <?php print render($page['bottom_note']); ?>
                    </div>
                <?php endif; ?>
                                        
        </div><!--end of bottom wrapper-->
            <?php endif; ?>

        <div style="clear:both"></div>
        
        <div id="footer">

            <?php if($page['footer']): ?>
                <div class="footer">
                    <?php print render($page['footer']); ?>
                </div>
            <?php endif; ?>
            
            <div style="clear:both"></div>

            <div style="clear:both"></div>
            <div id="notice">
                <div class="notice" style="font-size:11px;text-align: center;margin-left: 2px;"><p>Theme by <a href="http://www.igoen.com">Gunawan Probo Swandono</a>.</p></div>
            </div><!--this is a notice,the banner of me, Gunawan Probo Swandono (a.k.a igoen)-->
            
            <div style="clear:both"></div>
            
        </div><!--end of footer-->
    </div>
    </div>
    <div id="background-bottom" style="height: 50px;"></div>
    </div>
    </div>
