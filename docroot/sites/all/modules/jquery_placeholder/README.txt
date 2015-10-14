
## JQUERY PLACEHOLDER

This module integrates the HTML5 Placeholder jQuery Plugin found here:

  https://github.com/mathiasbynens/jquery-placeholder

with Drupal.


## 1. INSTALLATION

Download jquery.placeholder.min.js from the above repo and place it somewhere
the libraries module will find it. For example:

  sites/all/libraries/jquery.placeholder/jquery.placeholder.min.js

This may be accomplished with the following Drush Make snippet, for example:

  libraries[jquery.placeholder][download][type] = "file"
  libraries[jquery.placeholder][download][url] = "https://raw.github.com/mathiasbynens/jquery-placeholder/master/jquery.placeholder.min.js"
  libraries[jquery.placeholder][directory_name] = "jquery.placeholder"


## 2. USAGE

  RECOMMENDED USAGE: Install the Elements module
  (http://drupal.org/projects/elements) and then simply use the #placeholder
  attribute on your form/render elements. E.g.

    $form['mytextfield'] = array(
      '#type' => 'textfield',
      '#title' => t('My example textfield'),
      '#placeholder' => t('Please enter a value'),
    );

  Then this module will 'just work' ensuring that the placeholder shows even in
  browsers which do not support it.

  ALTERNATIVE USAGE: You can enable the plugin for every text input on an entire
  page by calling the following function on the page(s) you want:

    jquery_placeholder_global().
