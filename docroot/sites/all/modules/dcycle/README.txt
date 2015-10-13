Dcycle
======

Intro
-----

Dcycle is meant to help you enforce best practices and get team members up and running fast(er) with automated testing and continuous integration.

Specifically, it is meant to help enforce the practices outlined in the [Dcycle manifesto](http://dcycleproject.org/manifesto).

This module is still under active development; the API, code and features are usable but can change until the beta release. Please use with caution.

Dcycle is meant to be used with `drush` on the command line, and will be most useful when used in conjunction with a continuous integration server.

Example usage: metrics
-------------

Dcycle contains several metrics collectors, for example DcycleCoder, which runs code review. Code review can be run from the command-line, but Dcycle takes it a step further and saves the output as an artifact, and parses the output to create a graph:

 * Enable Dcycle and [Coder](http://drupal.org/project/coder)'s `code_review` module.
 * Run `drush dcycle-check DcycleCoder`
 * Coder will be run, errors will be suppressed, and the output will be placed in public:://ci/artifacts/DcycleCoder; metrics will be placed in public://ci/metrics/DcycleCoder.csv
 * When used in conjunction with [Jenkins](https://jenkins-ci.org) and the [Jenkins plot plugin](https://wiki.jenkins-ci.org/display/JENKINS/Plot+Plugin), you can collect coder review metrics for each build, and graph your code's quality over time.

Experimental features
---------------

`drush dcycle-test` is an experimental feature which is under development (not yet stable). The idea is that for each build tests can include:

 * Artifacts (like the result from coder review)
 * Metrics (various CSV files which can be converted into graphs)
 * Thresholds (for example 100 coder review errors is OK, but not 101, or 0 test failures is OK, but not 1). This allows for some fuzziness when running CI jobs.

The current plan is for your [site deployment module](http://dcycleproject.org/blog/44/what-site-deployment-module) to include a file, `mysite_deploy.dcycle.inc`, which will contain the "recipe" for what should make your site fail, and what kinds of metrics you want to collect.

Sponsors
-----

The following organizations have sponsored development of this module:

 * [The Linux Foundation](http://www.linuxfoundation.org/) (current)
 * [CGI](http://www.cgi.com/en)
 * [TP1](http://tp1.ca) (initial development)
