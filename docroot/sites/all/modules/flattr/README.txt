=== Flattr module ===

The flattr module is not tested enough production environment, but is known to
work.

=== How to use ===
The module supplies a new field called Flattr. This field can be attached to
any of your content, although only node entities are currently known to work.
Please report to the issue queue if you test on other entities.

When creating a new instance of your entity type, there will be a checkbox
to determine whether or not to render the button, and a dropdown to select
which Flattr category the content belongs to.

In order to control for example what tags and description are used for the
button, you create a separate field, and tell the Flattr module to use it.
These settings for field are found under the display settings for your
content type, which is /admin/structure/types/manage/[content-type]/display
The list of fields for Flattr username is currently restricted to fields on
the user entity, while other fields are restricted to the current entity.
Both of these restrictions are likely to be lifted together with the fix for
the ctools issue under == Known problems ==.

Please note that after you've updated the display settings, it's necessary
to also press the save button at the bottom of the page.

Some values are required, and some are optional, see Flattrs documentation
here: http://flattr.com/support/integrate/js


=== Reporting issues ===
Whenever you report a bug or submit a support request, always provide the code
sent to Flattr in the issue. This should look like
<a href="node/1/" rel="Flattr;">This is node 1</a>
and there's two ways to find this information.
1) Use Devel.
View your content, go to the devel tab, select render, and find
['name_of_flattr_field'][0]['#markup']

2) Prevent the module from converting the markup to a button. 

There's a section of code that looks like this:
      '#attached' => array(
        'js' => array(
          'http://api.flattr.com/js/0.6/load.js?mode=auto' => array(
            'type' => 'external',
          ),
        ),
        'css' => array(drupal_get_path('module', 'flattr') . '/flattr.css'),
      ),

which you need to enclose in /* */ like this,

/*      '#attached' => array(
        'js' => array(
          'http://api.flattr.com/js/0.6/load.js?mode=auto' => array(
            'type' => 'external',
          ),
        ),
        'css' => array(drupal_get_path('module', 'flattr') . '/flattr.css'),
      ),*/

Then when viewing your content, you will get the raw data passed to Flattr,
instead of the button that Flattr returns.


=== Known problems ===
Currently doesn't work with Ctools, and as such neither Views and Panels.
This is because in a ctools context the current entity_type normally isn't
known. This module makes certain assumptions about entity_type that then don't
hold true. This will be fixed in a future version.
