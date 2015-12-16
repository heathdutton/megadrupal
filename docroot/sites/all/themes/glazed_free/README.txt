Compass-based subtheme of https://drupal.org/project/bootstrap

Based on Thomas McDonald's https://github.com/thomas-mcdonald/bootstrap-sass/blob/3/docs/COMPASS.md

Info on standard Bootstrap subtheme: http://drupal.org/node/1978010

Dependancies: https://drupal.org/project/bootstrap, bootstrap-sass, compass, jquery_update (1.7 enabled)

Installation notes:
	Download this theme along with the 2.x bootstrap theme
	Download and enable jquery_update
	admin/config/development/jquery_update, set to jquery 1.7

This project is meant to serve as a shortcut for setting up a Bootstrap subtheme and using Compass SASS instead of LESS. This should decrease the complexity of setting up a subtheme, and relies on fewer modules (you do not need the lessphp library, nor the less module).

Once you've got this theme installed along with the core Bootstrap theme, simply navigate to the compass-project directory and run '$ compass watch' from the command line (or use any other SASS compiling tool of your choice).

In the near future, I plan to branch this for the 3.x branch of Bootstrap, but am waiting until it becomes more stable - and it looks like it will happen relatively quickly.