GitHub Pages (ghpages)
======================
Module allowing users to create a HTML version of a node and then push it to
GitHub's free Pages hosting.

Adapted from Kevin O'Brien's SaveToFTP module:
http://drupal.org/project/savetoftp

Drush support based on Colorbox's:
http://drupal.org/project/colorbox

Inspired by the awesomeness of Mike Bostock, d3.js creator.
http://www.bost.ocks.org

Download
---------
http://drupal.org/project/ghpages

Installation
------------
Untar or unzip the downloaded module to your module directory
(e.g. sites/all/modules) and then enable on the admin modules page
(/admin/modules).

Usage
------
1. Configure at /admin/config/media/ghpages.
Ensure that the repo you plan to push to has a "gh-pages" branch!

2. Go to a node's edit page, e.g., node/edit/1.
At the bottom there will be a "Push to GitHub" button

3. Everything should then be visible at http://username.github.com/repo_name

Requirements
------------
+ A GitHub account -- can be public or private
+ A repository on that account with a branch named "gh-pages"
+ PHP Version > 5.1.2
+ QueryPath module (http://drupal.org/project/querypath)
+ Libraries module (http://drupal.org/project/libraries)
+ Git.php library extracted into sites/all/libraries/Git.php/
    + Download from: https://github.com/kbjr/Git.php/
    + Alternately, use Drush: drush gitphp

ToDos
-----
+ Add "Remove from GitHub button."
+ Add user-level config options (I.e., user repos)
+ Integrate with GitHub Connector
