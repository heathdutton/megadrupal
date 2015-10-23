Commerce VAT Included
=====================

This modules solves the problem of impossible vat-included prices.

Problem
-------

XXX Example, Issue link.

Solution
--------

This module changes VAT calculation so that the final price including VAT matches the product price.

Implications
------------

Note that in the example above the resulting VAT is one cent more than it has to be, effectively
donating this cent to your tax authority. This is a feature, not a bug.

Note that tax rounding can lead to un-intuitive results for prices with multiple components.
The final effective tax rate will differ from the applied tax rate due to rounding.
(The math behind: Summation and rounding do not commute.)

Also note that the Commerce VAT module does not yet have configurable rounding but always rounds "half down".
See XXX.


How to install and configure
----------------------------

* Prerequisite: Install either "Commerce Tax" or "Commerce VAT" module otherwise this module will not have any effect.
* Place the module into sites/all/modules directory.
* Enable "Commerce VAT Included (Replacement)" module to change your existing VAT types to have vat-included prices.

For special needs:
* Enable "Commerce VAT Included (Additional)" module to create additional vat-included VAT types.
* Change applicable price components by going to XXX