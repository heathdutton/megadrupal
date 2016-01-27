About Advanced Page
This feature allows a site maintainer to provide editors a way to create and edit static pages on the site, some of the important features include:

    Allows editor to supply per node/page JS and CSS file which will get applied on that node alone.
    Allows editor to create menu's without giving them administer menu permission.

Allow to publish/unpublished a page without giving administer content permission

    Dashboard area to view all static pages.
    Allows editors to create path alias, without giving "create and edit path aliases"
    A configuration page for the site maintainers to show/hide respective fields.

Installation and setup instructions

    Download the recommended release of the module.
    Follow the standard installation procedure to install the Advanced Page module.
    To configure Advanced page module, give “Administer Advanced Page” permission to the appropriate role.
    To view the “Dashboard” area, give the “View Advanced Page List” permission to the appropriate role.

How to Create new static page using Advanced Page

    After installation of the module a new content type “Advanced Page” will be created.
    Other than the standard fields following three fields will be there:


    File attachments
    Embed Javascript
    Embed css


    Put your markup in the body text area and select Ful HTML text format.
    Upload your custom css file using embed css field
    if your design requires any javascript, you can embed it using “Embed javascript” field.
    Save Page



Other Features

    Allows editors to create path alias, without giving "create and edit path aliases"
    Allows editor to create menu's without giving them administer menu permission.
    Allow to publish/unpublished a page without giving administer content permission


Advanced Page Configuration

    The advanced page configuration options can be accessed at admin/config
    Click on the Advanced page menu item.
    Select advanced page settings tab.
    Here following configuration options are available:


    Page url: This option is used to show/hide the path alias field on the advanced page. It also provide a textfield to put the desired path alias prefix for your all static contents.
    Javascript file field: This option is used to show/hide the Embed javascript file filed.
    File Attachment: This option is used to show/hide the File Attachments field. On advanced page. It also provide a text field to add desired file types to be supported by this file attachment field.
    Menu list: This option is used to show/hide Menu settings on the advanced page.


Advanced Page Dashboard
This page can be accessed at admin/config/system/advanced-page. This page lists the static pages created by the advanced page content type. The sitemaintainer can edit or delete the pages using this dashboard.

Advanced Page Permissions:

    Administer Advanced Page: This permission allows the user to access the Advanced Page Configuration page.
    View Advanced Page List: This permission allows the user to access the advanced page dashboard.
