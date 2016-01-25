INTRODUCTION
------------

The Social Share Statistics module displays the number of facebook / twitter
 shares by registered drupal users on a particular node.

It creates a block of social media icons.

It has the option to display the total count of all the shares on the page.

The module stores the user ids along with the node ids they share on each
 platform.

A Template File is added inside the module for full flexibilty to manage the
 theming of the output.


REQUIREMENTS
------------
No special requirements


INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

* Configure in Administration >> Configuration >> Social Share Statistics

   - Provide the facebook App ID of your app.

   - Provide your twitter username to add in the tweet (optional)

   - Select the option for displaying total number of shares for a page.

   - Select if you want to show total number of shares for a node.

   - Add Your Total Share Count Text(optional).


* Display the social icons on the page

   - Go to Structure » Blocks and drag "Social Share Statistics Buttons" block
        to where you want to place it.
        (Most commonly in the content region below the main content)

   - Emable the block to see the share buttons on node pages.
