Overview:
This is a small module which marks the HTML elements 'read' or 'unread' 
based on the CSS selector provided on the admin. It uses database to 
track the user click activity as opposed to the cookies. This can be used 
with any html elements, but requires to have the unique attribute value 
which represents that html element uniquely.

Common Use Case:
Email client unbolds or changes display properties when unread email is 
clicked or read.

Example User Story:
As a logged in user I should be able to identify whether I clicked the 
link in the folder tree/menu list or not. It should persist that behavior 
forever.

Important:
While making a ajax request it uses 'async' (Asynchronous) as false which 
creates slight delay before redirecting pages, or any other browser activities
when unread element is clicked. Its done to make sure ajax request has 
completed especially when html element is hyperlink.

Installation and Usage:
Once module is installed
Goto : admin/config/user-interface/mark_as_read
Then it is required to add following fields on the admin to make it work 
on the client side.

Css Selector:
Selector that refers to all the links/html elements on which click event 
is tracked and marked read/unread respectively.
For eg: If the links are under Main Menu then it should be like 
".main-menu li a" . Now this will attach click events on all the anchor tag 
that is represented by above css selector.

Attribute Name: 
This is attribute name which uniquely represents html element provided on 
the CSS selector field. In most of the cases "href" can be used with anchor 
tag. We can have custom attribute too like "data_id" for 
"<span data-id=test_id_123>test data </span>". In short, this attribute 
should uniquely represent the html element provided on the Css Selector Section

Features:
All the admin list entries are exportable by Features module.
It marks any html element as read/unread(clicked/non-clicked) as 
specified in the admin.
We can have multiple number of such list/items in the single page.
For optimization purpose Javascript can only be rendered on specific 
pages as specified in the admin settings.
