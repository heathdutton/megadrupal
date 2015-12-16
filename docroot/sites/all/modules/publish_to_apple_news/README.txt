INTRODUCTION
------------
This module provides Drupal integration with Apple's News platform. It allows you to easily configure the layout of nodes published to News via the Apple News API. When a node is created or updated, it is converted to Apple's new JSON-based Native format with your layout configuration and posted to your channel.

This module allows you to define 


 * For an overview of how to configure the module, check out our blog post at http://projectricochet.com/blog/publishing-drupal-content-apple-news-api


Learn more about News here: https://www.apple.com/news/




 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/publish_to_apple_news
   


REQUIREMENTS
------------
This module requires the following modules:
 * Entity API (https://drupal.org/project/entity)
 * Token (https://drupal.org/project/token)


INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * First, you'll need to visit /admin/config/services/publish-to-apple-news.
 
   - Enter a live base URL, which should be the $base_url of either production site or a reachable development site.
   - Enter at minimum your API key and shared secret. 
   - Enter a default channel ID.
   - Click "Refresh Section ID list" button.
   - Choose a Default section ID.
   - Save the form.
 
 * To get started quickly, create an Apple News article type from a template.
 
   - Visit /admin/structure/publish-to-apple-news/create-template.
   - Select the node type to create the article type for.
   - Select a left, center, or right aligned template and click "Create".
   - That's it! When nodes of this type are created or updated they will be automatically published to Apple News with the article type's configuration.
   - NOTE: These default article types assume your nodes have a field_image that it can use for the thumbnail and header image in the article. If you don't have an image field, follow these steps:
     - Click "Settings" next to the article type at /admin/structure/publish-to-apple-news
     - Remove "[node:field_image]" in the "Thumbnail URL" text box and click "Save".
     - Click "Configure" next to the article type (/admin/structure/publish-to-apple-news) and delete the "Header" component.
     - Click the "Component Layouts" and delete "headerLayout".
     - Click "Component Styles" and delete "headerStyle".
 
 * Configuring an Apple News Article type
  
   - Included in docs/native-format-documentation.zip is Apple New's documentation for the Native format.
   
   1) Create a new article type at /admin/structure/publish-to-apple-news/create.
      - Fill out the required fields.
      - Some fields allow you to enter tokens, which will have their values replaced during the conversion process.
      - Note: Apple *highly* recommends leaving the layout configuration as-is in order to assure your article will be viewable across all iOS devices.
      - Click "Save".
   
   2) Configure your article type's layouts, styles, and text styles.
      - Under the "Configure" section for your article type you will see tabs for "Component Layouts", "Component Styles", and "Component Text Styles".
        - Component Layouts define the layout information for a component.
        - Component Styles define styling properties for a component.
        - Component Text Styles define text styling for a component.
   
   3) Configure your article type's components.
      - Visit /admin/structure/publish-to-apple-news and click "Configure".
      - Under "Add new component", select a component type and click "Add".
      - Fill out the fields for the component, optionally relating it to a component layout, component style, and component text style (if applicable).
      - Click "Save".
      - If the component you are editing accepts other components (header, section, and chapter), you'll see an "Add Components" button. Components are infinitely nestable.
      - You can re-order components within their parent by dragging and dropping the component fieldset's title. The new order is saved instantly via AJAX.
      
   4) Test it out!
      - Included in this module is a handy testing function that will take a node and generate the Apple News article JSON text.
      - Install and enable the devel module to use this function.
      - Visit /admin/structure/publish-to-apple-news/test.
      - Choose a node and click "Generate".
      - Take the outputted JSON and place it in a file called article.json, in a folder on your local machine.
      - Copy any images from the node into the folder.
      - Drag the folder into the Apple News Preview app.
      
   4) That's it! Now, when you add or update a node of a type mapped to an article type, the article will be instantly published to Apple News.
