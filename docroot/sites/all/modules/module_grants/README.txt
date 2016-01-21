
DESCRIPTION
===========
This module gets around a quirk in the 7.x core Node module.
Currently the Node module:
- ORs together access grants coming from multiple modules; this results
  in content being made accessible by one module when access had already
  been restricted by another, which is undesirable in most cases.

When Organic Groups, Domain, Content Access modules are used together this module
makes sure that the combination exhibits the expected behaviour: access is
granted to content only when it is in the correct group, domain AND user has the 
appropriate access.

The module_grants module achieves this by AND-ing rather than OR-ing the grants.

INSTALLATION AND CONFIGURATION
==============================
1. Place the "module_grants" folder in your "sites/all/modules" directory.
2. Under Administer >> Site building >> Modules, enable Module Grants.
3. If required, install and enable as many modules for content access control
   as you need for your situation. Typical examples are Organic Groups, Domain
   and Content Access.
