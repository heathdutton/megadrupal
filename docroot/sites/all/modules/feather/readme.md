Feather is a light-weight script for running an Apache http server in a 
development environment with minimal configuration.  Feather maintains an 
isolated set of Apache configuration files, optimized to run a minimal httpd 
server.  These configuration files are completely managed by Feather, so gone 
are the days of manually setting up virtual hosts for every new development site.  

How is this different from _drush run-server_?  The key difference is that 
Feather runs Apache, so it provides a development environment that is more 
similar to most production environments.

### Drush commands provided by feather:

<dl>
<dt>feather-add</dt>
<dd>Add a virtual host configuration.</dd>

<dt>feather-remove</dt>
<dd>Remove a virtual host configuration.</dd>

<dt>feather-list</dt>
<dd>Display all configured virtual hosts.</dd>

<dt>feather-status</dt>
<dd>Display the current status (running/not running) of the server.</dd>

<dt>feather-start</dt>
<dd>Start the web server.</dd>

<dt>feather-stop</dt>
<dd>Stop the web server.</dd>

<dt>feather-restart</dt>
<dd>Restart the web server.</dd>

<dt>feather-check-config</dt>
<dd>Check the syntax of the generated configuration files.</dd>

<dt>feather-error-log</dt>
<dd>Display the error log using `tail`.</dd>

<dt>feather-install</dt>
<dd>Install the "feather" symlink.</dd>
</dl>
