/**
 * Rut
 */
This module defines a new element for RUN or RUT, these are unique numbers
assigned to natural or legal persons of Chile.

Example
You can use it on your custom forms like this:

<?php
...
  $form['rut_client'] = array(
    '#type' => 'rut_field',
    '#title' => t('Rut'),
    '#required' => TRUE,
  );
...
?>

That was easy. With this you can forget validate the RUT because this element
will do it for you!.
Can also validate the RUT on the client side like this:
<?php
  $form['rut_client'] = array(
    '#type' => 'rut_field',
    '#title' => t('Rut'),
    '#required' => TRUE,
    '#validate_js' => TRUE,
  );
?>

For this, the module uses the jquery plugin Rut created by José Joaquín Núñez.
Website: http://joaquinnunez.cl/jQueryRutPlugin/

/**
 * Additional modules
 */
This module has a submodule to implements the rut like a field and this have
integration with the devel generator.


