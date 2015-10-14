-- SUMMARY --

Bongo (http://ebongo.org) is a GPS-based, real-time passenger information system
that allows riders to find current bus locations as well as predictions for
upcoming bus arrivals for Iowa City, Coralville and the University of Iowa.

The Bongo module provides functions that interact with the Bongo API
(http://api.ebongo.org). It does not really do anything on it's own, but you may
build upon it to create displays of Bongo data on your Drupal site.

A Bongo Panes module is provided as part of this project which provides a Ctools
content type for Bongo Predictions.

All of the data for Bongo is copyrighted by the University of Iowa, the City of
Iowa City and/or the City of Coralville and is not permitted to be used in a
commercial application.


-- REQUIREMENTS --

* A Bongo API key. If you do not have one, apply for one here:
http://lb.cm/mNo


-- INSTALLATION --

* Install as usual, see
  http://drupal.org/documentation/install/modules-themes/modules-7


-- CONFIGURATION --

* Go to Administration » People » Permissions and give proper roles the
  "Administer Bongo" permission. This permissions allows access to set the
  Bongo API key.

* Go to Administration » Configuration » Web Services » Bongo - Bus on the Go,
  and enter your Bongo API Key.


-- SUPPORT --

There is a Bongo API community on Google Plus
(https://plus.google.com/communities/102129876377787257184)

Current maintainers:
* Brandon Neil (bneil) - http://drupal.org/user/54136
