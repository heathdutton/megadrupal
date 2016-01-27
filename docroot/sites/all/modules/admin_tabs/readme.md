# Menu Tabs

Creates floating tasks and actions, to avoid the need to accomodate them in a theme. Intended to work with the [Administration Menu](http://drupal.org/project/admin_menu) module.

### To Do

-   UI - I built the UI and I'm not a designer. I am not enamored with my work. Please help.

## Instructions

1.  Install and enable in the normal way.
2.  Remove calls to render tabs and actions in your theme. For example, override *modules/system/page.tpl.php* and remove the lines

        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>

    and

        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>

## Developer

I'm using [compass](http://compass-style.org/) to build the module CSS. Feel free to edit the CSS files directly if the overhead of using compass would prevent you from contributing :)

Similarly, the javascript is minified. Use the minify tool of your choice.
