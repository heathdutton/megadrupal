<?php
	$vars = get_defined_vars();
	$view = get_artx_drupal_view();
	$view->print_head($vars);

	if (isset($page))
		foreach (array_keys($page) as $name)
				$$name = & $page[$name];
	$art_sidebar_left = isset($sidebar_left) && !empty($sidebar_left) ? $sidebar_left : NULL;
	$art_sidebar_right = isset($sidebar_right) && !empty($sidebar_right) ? $sidebar_right : NULL;
	if (!isset($vnavigation_left)) $vnavigation_left = NULL;
	if (!isset($vnavigation_right)) $vnavigation_right = NULL;
	$tabs = (isset($tabs) && !(empty($tabs))) ? '<ul class="arttabs_primary">'.render($tabs).'</ul>' : NULL;
	$tabs2 = (isset($tabs2) && !(empty($tabs2))) ?'<ul class="arttabs_secondary">'.render($tabs2).'</ul>' : NULL;
?>

<div id="b2-main">
<div class="b2-header">
    <div class="b2-header-clip">
    <div class="b2-header-center">
        <div class="b2-header-png"></div>
    </div>
    </div>
<div class="b2-header-wrapper">
<div class="b2-header-inner">
<div class="b2-logo">
    <?php
    if (!empty($logo)) {
        ?>
        <div style="float:left;height:60px;overflow: hidden;">
         <?php echo '<a href="'.check_url($front_page).'" title = "'.$site_name.'"><img src="'.$logo.'" alt="'.$site_name.'" style="padding-right:10px;" /></a>'; ?>
        </div>
        <?php
    }
    ?>
    
    <div style="float:left;float:left;height:60px;">
     <?php  if (!empty($site_name)) { echo '<h1 class="b2-logo-name"><a href="'.check_url($front_page).'" title = "'.$site_name.'">'.$site_name.'</a></h1>'; } ?>
     <?php   if (!empty($site_slogan)) { echo '<h2 class="b2-logo-text">'.$site_slogan.'</h2>'; } ?>
    </div>
    
    <?php
    
    ?>


</div>

</div>
</div>
</div>
<div class="cleared reset-box"></div>
<?php if (!empty($navigation) || !empty($extra1) || !empty($extra2)): ?>
<div class="b2-nav">
<div class="b2-nav-outer">
<div class="b2-nav-wrapper">
<div class="b2-nav-inner">
    <?php if (!empty($extra1)) : ?>
    <div class="b2-hmenu-extra1"><?php echo render($extra1); ?></div>
    <?php endif; ?>
    <?php if (!empty($navigation)) : ?>
    <?php echo render($navigation); ?>
    <?php endif; ?>
    <?php if (!empty($extra2)) : ?>
    <div class="b2-hmenu-extra2"><?php echo render($extra2); ?></div>
    <?php endif; ?>
</div>
</div>
</div>
</div>
<div class="cleared reset-box"></div>
<?php endif;?>
<div class="b2-sheet">
    <div class="b2-sheet-tl"></div>
    <div class="b2-sheet-tr"></div>
    <div class="b2-sheet-bl"></div>
    <div class="b2-sheet-br"></div>
    <div class="b2-sheet-tc"></div>
    <div class="b2-sheet-bc"></div>
    <div class="b2-sheet-cl"></div>
    <div class="b2-sheet-cr"></div>
    <div class="b2-sheet-cc"></div>
    <div class="b2-sheet-body">
<?php if (!empty($banner1)) { echo '<div id="banner1">'.render($banner1).'</div>'; } ?>
<?php echo art_placeholders_output(render($top1), render($top2), render($top3)); ?>
<div class="b2-content-layout">
    <div class="b2-content-layout-row">
<?php if (!empty($art_sidebar_left) || !empty($vnavigation_left))
echo art_get_sidebar($art_sidebar_left, $vnavigation_left, 'b2-sidebar1'); ?>
<div class="<?php echo art_get_content_cell_style($art_sidebar_left, $vnavigation_left, $art_sidebar_right, $vnavigation_right, $content); ?>">
<?php if (!empty($banner2)) { echo '<div id="banner2">'.render($banner2).'</div>'; } ?>
<?php if ((!empty($user1)) && (!empty($user2))) : ?>
<table class="position" cellpadding="0" cellspacing="0" border="0">
<tr valign="top"><td class="half-width"><?php echo render($user1); ?></td>
<td><?php echo render($user2); ?></td></tr>
</table>
<?php else: ?>
<?php if (!empty($user1)) { echo '<div id="user1">'.render($user1).'</div>'; }?>
<?php if (!empty($user2)) { echo '<div id="user2">'.render($user2).'</div>'; }?>
<?php endif; ?>
<?php if (!empty($banner3)) { echo '<div id="banner3">'.render($banner3).'</div>'; } ?>
<?php if (!empty($breadcrumb)): ?>
<div class="b2-post">
    <div class="b2-post-body">
<div class="b2-post-inner b2-article">
<div class="b2-postcontent">
<?php { echo $breadcrumb; } ?>

</div>
<div class="cleared"></div>

</div>

		<div class="cleared"></div>
    </div>
