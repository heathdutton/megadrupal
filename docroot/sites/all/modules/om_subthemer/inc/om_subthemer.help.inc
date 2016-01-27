<?php
// $Id$

/**
 * @file
 * OM Subthemer Help
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 * 
 * Layout edit for each OM Subthemer layout
 *
 */

/**
 * Implementation of hook_help().
 *
 */
function om_subthemer_help($path, $arg) {
  $node = node_load($arg[1]);
  switch ($path) {
    // Main module help for the block module
    case 'admin/help#om_subthemer':
      return '<div class="om-subthemer-help">' . t('
<ul>
  <li><h3>Variables</h3>
    <ul>
      <li><strong>$css</strong>: An array of CSS files for the current page.</li>
      <li><strong>$language</strong>: (object) The language the site is being displayed in.</li>
      <li><strong>$language->language</strong> contains its textual representation.</li>
      <li><strong>$language->dir</strong> contains the language direction. It will either be \'ltr\' or \'rtl\'.</li>
      <li><strong>$rdf_namespaces</strong>: All the RDF namespace prefixes used in the HTML document.</li>
      <li><strong>$grddl_profile</strong>: A GRDDL profile allowing agents to extract the RDF data.</li>
      <li><strong>$head_title</strong>: A modified version of the page title, for use in the TITLE tag.</li>
      <li><strong>$head_title_array</strong>: (array) An associative array containing the string parts
        that were used to generate the $head_title variable, already prepared to be
        output as TITLE tag. The key/value pairs may contain one or more of the
        following, depending on conditions:<br />
        - title: The title of the current page, if any.<br />
        - name: The name of the site.<br />
        - slogan: The slogan of the site, if any, and if there is no title.</li>
      <li><strong>$head</strong>: Markup for the HEAD section (including meta tags, keyword tags, and so on).</li>
      <li><strong>$styles</strong>: Style tags necessary to import all CSS files for the page.</li>
      <li><strong>$scripts</strong>: Script tags necessary to load the JavaScript files and settings for the page.</li>
      <li><strong>$page_top</strong>: Initial markup from any modules that have altered the page. This variable should always be output first, before all other dynamic content.</li>
      <li><strong>$page</strong>: The rendered page content.</li>
      <li><strong>$page_bottom</strong>: Final closing markup from any modules that have altered the page. This variable should always be output last, after all other dynamic content.</li>
      <li><strong>$classes</strong> String of classes that can be used to style contextually through CSS.</li>
    </ul>
  </li>                       
  <li><h3>General utility variables</h3>
    <ul>
      <li><strong>$base_path</strong>: The base URL path of the Drupal installation. At the very least, this will always default to /.</li>
      <li><strong>$directory</strong>: The directory the template is located in, e.g. modules/system or themes/bartik.</li>
      <li><strong>$is_front</strong>: TRUE if the current page is the front page.</li>
      <li><strong>$logged_in</strong>: TRUE if the user is registered and signed in.</li>
      <li><strong>$is_admin</strong>: TRUE if the user has permission to access administration pages.</li>
    </ul> 
  </li> 
  <li><h3>Site identity</h3>
    <ul>
      <li><strong>$front_page</strong>: The URL of the front page. Use this instead of $base_path, when linking to the front page. This includes the language domain or prefix.</li>
      <li><strong>$logo</strong>: The path to the logo image, as defined in theme configuration.</li>
      <li><strong>$site_name</strong>: The name of the site, empty when display has been disabled in theme settings.</li>
      <li><strong>$site_slogan</strong>: The slogan of the site, empty when display has been disabled in theme settings.</li>
    </ul>
  </li>
  <li><h3>Navigation</h3>
    <ul>
      <li><strong>$main_menu</strong>: (array) An array containing the Main menu links for the site, if they have been configured.</li>
      <li><strong>$secondary_menu</strong>: (array) An array containing the Secondary menu links for the site, if they have been configured.</li>
      <li><strong>$breadcrumb</strong>: The breadcrumb trail for the current page.</li>
    </ul>
  </li>
  <li><h3>Page content (in order of occurrence in the default page.tpl.php)</h3>
    <ul>
      <li><strong>$title_prefix</strong>: (array) An array containing additional output populated by modules, intended to be displayed in front of the main title tag that appears in the template.
      <li><strong>$title</strong>: The page title, for use in the actual HTML content.
      <li><strong>$title_suffix</strong>: (array) An array containing additional output populated by modules, intended to be displayed after the main title tag that appears in the template.
      <li><strong>$messages</strong>: HTML for status and error messages. Should be displayed prominently.
      <li><strong>$tabs</strong>: (array) Tabs linking to any sub-pages beneath the current page (e.g., the view and edit tabs when displaying a node).
      <li><strong>$action_links</strong>: (array) Actions local to the page, such as \'Add menu\' on the menu administration interface.
      <li><strong>$feed_icons</strong>: A string of all feed icons for the current page.
      <li><strong>$node</strong>: The node object, if there is an automatically-loaded node associated with the page, and the node ID is the second argument in the page\'s path (e.g. node/12345 and node/12345/revisions, but not comment/reply/12345).
      <li><strong>$page[\'help\']</strong>: Dynamic help text, mostly for admin pages.</li>
    </ul>
  </li>     
</ul>       
      
      
      

                     <h3 id="help-guide-base-theme">How to create OM Subthemer base theme files</h3>
                     <p>You must first install the base theme, e.g. OM, Zen, Fusion, Genesis, etc. Create /sites/all/libraries/om_base_themes/new_subtheme folder, this is where you\'ll put your new subtheme. All these files below are just helpers to format the subthemes for specific base theme, you can copy and modify existing base theme files, see OM base theme helper files at om_subthemer/base_themes/om/.</p>
                     <ul>
                       <li><strong>Layout</strong> - base_theme.layout, you can generate you own layout by using the layout edit and pasting the output on the file.</li>  
                       <li><strong>Settings</strong> - base_theme.settings, you can generate you own settings by using the settings edit and pasting the output on the file.</li>  
                       <li><strong>Stylesheets</strong> - base_theme.stylesheets, these are base theme stylesheet files to include, see om.stylesheets sample.</li>  
                       <li><strong>Scripts</strong> - base_theme.scripts, these are base theme script files to include, see om.scripts sample.</li> 
                       <li><strong>Templates</strong> - templates/template.php and other templates, e.g. node.tpl.php, to include from the base theme if needed.</li>   
                     </ul>

                     <h3 id="help-guide-skin">How to create OM Subthemer skin files</h3>
                     <p>All these files must be present, you can copy and modify existing skin files. Create /sites/all/libraries/om_skins/new_skin/ folder, this is where you\'ll put your new skin. See OM Black skin files at om_subthemer/skins/om_black/.</p>
                     <ul>
                       <li><strong>Logo</strong> - Default logo image</li>  
                       <li><strong>Favicon</strong> - Default favicon.ico</li>  
                       <li><strong>Screenshot</strong> - Default screenshot</li>  
                       <li><strong>CSS</strong> - css/style.css, css/ie.css, css/ie6.css, css/ie7.css, css/ie8.css, css/ie9.css</li>  
                       <li><strong>Script</strong> - js/script.js</li>  
                     </ul>
                     
                     ') . '</div>';

    case 'admin/settings/om-subthemer':
      return t('<p>The OM Subthemer will be attached to the following content types if checked.</p>
                <ul>
                  <li><strong>Base Theme</strong> - You can add more base themes on /sites/all/libraries/om_base_themes/ folder. Follow the <a href="@guide_base_theme">guide</a> on creating base theme files.</li>
                  <li><strong>Skins</strong> - You can add more custom skins on /sites/all/libraries/om_skins/ folder. Follow the <a href="@guide_skin">guide</a> on creating skin files.</li>
                </ul>',
                array(
                '@guide_base_theme' => base_path() . 'admin/help/om_subthemer#help-guide-base-theme', 
                '@guide_skin' => base_path() . 'admin/help/om_subthemer#help-guide-skin'));

    case 'admin/settings/om-subthemer/' . $arg[3]:
      return '<p>' . t('These are current available variables for this base theme. Other variables specific to your base theme must be added here.') . '</p>';

    case 'admin/settings/om-subthemer/' . $arg[3] . '/import':
      return '<p>' . t('Importing settings will automatically delete existing settings for this type.') . '</p>';

    case 'node/%/om-skin':
      return t('<p>These are the current available skins.</p>
                <ul>
                  <li><strong>Skins</strong> - You can add more custom skins on /sites/all/libraries/om_skins/ folder. Follow the <a href="@guide_skin">guide</a> on creating skin files.</li>
                </ul>',
                array('@guide_skin' => base_path() . 'admin/help/om_subthemer#help-guide-skin'));

    // Help appears on om-layout
    // known D6 bug: url() doesn't properly encode '#' or '%'
    case 'node/%/om-layout':
      return '<ul>' . t('
                     <li><strong>Variables</strong> are automatically identified by the system via their names. 
                     Click <a href="@variables">here</a> to see all Drupal default variables. 
                     If you want to change/add the default html and php values of these variables, 
                     click <a href="@settings">here</a>. Variables cannot have inner wrappers or inner divs.</li>
                     <li><strong>Regions</strong> are elements without other elements nested under them, or without \'children\' and not identified as variables. Regions can have inner wrappers or inner divs.</li> 
                     <li><strong>Wrappers</strong> are elements with other elements nested under them, or with \'children\'. Wrappers can have inner wrappers or inner divs.</li>
                     <li><strong>Row/Column</strong>, for the layout not to break, a non-variable element in a group with the same level should have the same type, row or column, which is automatically changed if any of its members changed to row or column type.</li>
                     <li><strong>Reset to default layout</strong> can be done by deleting all existing variables, region, wrappers. Deleting a wrapper will automatically delete all elements nested under it, or delete all its children.</li>', 
                     array(
                     '@variables' => base_path() . 'admin/help/om_subthemer#help-drupal-default-variables', 
                     '@settings' => url('admin/settings/om-subthemer/' . $node->type))) . '</ul>';
    case 'node/%/om-layout/import':
      return '<p>' . t('Importing layout will automatically delete existing layout for this node.') . '</p>';

    case 'node/%/om-blocks':
      return '<p>' . t('Click <a href="@regions">here</a> to add/update regions. These are just dummy blocks which you can use for previewing subthemes. You can add any block from Drupal html output. You can copy and paste directly from your browser\'s source output, ex. &lt;div id="block-block-1"&gt;&lt;div ...&lt;/div&gt;.', 
                     array('@regions' => url('node/' . $node->nid . '/om-layout'))) . '</p>';

    case 'node/%/om-blocks/import':
      return '<p>' . t('Importing blocks will automatically delete all existing blocks for this node.') . '</p>';

  }
}



