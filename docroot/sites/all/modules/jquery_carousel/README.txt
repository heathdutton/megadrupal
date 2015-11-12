jQuery carousel is based on the jquery plugin
http://github.com/richardscarrott/jquery-ui-carousel. It allows developers to
create carousels using the data entered through the content types. The carousels
could be vertical or Horizontal depending on the configurations. These carousels
can be configured for "swipe to slide" on mobile devices as well. Also, the
module allows us to have "multiple carousels" with different configurations on a
single page as it ties up the config for a carousel to its instance only
rather than having it as a global config for the page.

Demo:
  http://jcarousel.qed42.webfactional.com/jquery-carousel-demo

Features:
  1. Views style Plugin: This module provides a views style plugin to display
  the view-rows in the form of a carousel.

  2. Field Formatter Plugin: This module provides a field formatter plugin for
  image field. Displays the multi-valued content as a carousel.

  3. hook_carousel_theme_info(): This hooks allows developers to register a new
  theme for the carousel and load the corresponding CSS file. e.g.,

    /**
     * Implements hook_carousel_them_info()
     */
    function mymodule_carousel_theme_info() {
      $themes = array();

      $themes['my_custom_theme'] = array(
        'title' => t('My Custom Theme'),
        'file' => 'my_module/my_custom_theme/jquery-carousel-default.css'
      );

      return $themes;
    }

  4. Configurable vertical & Horizontal carousels.
  5. Touch support for mobile devices
  6. Can easily place multiple carousels with different configurations on a
     single page. Carousel configurations are tied up with carousels
     and not the page.

Requirements:
  This module depends on jquery ui carousel
  library(http://github.com/richardscarrott/jquery-ui-carousel).

Installation:
  1. Download the module and place it with other contributed modules
     (e.g. sites/all/modules/contrib).
  2. Enable the jQuery Carousel module on the Modules list page.
  3. Download the jquery ui carousel library form
     http://github.com/richardscarrott/jquery-ui-carousel and save it in
     sites/all/libraries folder.
  4. Goto admin/reports/status & make sure jquery ui carousel is not throwing
     errors.

Usage:
  1. Views Style Plugin:
    a. Goto admin/structure/views. Click on "Add new view"
    b. Create a view with "page" display. Add fields you need to be displayed
       in a carousel. This would mostly be images. Add images with
       appropriate "image presets".
    c. Change the "Format" to jQuery Carousel and click on "Apply this display".
    d. Configure the plugin in the settings form that pops up.
    e. Save the view and visit the path set for the view while creating the page
       display.
    f. You should be able to see the data requested in the view in the form of
       a carousel.

  2. Field Formatter Plugin:
    a. Goto admin/structure/types. You should be able to see article content
       type(ships with an image field).
    b. Click on Manage display.
    c. Under format select list, choose jQuery Carousel. You will see the same
       settings form as you see for the views style plugin. Configure it as per
       the requirements.
    d. Goto node/add/article. Create an article with multiple images uploaded.
    e. The view page for the article should show the multiple values in a
       carousel.

  NOTE:
     a. Module ships with only one default theme named as default.
        You can create as many themes as needed using the hook
        explained under Features#3.
     b. You must add a selector value in the selector field coming up in
        settings form. This is the class that the carousel's markup gets wrapped
        with & the configs done get applied to.
