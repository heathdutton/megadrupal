ADVANCED HELP DIALOG

This module requires the developer implement a hook to get any result 
(see below).

Advanced Help Dialog module provides a hook that lets you put links to advanced 
help topics in the "Help" region of specific paths. When users click these 
links, a ctools-style modal dialog box appears with the appropriate advanced 
help content. Here is an example of how to implement the hook:

<code>
  <?php

  /*
   * Implements hook_advanced_help_dialog_help().
   */
  function modulename_advanced_help_dialog_help() {
    $help = array(
      'admin/config/path' => array(
        'topic' => array( // This is the name of a topic in module modulename
          'title' => t('Topic title'),
          'link_text' => t('Link title'),
          'show_navigation' => FALSE // Or TRUE if you want navigation ..
        )
      )
    );

    return $help;
  }

  ?>
</code>

The snippet above will place a link with the title "Link title" in the help 
region that opens a modal box displaying the advanced help content with name 
"topic" in module "modulename".

Also, this module exposes a new "modal_help_dialog" render element that you can 
use to do the same thing in any render array, like this:

<code>
  <?php

  $form['help'] = array(
    '#type' => 'modal_help_dialog',
    '#module' => 'mymodule',
    '#topic' => 'mytopic',
    '#link_text' => t('Link text')
  );

  ?>
</code>