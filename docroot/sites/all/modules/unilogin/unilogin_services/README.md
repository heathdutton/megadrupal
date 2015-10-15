UNI•Login Services module for Drupal
===========================

This module connects with the various UNI•Login services.

The module is meant as a tool for developers and does not provide
out-of-the-box site functionality.

How does it work?
-----------------
 
### Following request are supported:

**unilogin_services_w02_load:**

    $info = unilogin_services_w02_load(
      $unilogin_id,
      array('hentPerson', 'hentInstitutionsliste')
    );

*[Read about w02](http://www.uni-c.dk/produkter/infrastruktur/uni-login/infotjenestenswebservice_ws02.pdf)*


Install instructions
--------------------

1.  Fill in settings provided by UNI•Login in the UNI•Login Services configuration form.
2.  Use one of the request supported by this module in your code. :)
