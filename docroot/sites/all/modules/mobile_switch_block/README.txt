Mobile Switch Block module, README.txt

Extends the Mobile Switch module with an theme switch block.

The block content provides a link to manually switch the current theme to
the mobile or desktop theme.

HOW IT WORKS

The module uses a cookie to store the theme was changed to. This works with
anonymous visitors and logged in users.

DEPENDENCIES

Drupal 7.
Mobile Switch module - http://drupal.org/project/mobile_switch.

INSTALL

1)
Copy the Mobile Switch Block folder to the modules folder in
your installation. Usually this is sites/all/modules.
Or use the UI and install it via admin/modules/install.

2)
In your Drupal site, enable the module under Administration -> Modules.
admin/modules

ADMINISTER

Administer blocks under:
Administration -> Structure
admin/structure/block

a)
Singlesite installation
Enable the 'Mobile Switch' block in the desktop theme.
Enable the 'Mobile Switch' block in the mobile theme.

b)
Multisite installation
Enable the 'Mobile Switch' blocks in both multisite themes.

c)
Singlesite, Multisite:
Configure one of the 'Mobile Switch' blocks.

Administer the option 'Block content' to display the block content. The same
configuration option exists in the Advanced module administration.

d)
Administer the Mobile Switch 'Advanced' settings under:
Administration -> Configuration -> User interface -> Mobile Switch
admin/config/user-interface/mobile-switch/advanced

- Configure the output of the switch content.

- Configure the 'Cookie lifetime' when needed.

Lifetime examples:

31536000 - 1 year (default)
2628000  - 30 days
604800   - 1 week
86400    - 24 hours

- Use the switch block only on large mobile devices.

Enable the option 'Non-tablet usage'.

DEVELOPMENT

Don't forget to take a look at the 'Development' funtions in the Mobile Switch
module settings.

Development with Mobile Switch Block.

1)
The module provides the system variable:

theme_cookie

The default value of this variable is FALSE. If the theme switch cookie
exists the value can be 'standard' or 'mobile'.
If used the operationg mode redirect the cookie values can be
'standard-rm' or 'mobile-rm'.

2)
Use the block content. Example PHP code to do this:

@code
print mobile_switch_block_get_block_content();
@endcode

In this use case, you ensure that the switch link content appears only
with mobile devices.

THEMING

- Module template

 The module comes with a template file, included in the module folder.

 mobile-switch-block-content.tpl.php

 Is used for the switch content - the switch link and the switch message.

- Use of the template

 This requires that there is a mobile-switch-block-content.tpl.php template
 in the desktop and/or mobile theme folder.

- Switch link without menu bullet

Remove in the template/s the class 'leaf' from the list tag.

@code
<li class="even first last">
@endcode

This will work with most themes and is recommended rather than an additional
CSS formatting.

- Usage with a 'Mobile jQuery' theme

Mobile jQuery Theme - http://drupal.org/project/mobile_jquery

To obtain a 'Mobile jQuery' like output of the switch link, create a
template file for the Mobile jQuery sub-theme from the desktop template file
'mobile-switch-block-switch-content.tpl.php' with the same name and put this
new template file in the 'templates' folder. Example path for the new
Mobile jQuery sub-theme template file:

sites/all/themes/your_mobile_jquery_sub_theme/templates/

In the new template file replace the code

@code
<?php if ($switch_message): ?>
  <?php print $switch_message ?>
<?php endif;?>
<ul class="menu clearfix">
  <li class="leaf even first last">
    <?php print $switch_link ?>
  </li>
</ul>
@endcode

with the following code

@code
<?php print $content ?>
@endcode

It is therefore possible to use different templates in the desktop and
mobile Theme.

Note:
Take a look at the Mobile Switch Block module folder
templates/mobile_jquery.
