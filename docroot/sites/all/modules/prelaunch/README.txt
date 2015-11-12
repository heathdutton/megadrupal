This module is used to set one page of the drupal site as prelaunch page. It should be used when you are still working on your site
but you want to give a preview to your users. For example you want to display a webform to collect emails for a trial.

As an admin you can give rights to roles to bypass this. You can set which page is the only page.

How is this different from maintenance mode?
* Maintenance mode puts the site offline. With prelaunch the site is not offline. 
* In maintenance mode Drupal prints a 503 (drupal_add_http_header('Status', '503 Service unavailable');) Prelaunch gives a 200.
* Maintenance mode allows the user to still see parts of the site when surfing to /user. 
  You can see the menu structure, footer and other blocks present. Prelaunch shows a bare form.

Why use prelaunch module?
* If you want an easy way to put one page of your site online.
* You don't want anyone to see any portions of the new site except this one page before it launches.
* You dont want to use maintenance mode as this puts your site offline and displays a 503.