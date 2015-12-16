-- SUMMARY --

The USAJobs module provides a block to display all opening jobs for a specific
federal, state or local agency. Data source comes from Government Jobs API
which is collecting across federal governments and includes all current openings
posted on USAJobs.gov.

For a full description of the module, visit the project page:
  https://drupal.org/sandbox/axlrose/2225805

To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/2225805


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure USAJobs listing block in in Administration >> Structure >> Blocks:

  - Enter appropriate Organization ID for your organization.

    For federal agencies, the ID is based on USAJobs' agency schema.

    Two letter codes are used to span entire departments, while four letter
    codes are generally used for independent agencies or agencies within
    a department.

    For state and local agencies, a sample of the format follows.

    State of Virginia US-VA
    State of Virginia Department of Taxation US-VA:DEPT-TAX
    Fairfax County, VA US-VA:COUNTY-FAIRFAX
    Fairfax County Sheriff US-VA:COUNTY-FAIRFAX:SHERIFF
    City of Fairfax, VA US-VA:COUNTY-FAIRFAX:CITY-FAIRFAX

-- CUSTOM TEMPLATE --
* Override USAJobs template
 - Copy file usajobs-item.tpl.php to your theme folder.
 - Clear caches.
