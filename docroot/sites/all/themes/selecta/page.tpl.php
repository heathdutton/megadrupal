<!--header-container-->
<div id="header-container">
	<!--header-->
	<div id="header" class="clearfix">

        <!-- #header-left -->
        <div id="header-left">
        
            <?php if ($logo): ?>
                <div id="logo">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
                <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
                </a>
                </div>
            <?php endif; ?>
            
            <?php if ($site_name || $site_slogan): ?>
                <?php if ($site_name): ?>
                <div id="site-name"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></div>
                <?php endif; ?>
                
                <?php if ($site_slogan): ?>
                <div id="slogan"><?php print $site_slogan; ?></div>
                <?php endif; ?>
            <?php endif; ?>
        
        </div><!-- EOF: #header-left -->
        
        <!-- #header-right -->
        <div id="header-right">
        
        <?php if ($page['header']) : ?>
        <?php print drupal_render($page['header']); ?>
        <?php else : ?>
            <div id="main-menu">
            <?php print theme('links__system_main_menu', array(
              'links' => $main_menu,
              'attributes' => array(
                'class' => array('links', 'main-menu', 'menu'),
              ),
              'heading' => array(
                'text' => t('Main menu'),
                'level' => 'h2',
                'class' => array('element-invisible'),
              ),
            )); ?>
            </div>
        <?php endif; ?>
    
        </div><!-- EOF: #header-right -->
            
	</div><!--EOF:header-->

</div><!--EOF:header-container-->

<div id="feature-area" class="clearfix">
	<div id="feature-area-inside">
    
    <?php if($is_front): ?>
    
		<?php if($page['feature_area']): ?>
        <?php print render($page['feature_area']);?>
        <?php endif;?>
        
        <!-- Javascript Implementation -->
        <?php if (theme_get_setting('feature_display','selecta')): ?>
        <div class="selected-feature">
        
            <div id="feature-node-header-1" class="feature-title">
            <h4>17 Jan 2012</h4>
            <h2><a href="<?php print url('node/6'); ?>">Elbow /// One Day Like This</a></h2>
            <div id="first_selected" class="no_display">1</div>
            </div>
            
            <div id="feature-node-header-2" class="feature-title no_display">
            <h4>21 Jan 2012</h4>
            <h2><a href="<?php print url('node/7'); ?>">Cinnamon Chasers - Luv Deluxe (Official Music Video)</a></h2>
            </div>
            
            <div id="feature-node-header-3" class="feature-title no_display">
            <h4>21 Jan 2012</h4>
            <h2><a href="<?php print url('node/8'); ?>">Blue</a></h2>
            </div>
            
            <div id="feature-node-header-4" class="feature-title no_display">
            <h4>21 Jan 2012</h4>
            <h2><a href="<?php print url('node/9'); ?>">Birds on the Wires</a></h2>
            </div>
            
            <div id="feature-node-header-5" class="feature-title no_display">
            <h4>6 Mar 2012</h4>
            <h2><a href="<?php print url('node/11'); ?>">Artificial Paradise, INC.</a></h2>
            </div>
        
            <div id="feature-media-container" class="container-light">
            
                <div id="feature-node-media-1" >
                <iframe src="http://player.vimeo.com/video/4224453" width="640" height="360" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
                </div>
                
                <div id="feature-node-media-2" class="no_display">
                <iframe src="http://player.vimeo.com/video/6540668" width="640" height="360" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
                </div>
                
                <div id="feature-node-media-3" class="no_display">
                <iframe src="http://player.vimeo.com/video/6050064" width="640" height="360" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
                </div>
                
                <div id="feature-node-media-4" class="no_display">
                <iframe src="http://player.vimeo.com/video/6428069" width="640" height="360" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
                </div>
                
                <div id="feature-node-media-5" class="no_display">
                <iframe src="http://player.vimeo.com/video/6132324" width="640" height="360" frameborder="0" webkitAllowFullScreen mozallowfullscreen allowFullScreen></iframe>
                </div>
            
            </div>
        
        </div>
        
        <ul class="feature-list">
            <li>
                <span class="item">1</span>
                <h4>17 Jan 2012</h4>
                <a id="feature-href-1" href="">Elbow /// One Day Like This</a>
            </li>
            <li>
                <span class="item">2</span>
                <h4>21 Jan 2012</h4>
                <a id="feature-href-2" href="">Cinnamon Chasers - Luv Deluxe (Official Music Video)</a>
            </li>
            <li>
                <span class="item">3</span>
                <h4>21 Jan 2012</h4>
                <a id="feature-href-3" href="">Blue</a>
            </li>
            <li>
                <span class="item">4</span>
                <h4>21 Jan 2012</h4>
                <a id="feature-href-4" href="">Birds on the Wires</a>
            </li>
            <li>
                <span class="item">5</span>
                <h4>6 Mar 2012</h4>
                <a id="feature-href-5" href="">Artificial Paradise, INC.</a>
            </li>
        </ul>
        <?php endif;?><!-- EOF:Javascript Implementation -->
        
	<?php endif;?>

	<?php if(isset($node) && variable_get('node_submitted_' . $node->type, TRUE)): ?>
    <h4 class="date"><?php print format_date($node->created, 'custom', 'j M Y');?></h4>
    <?php endif;?>
    
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
    <h2 class="title"><?php print $title; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    
    <?php if(isset($node) &&  !empty($node->body['und'][0]['summary'])): ?>
    <div class="node">
        <div class="copy">
        <?php print $node->body['und'][0]['summary']; ?>
        </div>
    </div>
    <?php endif; ?>
            
	</div>
</div>

<div id="content-container" class="clearfix">

	<?php if($page['slider'] && $is_front):	?>
	<div id="slider" class="clearfix">
		<?php print render($page['slider']);?>
	</div>
	<?php endif;?>

	<?php if($page['sidebar_first']):?>
	<div id="sidebar">
		<?php print render($page['sidebar_first']);?>
	</div>
    <?php endif;?>
    
	<div id="content">
    
		<?php if (theme_get_setting('breadcrumb_display','selecta')): print $breadcrumb; endif; ?>
        
        <?php if ($tabs): ?>
        <div class="tabs">
        <?php print render($tabs); ?>
        </div>
        <?php endif; ?>
        
        <?php print $messages; ?>
        
        <?php print render($page['help']); ?>
        
        <a id="main-content"></a>
        
        <?php if ($action_links): ?>
        <ul class="action-links">
        <?php print render($action_links); ?>
        </ul>
        <?php endif; ?>
        
        <?php print render($page['content']); ?>
        
        <?php print $feed_icons; ?>
        
	</div><!--main-->
</div><!--main-area-->

<!--footer-container-->
<div id="footer-container" class="clearfix">
	<!--footer-->
	<div id="footer" class="clearfix">

        <ul class="footer-columns clearfix">
            <li class="column-one">
                <?php print render($page['footer_left']);?>
            </li>
            <li class="column-two">
                <?php print render($page['footer_center']);?>
            </li>
            <li class="column-three">
                <?php print render($page['footer_right']);?>
            </li>
        </ul>

		<div class="footer-bottom clearfix">
			<?php print render($page['footer']);?>
			<p class="copyright">Ported to Drupal for the Open Source Community by <a href="http://www.drupalizing.com">Drupalizing</a>, a Project of <a href="http://www.morethanthemes.com">More than Themes</a>. Original design by <a href="http://www.obox-design.com/">Obox themes</a>.</p>
		</div>
        
	</div><!--EOF:footer-->
    
</div><!--EOF:footer-container-->