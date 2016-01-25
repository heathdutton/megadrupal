## Overview ##
All state of New York websites are required to have the state Universal
 Navigation bar at the top and bottom of the site, surrounding any other
 content.  The content of the bars are iFrames, this module makes it
 easy to integrate them on a Drupal site.

## Installation and Configuration ##
- Install as usual, see [http://drupal.org/node/70151](http://drupal.org/node/70151)
  for further information.
- Enable the module.
- Go to Configuration >> User Interface >> NYS Universal Navigation
  (/admin/config/user-interface/nys-unav) to configure the module.
  You can also reach the configuration page from the Configure link on the
  module page.
  - The configuration page has two options:
		- Whether to automatically insert the header/footer
		- Whether to use the interactive or static uNav header
  - *Note that if you don't configure the module, it will default to
  automatic insertion and the interactive version of the header.*
- Set the *Administer NYS Universal Navigation* permission for those roles that
  should be able to administer the configuration of this module at People >>
  Permissions (/admin/people/permissions#module-nys_unav).
  You can also reach the permission from the Permissions link on the
  module page.
- The module will automatically insert the Universal Navigation at the top
  and the Universal footer at the bottom of your website's page;
  outside of any page HTML.


Alternatively the module adds two blocks, NYS uNav Header and NYS uNav Footer,
 which can be placed in your theme and it exposes two functions which can be
 used in theme templates.

## Restrictions ##
This Drupal module was developed for use by New York State agencies and
 entities for official New York State websites to be compliant per ITS mandate
 policy NYS-S05-001 ([https://www.its.ny.gov/document/state-common-web-banner](https://www.its.ny.gov/document/state-common-web-banner)).
 For use on other sites, please contact New York State Office of Information
 Services WebNY team at webnysupport@its.ny.gov for guidance and authorization
 for use. The static html `NY State Universal Navigation`, that is integrated
 using this module, can be found at [https://github.com/ny/universal-navigation](https://github.com/ny/universal-navigation).

## Credits ##
This project was sponsored by the [New York State Office of Information
 Technology Services WebNY department](https://www.drupal.org/webny-new-york-state-office-of-information-technology-services).

----------


See DOCUMENTATION.md for additional information.