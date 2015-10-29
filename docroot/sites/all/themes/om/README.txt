===================================================================
Credits:
===================================================================
@author      : Daniel Honrade http://drupal.org/user/351112
               Drupal Theme and Module Developer
               
@sponsored by: Promet Source, Inc. http://www.prometsource.com
               Certified Drupal Company

===================================================================
OM Objectives:
===================================================================
1) Simplicity - The first and foremost objective of this base theme
   is to lessen the work on subtheming while having most commonly
   used functionalities, such as:
   a) the body classes
   b) complete tree of main menus and not just the first level
   c) includes reset.css to maintain cross-browser compatibilities
   d) IE conditional stylesheets
   
2) How things are simplified:
   a) Variable groupings:
      One way of simplifiying the page.tpl.php is to group these
      commonly used variables which help to organize and lessen the
      markups seen on page.tpl.php
      i)   Identity
         - Logo, Site Name Site Slogan
      ii)  Elements
         - Page Title, Tabs
      iii) Regions
         - Content, Side Bar First, Side Bar Second, Footer, Highlighted
      iv)  Menus
         - Primary, Secondary menus
         
      Note: You can still use individual variables in a group.   
         
   b) regions.php
      Another way of simplifiying page.tpl.php is to have this file
      called regions.php, which is just a setting to define the properties 
      of regions, and these are:
      i)   Tags    - container markups
      ii)  ID      - for css id
      iii) Classes - for css classes
      iv)  Inner   - adding inner divs
      v)   Top     - adding div set on top of the region
      vi)  Bottom  - adding div set at the bottom of the region
      vii) Grids   - number of columns for each region, plus 
                     conditional settings of side bar presence
   c) Theme Settings
      All additional theme settings are in the .info file, 
      and these are:
      i)   meta viewport - for mobile browsers
      ii)  960 Grid System - for standardizing the layout dimensions
      iii) offline timer - for site development purposes
      
2) Maintainability
   a) There's not much core overriding or modifications placed in 
      this base theme which basically to be easily updated for future 
      Drupal core versions.
      
   b) This base theme only includes simple functionalities which are 
      also found in other modules, but these functionalities are better 
      placed in the theming layer. These functionalities may change 
      depending on problems or better options in the future.

3) Pro Theming - In Promet Source, we create themes based on bitmap 
   or vector files made specifically for particular client by professional 
   designers. Most of the time if not all the time, we cannot reuse 
   our previously customized subthemes. OM base theme is optimized 
   in that way, that's why it's useless to store any css or other 
   configuration in the database for later exporting purposes.

4) Performance & Design Complexity - OM base theme is a product of 
   years of Drupal theming. It has proven to be easily subthemed based on 
   complex designs. It can be customized to do different layouts on 
   different pages without using panels or display suite modules which
   are both heavy on database queries and html markups. 

5) Up to date with front web development tools - This base theme is 
   actively maintained to support newer systems in front end web development.
   a) HTML5
   b) 960 Grid System
   c) Responsive - using OM Tools for better media queries

===================================================================
OM How To:
===================================================================
1) Subtheming: XHTML or HTML5 
   a) COPY om_html5_subtheme or om_subtheme and RENAME the folder 
      and .info file to your desired name.
      ex. awesome/awesome.info
      
   b) You can PLACE your newly created subtheme outside the om theme
      folder.
      ex. /sites/all/themes/om/
          /sites/all/themes/awesome/
          
   c) You can now modify these files:
      i)   awesome.info  - theme settings
      ii)  page.tpl.php  - for page layout       
      iii) regions.php   - additional region settings
      iv)  logo.png      - your logo
      v)   screenshot    - image for themes list
      vi)  favicon.ico   - browser icon
      vii) template.php  - for preprocess and other functions
      viii)node.tpl.php  - copy from /om/core/tpl/ if needed and doesn't
                           exist in your subtheme folder
      ix)  block.tpl.php - copy from /om/core/tpl/ if needed and doesn't
                           exist in your subtheme folder
      ix)  html.tpl.php  - (D7 only) copy from /om/core/tpl/ if needed and doesn't
                           exist in your subtheme folder
   
      Note: Not all template files are in /om/core/tpl/, but copy om's
      version of these template files if they exist in om's folder
   
   d) You should see now your newly created subtheme in
      Drupal 6 -> /admin/build/themes
      Drupal 7 -> /admin/appearance
      
   e) From here onwards, you should be constantly customizing/revising
      your theme files:
      i)   /sites/all/theme/yourtheme/css/style.css  - custom styling
           
           Note: OM base theme can identify these stylsheets for each
           region with the same name, same with javascripts in /js/ folder.
           This is great for organizing your stylesheets.
           
           /sites/all/theme/yourtheme/css/content.css
           /sites/all/theme/yourtheme/css/sidebar_first.css
           /sites/all/theme/yourtheme/css/sidebar_second.css
           /sites/all/theme/yourtheme/css/footer.css
                                            
      ii)  /sites/all/theme/yourtheme/css/images/    - additional images
      iii) /sites/all/theme/yourtheme/page.tpl.php   - custom layout
      iv)  /sites/all/theme/yourtheme/regions.php    - for region properties
      iv)  /sites/all/theme/yourtheme/yourtheme.info - for additional regions  
   
   f) Clear cache if you have added preprocess functions to your 
      template.php
            
2) Using 960 Grid System
   a) edit yourtheme.info, and look for:
      ; 960 grids, 12, 16, 24 columns
      settings[grid_guide] = on 
      ;settings[grid] = 16 
      
   b) uncomment this line by deleting ';', and use any 960gs (12, 16, 24)
      settings[grid] = 16 
      
   c) edit regions.php, add values to 'grid' => 0, especially to side bar regions,
      this will correct the widths of your main content and side bar regions
      in your layout.
         
      $regions['sidebar_first'] = array(
        'tag'    => 'div',
        'id'     => 'sidebar-first',
        'class'  => '',
        'inner'  => 0,
        'top'    => 0,
        'bottom' => 0,
        'grid'   => 4, 
     ); 
     $regions['sidebar_second'] = array(
       'tag'    => 'div',
       'id'     => 'sidebar-second',
       'class'  => '',
       'inner'  => 0,
       'top'    => 0,
       'bottom' => 0,
       'grid'   => 3, 
     ); 
  
   Note: The main content is automatically resized if these
   side bar regions have grid values. Additional settings information 
   are found in this file
        
3) Offline Timer
   a) edit yourtheme.info file and look for:
      ; this will set the site offline with a timer,
      ; great for underconstruction sites, maintenance
      ; you can set the time of launch or when the site will 
      ; be live again.
      settings[offline][switch] = off
      settings[offline][message] = "We're currently improving this site."
      settings[countdown][year] = 2012
      settings[countdown][month] = 1
      settings[countdown][days] = 1
      settings[countdown][hours] = 0
      settings[countdown][minutes] = 0
      settings[countdown][seconds] = 0
      
   b) change off to on
      settings[offline][switch] = on
   
   c) you can change the default values depending on what
      time, month, year you want your site to be active for
      anonymous users, you can also customize the appearance
      of this page by overriding it by your style.css.


===================================================================
Drupal 7 Update Manager Known Issue
===================================================================
Currently, as long as this issue has not been resolved, you cannot
use the update manager of D7.

http://drupal.org/node/986616 - Update Manager fails when the primary module for a project lives in a subdirectory

Current solution: apply this patch: http://drupal.org/files/986616-56-root_path.patch




