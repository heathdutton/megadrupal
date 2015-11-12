CRM Core Profile for Drupal 7.x
-------------------------------
CRM Core Profile is a form builder for CRM Core. With it, administrators
can quickly build forms for capturing contact and activity information, and 
control various parts of the workflow around form submission.

Forms built with CRM Core Profile can be displayed as stand-alone pages, 
blocks, attached to display inline with nodes, or displayed programatically.

CRM Core Profile can be extended through the use of entity handlers. Some 
notes about extending it are included in this file.

Requirements
------------
* CRM Core Contact
* CRM Core Activity

Installation
------------
1) Download CRM Core Profile to the modules directory of your website.

2) Enable the module from the admin/build/modules page.

Notes on Usage
--------------
The main configuration page for CRM Core Profile can be found at  
admin/structure/crm-core/profile. Each profile listed on this page is a 
distinct form that can be used to accept information from users. 

1) Creating a profile 

When adding a new profile, you will be asked to select one or more entity
types to use in the form, along with an associated bundle. The bundle
will control what fields are available to place in a form.

When adding fields to a profile, check the box next to the field name. The 
field will appear in the drag and drop interface at the bottom of the screen,
allowing you to control the placement of fields (along with other options).

Fields added to the drag and drop interface appear as rows, and there are 
a number of items to be aware of:

- active: the checkbox to the left indicates whether or not the field will
  be displayed to users. Checking this box simply means the field will be 
  visible. Not checking it means it will be available upon form submission.
  
- title: the title of the field. Always displays with the name of the entity
  it is associated with, in paraentheses.
  
- hidden: the hidden checkbox means the field will be rendered as a hidden
  field, regardless of other factors. 
  
- default value: used to pass default values into the form. Default values
  can include static values and tokens.   

2) Profile Settings

Each profile has a number of settings attached to it, depending on what 
entities have been selected. The settings page for each profile allows users 
to configure how the form behaves when presented to users. There are some 
settings that are common to most CRM Core Profiles:

- Create a page: allows you to display the profile as a standalone page.

- Create a block: allows you to display the profile within a block.

- Message to users: a message that is displayed when the profile is 
  successfully submitted.

- Permissions: controls what user roles can see the profile.

- Contact Matching: controls how CRM Core will attempt to match contact
  record information with existing records. 
  
- Propopulate: CRM Core Profile can attempt to prepopulate a form with information
  about the current user. This controls whether this happens automatically
  or via a variable in the URL.

Depending on how your site is configured, other options may appear on the 
settings page.

3) Node Displays

CRM Core Profile forms can be attached to and displayed alongside nodes in your 
Drupal site. Administrators can configure different content types to display
profiles, and control how they are displayed.

To enable this, select a content type from the administrative screen at 
admin/structure/types/manage. On the edit screen, you will see an option for
CRM Core Profile. Check the checkbox to reveal the following settings:

- Vertical tab label: The default profile selected for the node type can be 
  overridden for each piece of content. Enter a label to use when displaying 
  profile options on the selected node. For example: donation form.
  
- Select default profile: Allows administrators to select a default profile
  to associate with this content type.
  
When a node with the selected content type is edited, there should now be a 
vertical tab with the label selected for the content type. This tab should 
include the following options:

- Select profile: allows users to select between one or more profile forms.

- Display inline: checking this box will display the form as part of the node.
  It will automatically be added to the display of the node when it is viewed.
  Uncheck this box to hide the form and display it another way.
  
- Form label: by default, when the form is presented, CRM Core Profile will 
  attempt to add a title equal to the name of the profile. Enter alternate titles
  in this field.
  
4) Block Displays

CRM Core Profile includes a block that will automatically display any forms
that are not being displayed inline. If you are choosing not to display forms
inline, this is simply a convenient way to display the forms elsewhere on the page.

Look for the block titled 'CRM Core Profile: Display Hidden Form'. You can use this 
to control where forms will appear in your site.

Developers
----------
Developers may notice that CRM Core Profile works by taking one or more entities
and putting them together into a form. The module includes objects that can be used
to add support for other entity types. Look in the includes/entity_handlers folder
for examples.

Some notes for anyone considering building their own entity handlers:

1) Every plug-in for CRM Core Profile should inherit ProfileEntityHandler and include the 
   methods defined there. 

2) Every plug in will need to use the hooks listed in the crm_core_profile.api.php file to 
   register itself with CRM Core Profile.
   
Maintainers
-----------

- techsoldaten - http://drupal.org/user/16339
- RoSk0 - https://drupal.org/user/325151

Development sponsored by Trellon.

