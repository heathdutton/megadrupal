CTools Custom Link allows you to add custom links inside panels, leading to any user-defined page.
It is implemented as a CTools Content Plugin, and thus can be reused anywhere in the code as well.

-----------------------------------------------------------------------------
CURRENT FEATURES
-----------------------------------------------------------------------------
- The following properties can be set: URL, Text, CSS classes, and HTML attributes.
- Supports token replacements in the URL, Text, and CSS Classes fields.
- Supports context keyword substitutions when any contexts are available in the panel.
- Can be re-used anywhere in the code, by calling ctools_content_render and specifying a configuration array.

-----------------------------------------------------------------------------
REQUIREMENTS
-----------------------------------------------------------------------------
Requires CTools module, and is useful when Panels and Page Manager is enabled.
If token module is enabled, will provide a token tree of available replacements.

-----------------------------------------------------------------------------
INSTALLATION
-----------------------------------------------------------------------------
To use this module just enable it.

-----------------------------------------------------------------------------
SAMPLE USAGE
-----------------------------------------------------------------------------
1. Enable the module.
2. Got to Structure -> Pages and create a new page.
3. Go to the content page, click the gear on any region -> Add content.
4. Choose Custom link in the left of side of the window.
5. Fill out the required URL and Text properties, and any others needed.
6. Save the content pane and page, navigate to the page, and rejoice that your link has appeared.

-----------------------------------------------------------------------------
SAMPLE CODE USAGE
-----------------------------------------------------------------------------
When you want to render a custom link in the code, all you need to do is call ctools_content_render and pass a
configuration array. A sample snippet would be.

function my_module_display_certain_page() {
  ctools_include('content');
  ctools_include('context');
  $node = node_load(1);
  $context = ctools_context_create('node', $node);
  $context = array('context_node_1' => $context);
  $conf = array(
    'url' => '<front>',
    'text' => 'Homepage <b>[site:name]</b>', // Will be token replaced
    'classes' => 'first-class second-class node-%node:nid', // The context keyword will be substituted
    'attributes' => "id|front-page-link\ntarget|_blank", // Will set the id of the link, and the target.
    'substitute' => TRUE, // Will enable context keyword substitution
    'html' => TRUE, // Text will not be check_plain'ed, so html can be used.
  );
  $link = ctools_content_render('ctools_custom_link', 'ctools_custom_link', $conf, array(), array(), $context);
  return $link;
}

-----------------------------------------------------------------------------
CREDIT
-----------------------------------------------------------------------------
Maintainers and developers:
- placinta - https://drupal.org/user/176134
- asgorobets - https://drupal.org/user/1399950 (for the token integration code).