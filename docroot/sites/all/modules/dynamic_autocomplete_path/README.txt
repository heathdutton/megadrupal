This module allows developers to define special values for the
<strong>#autocomplete_path</strong> form API property making possible to use non
static URIs.

<strong>Example:</strong>

If you define this textfield:

<?php
  $form['items'] = array(
    '#title' => t('Items'),
    '#type' => 'textfield',
    '#autocomplete_path' => '/custom_search_callback/#language/#type/',
  );
?>

The #language and #type tokens will be replaced by the values of the elements
with id="language" and id="type" defined in the DOM of the current page.

You will have to provide your own custom callback, this module only simplifies
the proccess to make the uri replacements.

This module is based on ideas found here:

- http://sachachua.com/blog/2011/08/drupal-overriding-drupal-autocompletion-to-pass-more-parameters/
- http://stackoverflow.com/questions/6776661/drupal-autocomplete-callback-with-multiple-parameters/#answer-21853762
- http://www.7sabores.com/blog/sobrescribir-script-auto-completado-drupal-7
- https://www.silviogutierrez.com/blog/changing-drupal-autocomplete-path-javascript/
