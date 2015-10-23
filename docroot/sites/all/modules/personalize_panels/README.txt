INTRODUCTION
------------
This module extends the functionality of the Personalize module, integrating it 
with panels. It provides a new ctools custom content type that allows an editor 
to group existing panes on a panel page into option sets that respond to the 
bidding of any Decision Agent defined with Personalize. In short, it does for 
panels panes what Personalize Blocks does for blocks and Personalize fields does 
for fields. Once a pane is included in an option set, it is removed from the 
display and instead a Personalize Decision Agent becomes the arbiter for whether 
it shows or not. The panes still appear on the panels page admin screen however, 
making it easy for editors to make content changes to their personalized panes.


 * For a full description of the module, visit the sandbox project page:
   https://www.drupal.org/sandbox/michaeldewolf85/2396189


 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2396189


REQUIREMENTS
------------
This module requires the following modules:
 * Panels (https://www.drupal.org/project/panels)
 * Personalize (https://www.drupal.org/project/panels)


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * Note that in order to add personalized content, you will need to configure 
 Personalize and Create a campaign. See:
 https://www.drupal.org/project/personalize

    1. Navigate to any panels admin edit page.
    2. Click the content tab on the left for the page/page variant to 
    personalize.
    3. Choose the region where the personalized set of panes will display, and 
    click the gear in the top left to add content. From the add content modal, 
    select the personalize tab and then select "Personalization Pane".
    4. Select an existing Personalize campaign, and then configure the option 
    set in the table below. Note that each personalization pane creates its own 
    option set. For each option, the panels that are available are pulled from 
    panels already assigned to the page. If an option is missing, it probably 
    has not been added to the page. When done, click "Finish".
    5. Update and save the panels page. Now any panes selected in the options 
    sets from step 5 will not show, instead being passed to personalize. The 
    personalize decision agent for the campaign will then decide what shows. The 
    output of the option set will be rendered on the page in the region/location 
    where the "Personalization Pane" was placed.


 MAINTAINERS
-----------
Current maintainers:
 * Michael DeWolf (mrmikedewolf) - https://drupal.org/user/2679073