</div>
<?php endif; ?>
<?php if (($is_front) || (isset($node) && isset($node->nid))): ?>              
<?php if (!empty($tabs) || !empty($tabs2)): ?>
<div class="b2-post">
    <div class="b2-post-body">
<div class="b2-post-inner b2-article">
<div class="b2-postcontent">
<?php if (!empty($tabs)) { echo $tabs.'<div class="cleared"></div>'; }; ?>
<?php if (!empty($tabs2)) { echo $tabs2.'<div class="cleared"></div>'; } ?>

</div>
<div class="cleared"></div>

</div>

		<div class="cleared"></div>
    </div>
</div>
<?php endif; ?>
                <?php if (!empty($mission) || !empty($help) || !empty($messages) || !empty($action_links)): ?>
<div class="b2-post">
    <div class="b2-post-body">
<div class="b2-post-inner b2-article">
<div class="b2-postcontent">
<?php if (isset($mission) && !empty($mission)) { echo '<div id="mission">'.$mission.'</div>'; }; ?>
<?php if (!empty($help)) { echo render($help); } ?>
<?php if (!empty($messages)) { echo $messages; } ?>
<?php if (isset($action_links) && !empty($action_links)): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>

</div>
<div class="cleared"></div>

</div>

		<div class="cleared"></div>
    </div>
</div>
<?php endif; ?>
                <?php $art_post_position = strpos(render($content), "b2-post"); ?>
<?php if ($art_post_position === FALSE): ?>
<div class="b2-post">
    <div class="b2-post-body">
<div class="b2-post-inner b2-article">
<div class="b2-postcontent">
<?php endif; ?>
<?php echo art_content_replace(render($content)); ?>
<?php if ($art_post_position === FALSE): ?>

</div>
<div class="cleared"></div>

</div>

		<div class="cleared"></div>
    </div>
</div>
<?php endif; ?>
<?php else: ?>
<div class="b2-post">
    <div class="b2-post-body">
<div class="b2-post-inner b2-article">
<div class="b2-postcontent">
<?php if (!empty($title)): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
<?php if (!empty($tabs)) { echo $tabs.'<div class="cleared"></div>'; }; ?>
<?php if (!empty($tabs2)) { echo $tabs2.'<div class="cleared"></div>'; } ?>
<?php if (isset($mission) && !empty($mission)) { echo '<div id="mission">'.$mission.'</div>'; }; ?>
<?php if (!empty($help)) { echo render($help); } ?>
<?php if (!empty($messages)) { echo $messages; } ?>
<?php if (isset($action_links) && !empty($action_links)): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
<?php echo art_content_replace(render($content)); ?>

</div>
<div class="cleared"></div>

</div>

		<div class="cleared"></div>
    </div>
</div>
<?php endif; ?>
<?php if (!empty($banner4)) { echo '<div id="banner4">'.render($banner4).'</div>'; } ?>
<?php if (!empty($user3) && !empty($user4)) : ?>
<table class="position" cellpadding="0" cellspacing="0" border="0">
<tr valign="top"><td class="half-width"><?php echo render($user3); ?></td>
<td><?php echo render($user4); ?></td></tr>
</table>
<?php else: ?>
<?php if (!empty($user3)) { echo '<div id="user1">'.render($user3).'</div>'; }?>
<?php if (!empty($user4)) { echo '<div id="user2">'.render($user4).'</div>'; }?>
<?php endif; ?>
<?php if (!empty($banner5)) { echo '<div id="banner5">'.render($banner5).'</div>'; } ?>
</div>
<?php if (!empty($art_sidebar_right) || !empty($vnavigation_right))
echo art_get_sidebar($art_sidebar_right, $vnavigation_right, 'b2-sidebar2'); ?>

    </div>
</div>
<div class="cleared"></div>

<?php echo art_placeholders_output(render($bottom1), render($bottom2), render($bottom3)); ?>
<?php if (!empty($banner6)) { echo '<div id="banner6">'.render($banner6).'</div>'; } ?>
<div class="b2-footer">
    <div class="b2-footer-t"></div>
    <div class="b2-footer-l"></div>
    <div class="b2-footer-b"></div>
    <div class="b2-footer-r"></div>
    <div class="b2-footer-body">
        <?php 
            if (!empty($feed_icons)) {
                echo $feed_icons;
            }
            else {
                echo '<a href="'.url("rss.xml").'" class="b2-rss-tag-icon"></a>';
            }
        ?>
                <div class="b2-footer-text">
                        <?php
                    $footer = render($footer_message);
                    if (!empty($footer) && (trim($footer) != '')) {
                        echo $footer;
                    }
                    else {
                        ob_start(); ?>
<p>Copyright © 2011. All Rights Reserved.</p>


                        <?php echo str_replace('%YEAR%', date('Y'), ob_get_clean());
                    }
                ?>
                <?php if (!empty($copyright)) { echo $copyright; } ?>
                </div>
		<div class="cleared"></div>
    </div>
</div>
		<div class="cleared"></div>
    </div>
</div>
<div class="cleared"></div>
<p class="b2-page-footer">Designed by [ <a href="http://www.b2hq.com" target="_blank">b2</a> ] </p>

</div>


<?php $view->print_closure($vars); ?>