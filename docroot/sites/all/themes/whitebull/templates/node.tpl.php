<?php 
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <?php print render($title_prefix); ?>
            <h2<?php print $title_attributes; ?> class="title">
                <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
            </h2>
        
        
        <?php print render($title_suffix); ?>
        <?php if ($display_submitted): ?>
            <div class="meta submitted">
                
                <div id="user-picture">
                    <?php print $user_picture; ?>
                </div>
                
                <?php
                    print t('published by !username on !datetime',
                    array('!username' => $name, '!datetime' => $date));
                ?>
            </div>
        <?php endif; ?>

        
        <div class="content clearfix"<?php print $content_attributes; ?>>
            <?php
                hide($content['comments']);
                hide($content['links']);
                print render($content);
            ?>
        </div>
        
        <?php
            if ($teaser || !empty($content['comments']['comment_form'])):
                unset($content['links']['comment']['#links']['comment-add']);

            $links = render($content['links']);
            if ($links):
            endif;
        ?>
            <div class="links">
                <?php print $links; ?>
            </div>
        <?php endif; ?>
        
        <?php print render($content['comments']); ?>
        
</div>
