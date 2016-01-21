CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* API

INTRODUCTION
------------

By default Drupal provides ability to describe foreign keys through the
Schema API, but it doesn't create them on database physical level.
So data integrity could be corrupted.

This module checks database for data integrity issues.

INSTALLATION
------------

1 Download and install this module.
2 Open page: Reports -> Data integrity (admin/reports/di).

API
-----

Module provides hook_di_checker_info() hook, so other modules could
provide their data integriry checks which will be shown on Data integriry report
page. See di_checker.api.php for details.
