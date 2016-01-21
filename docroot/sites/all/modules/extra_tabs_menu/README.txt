
Extra Tabs Menu
-----------------

    Provides the ability to choose a menu parent item whose links will be added
    to the configured pages as tabs.

Usage
------

    * After installation navigate to the settings page
      (/admin/config/user-interface/extra-tabs).
    * Add a set of paths where you'd like extra tabs to appear.
    * Choose the menu parent whose links you'd like to have added as extra tabs
      on the set of pages you've chosen.
    * Save the configuration by submitting the form.
    * If you'd like to configure different menu parents for a different set of
      paths you can use the additional fields that are added after form
      submission to add more sets of options.

Behavior Notes
---------------

    * When choosing a menu parent in the config form, the parent item that's
      selected will be included in the tab output.
    * Like normal Drupal tabs, there must be more than a single tab for any to
      be displayed. This means that there must be at least a single menu link
      underneath the menu parent chosen on the config form, unless other tabs
      are already being displayed on the page.
    * If the module is configured in such a way that multiple menu parents will
      be added to the same page, all links will be added together as tabs.
