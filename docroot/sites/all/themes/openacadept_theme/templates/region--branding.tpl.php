<div<?php print $attributes; ?>>
    <div<?php print $content_attributes; ?>>
        <?php if ($linked_logo_img || $site_name || $site_slogan): ?>
            <div class="branding-data clearfix">
                <div id="branding-1">
                    <?php if ($linked_logo_img): ?>
                        <div class="logo-img">
                            <?php print $linked_logo_img; ?>
                        </div>
                    <?php endif; ?>
                    <?php if ($site_name || $site_slogan): ?>
                        <?php $class = $site_name_hidden && $site_slogan_hidden ? ' element-invisible' : ''; ?>
                        <hgroup class="site-name-slogan<?php print $class; ?>">        
                            <?php if ($site_name): ?>
                                <?php $class = $site_name_hidden ? ' element-invisible' : ''; ?>
                                <?php if ($is_front): ?>        
                                    <h1 class="site-name<?php print $class; ?>"><?php print $linked_site_name; ?></h1>
                                <?php else: ?>
                                    <h2 class="site-name<?php print $class; ?>"><?php print $linked_site_name; ?></h2>
                                <?php endif; ?>
                            <?php endif; ?>
                            <?php if ($site_slogan): ?>
                                <?php $class = $site_slogan_hidden ? ' element-invisible' : ''; ?>
                                <h6 class="site-slogan<?php print $class; ?>"><?php print $site_slogan; ?></h6>
                            <?php endif; ?>
                        </hgroup>
                    </div>
                    <div id="branding-2">
                        <div id="header-user-menu">
                            <?php
                            $menu_name = variable_get('menu_main_links_source', 'user-menu');
                            $main_menu_tree = menu_tree($menu_name);
                            print drupal_render($main_menu_tree);
                            ?>
                        </div>
                        <div id="header-search-block">
                            <?php
                            $block = module_invoke('search', 'block_view', 'search');
                            if ($block) {
                                print render($block);
                            }
                            ?>
                        </div>
                        <div id="header-language-block">
                            <?php
                            $block = block_load('locale', 'language');
                            if ($block) {
                                print drupal_render(_block_get_renderable_array(_block_render_blocks(array($block))));
                            }
                            ?>
                        </div>
                    </div>  
                <?php endif; ?>
            </div>
            <div class="clearfix"></div>
        <?php endif; ?>
        <?php print $content; ?>
    </div>
</div>
