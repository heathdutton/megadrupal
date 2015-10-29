<?php 
 ?>
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
                                        <a href="<?php print $base_path ?>" title="<?php print t('home') ?>" > <img src="<?php print $base_path . $directory; ?>/images/home.png" width="24" height="31" alt="<?php print t('home') ?>" /> </a>
                                    </div>
                                    
                                    <?php if ($main_menu || $page['superfish_menu']): ?>
                                        <div id="<?php print $main_menu ? 'nav': 'superfish'; ?>">
                                            <div id="menu-separator"></div>
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
                    
                    <div id="menu-right"></div>
                    
                </div><!--end of menu and search-box-->
                
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
                    
                    <div id="top-note">
                    
                        <?php if ($page['top_note']): ?>
                            <div class="top-note note">
                                <?php print render($page['top_note']); ?>
                            </div>
                        <?php endif; ?>
                    
                    </div><!--end of top note-->
                    
                    <?php endif; ?>
                    
                </div><!--end of branding-->    
                
            </div>
        </div><!--end of header-->
            

        <?php if($is_front): ?>        
        <div id="slideshow-wrapper">
            
            <div id="slideshow-outer">
                <div id="slideshow-inner">
                                
                                
                                <?php if ($page['slideshow_note'] || $page['highlighted']): ?>
                                <div id="mission-and-slideshow-note">    
                                    
                                        <div class="slideshow-note note">
                                            <?php print render($page['slideshow_note']); ?>
                                        </div><!--this is a slideshow note-->
                                            
                                        <div id="mission">
                                            <?php print render($page['highlighted']); ?>
                                        </div><!--end of mission-->
                                
                                </div><!--end of text wrapper in slideshow wrapper-->
                                <?php endif; ?>
                                
                                <div class="slideshow">
                                    <img src="<?php print $base_path . $directory; ?>/images/slideshow/image1.jpg" width="960" height="398" alt="image1"/>
                                    <img src="<?php print $base_path . $directory; ?>/images/slideshow/image2.jpg" width="960" height="398" alt="image2"/>
                                    <img src="<?php print $base_path . $directory; ?>/images/slideshow/image3.jpg" width="960" height="398" alt="image3"/>
                                </div>
                                
                                <!--above is slideshow image config, you can configure it personally,also could to bring more slideshow, just modified it:D -->
                                
                </div>
            </div>
    
        </div>
        <?php endif; ?><!--end of slideshow wrapper-->
        
            
            <?php if($page['preface_first'] || $page['preface_middle'] || $page['preface_last']): ?>
        <div id="preface-wrapper">

                <div id="preface-style"><!--this is to fix preface wrapper style-->
                
                <div class="triplet-wrapper in<?php print (bool) $page['preface_first'] + (bool) $page['preface_middle'] + (bool) $page['preface_last']; ?>">
                <div class="separator-fix">    
                
                <?php if($page['preface_first']): ?>
                    <div class="column A">
                        <?php print render($page['preface_first']); ?>
                    </div>
                <?php endif; ?>
          
                <?php if($page['preface_middle']): ?>
                    <div class="column B">
                        <?php print render($page['preface_middle']); ?>
                    </div>
                <?php endif; ?>
                
                <?php if($page['preface_last']): ?>
                    <div class="column C">
                        <?php print render($page['preface_last']); ?>
                    </div>
                <?php endif; ?>
                
                <div style="clear:both"></div>
                </div>
                </div>
                
                    <div style="clear:both"></div>
                </div><!--end of preface style-->

        </div><!--end of preface wrapper-->
            <?php endif; ?>
            
        
        <div id="container-wrapper">
            
                    
                    <?php if ($breadcrumb): ?>
                        <?php if(!$is_front): ?>
                            <div id="breadcrumb">
                                <?php print $breadcrumb; ?>
                            </div>
                        <?php endif; ?>
                    <?php endif; ?><!--end of breadcrumb-->
        
        
            <div id="container-outer">    
                    
                <div class="top-shadow"></div>        
                <div class="middle-shadow"><!--this is a background of container,consist of top-shadow middle-shadow and bottom-shadow, i create it for two other regions, in bottom-teaser and bottom-wrapper, so it's all connected, just edit this and two other will automatically changed, enjoy it:D-->
                <div class="middle-fix"><!--this is to fix container of middle shadow-->
                
                    
                    <?php if ($page['sidebar_first']): ?>
                        <div id="sidebar-left-wrapper">
                                <div id="sidebar-left" class="sidebar">
                                    <?php print render($page['sidebar_first']); ?>
                                </div>
                        </div>    
                    <?php endif; ?><!--end of sidebar left-->
                            
                    
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
                    
                </div><!--end of middle fix-->
                    <div style="clear:both"></div>
                </div><!--end of middle shadow-->
                <div class="bottom-shadow"></div>
                    
            </div>    

        </div><!--end of conteiner wrapper-->        

        
            <?php if($page['bottom_first'] || $page['bottom_middle'] || $page['bottom_last']): ?>        
        <div id="bottom-teaser">

            <div class="top-shadow"></div>
            <div class="middle-shadow">
            <div class="middle-fix">
                
                <div class="triplet-wrapper in<?php print (bool) $page['bottom_first'] + (bool) $page['bottom_middle'] + (bool) $page['bottom_last']; ?>">
                <div class="separator-fix"><!-- this is to create separator between column, just for bottom teaser and bottom wrapper-->    
                
                <?php if($page['bottom_first']): ?>
                    <div class="column A">
                        <?php print render($page['bottom_first']); ?>
                    </div>
                <?php endif; ?>
                
                <?php if($page['bottom_middle']): ?>
                    <div class="column B">
                        <?php print render($page['bottom_middle']); ?>
                    </div>
                <?php endif; ?>
          
                <?php if($page['bottom_last']): ?>
                    <div class="column C">
                        <?php print render($page['bottom_last']); ?>
                    </div>
                <?php endif; ?>
                
                <div style="clear:both"></div>
                </div><!--end of separator fix-->
                </div>
            
            </div><!--end of middle fix-->
                <div style="clear:both"></div>
            </div><!--end of middle shadow-->    
            <div class="bottom-shadow"></div>
                        
        </div><!--end of bottom teaser-->
            <?php endif; ?>

            
            <?php if($page['bottom_1'] || $page['bottom_2'] || $page['bottom_3'] || $page['bottom_note']): ?>
        <div id="bottom-wrapper">
            
            <div class="top-shadow"></div>
            <div class="middle-shadow">    
            <div class="middle-fix">
            
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
                <?php endif; ?><!--whitebull have many column, its divided by three section, preface column,bottom teaser,and bottom wrapper. every section have a group, column A column B or column C. you can edit it for every section , every group , or maybe every single column . modified it is easy:D -->
                        
                <div style="clear:both"></div>
                </div><!--end of separatir fix-->
                </div>
                
                <?php if($page['bottom_note']): ?>
                    <div class="bottom-note note">
                        <?php print render($page['bottom_note']); ?>
                    </div>
                <?php endif; ?><!--this is a bottom note, in whitebull there are three note(plus 1 note for drupal 7 version), top note,slideshow note and bottom note(plus footer note). use it with your own stuff:D -->
                
            </div><!--end of middle fix-->    
                <div style="clear:both"></div>
            </div><!--end of middle shadow-->
            <div class="bottom-shadow"></div>
            
        </div><!--end of bottom wrapper-->
            <?php endif; ?>
                        
        
        
        <div id="footer">

            <?php if($page['footer_note']): ?>
                <div class="footer-note note footer-message">
                    <?php print render($page['footer_note']); ?>
                </div>
            <?php endif; ?><!--this is another note, i give it for drupal 7 version, just enjoy it-->
            
            <div style="clear:both"></div>
            
            <?php if ($secondary_menu): ?>
                <div id="subnav">
                    <?php print theme('links__system_secondary_menu',
                    array(
                      'links' => $secondary_menu,
                      'attributes' => array(
                        'id' => 'subnav',
                        'class' => array('links', 'clearfix')))); ?>
                </div>
            <?php endif; ?>
            
            <div class="footer">
                <?php print render($page['footer']); ?>
            </div>
                
            <div id="notice">
                <div class="notice" style="font-size:11px;font-style: italic;float: left;margin-left: 2px;"><p>Theme by <a href="http://www.igoen.com">Gunawan Probo Swandono</a>.</p></div>
            </div><!--this is a notice,the banner of me, Gunawan Probo Swandono (a.k.a igoen)-->
            
            <div style="clear:both"></div>
            
        </div><!--end of footer-->

    </div>
