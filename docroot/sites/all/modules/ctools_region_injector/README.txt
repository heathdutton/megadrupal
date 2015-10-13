Provides a CTools plugin to be used with Panels Everywhere, that allows
outputting the content of a panel page region, as a separate pane inside the
Panels Everywhere site template.

-----------------------------------------------------------------------------
REQUIREMENTS
-----------------------------------------------------------------------------
- CTools
- Page Manager
- Panels
- Panels Everywhere

-----------------------------------------------------------------------------
INSTALLATION
-----------------------------------------------------------------------------
To use this module just enable it.

-----------------------------------------------------------------------------
SAMPLE USAGE
-----------------------------------------------------------------------------
1. Enable the module.

2. Create a custom Panels layout plugin in your custom theme, with some specific
code for the region you want to be injected. Sample layout plugin can be found
in plugins/layouts.

3. Go to Structure -> Pages and create a new custom page (/test).

4. Select the custom layout that you've created for the new page, and add
some content (e.g text pane with 'hello' string) to the region to be injected
(in our case the banner area region).

5. Go to Structure -> Pages -> Site template and add the Region injector pane
into the Header region for example. In the pane configuration enter the machine
name of the region to be injected (in our sample layout it is banner_area).

6. Save the panel and go to your custom panel page (/test). The result should
be that the 'hello' string should be displayed in the header, rather than the
content area.

Note that the 'hello' string was defined in the panel page, rather
than the site template, so you can have dynamically changed content in the
site template header, that is defined separately in any custom panel page.

-----------------------------------------------------------------------------
CREDIT
-----------------------------------------------------------------------------
Maintainers and developers:
- placinta - https://drupal.org/user/176134
