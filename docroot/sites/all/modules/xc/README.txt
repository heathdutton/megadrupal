

INTALLATION Guide for the Drupal Toolkit installation package

For more information, please consult the Drupal Handbook pages like
http://drupal.org/requirements.

We will give you information about installing Apache, MySQL, and PHP independently,
however you should know that there exist a concept named as LAMP
(Linux-Apache-MySQL-PHP/Perl), which covers an intallation package with
which you can install these tools in one step. You can find more information
about LAMP, and its Windows (WAMP), MAC OS (MAMP), Solaris (SAMP) and other
siblings at http://en.wikipedia.org/wiki/LAMP_%28software_bundle%29, and
http://en.wikipedia.org/wiki/List_of_AMP_packages (list of *AMPs).

Install Apache HTTP server
--------------------------
.1 Download Apache from http://httpd.apache.org/download.cgi. Choose one of the
   Win32 Binary distribution of  2.2.xx. (at the time of writing the actual
   version is 2.2.14)
   If you already have Apache HTTP server, you can check the version in the
   CHANGES.txt file in the root directory of Apache web server.

.2 It comes with an installer. Run it, and follow the steps.

.3 We suggest to uncomment mod_rewrite to use clean URL.
   3.1 open conf/httpd.conf
   3.2 find the following line:
   #LoadModule rewrite_module modules/mod_rewrite.so
   3.3 remove the hashmark, so the line will be as following:
   LoadModule rewrite_module modules/mod_rewrite.so

.4 You should allow that the .htaccess file of the would-be Drupal’s
   root directory should be readable. The .htaccess file will add more
   site specific settings to Drupal, and it allows the clean URLs (like:
   http://[sitename]/drupal/node/1 instead of http://[sitename]/drupal?q=node/1)
   and other things.

   4.1 open open conf/httpd.conf
   4.2 find the following passage:
<Directory "C:/Program Files/Apache Software Foundation/Apache2.2/htdocs">
    Options Indexes FollowSymLinks
    AllowOverride All
    Order allow,deny
    Allow from all
</Directory>

   It is possible, that there are multiple lines commented out (they are beginning with
   hashmark ('#') character) between the opening and closing <Directory> tags.
   What is important here, that the AllowOverride directive should be set to 'All',
   and not 'None'.
   This settings make possible to use .htaccess files from every subdirectory
   of the Apache default web directory. If you want to use it with the Drupal
   site, after downloading and extracting Drupal, you should copy the original passage,
   and modify that the Directory should be fit to the Drupal directory, e.g.

<Directory "C:/Program Files/Apache Software Foundation/Apache2.2/htdocs/drupal-xc">
    Options Indexes FollowSymLinks
    AllowOverride All
    Order allow,deny
    Allow from all
</Directory>

You can find more information all about this settings in Clean URLs manual page
of Drupal Installation Guide at http://drupal.org/node/15365.

Install Java
--------------------------
Solr is a Java-based application, so you need to install Java, if you don't have it.
Solr require Java 1.5 or greater. I reccommend the JDK (development kit) instead of JRE
(running environment). At the time of writing you can download it from
http://java.sun.com/javase/downloads/widget/jdk6.jsp
The Java comes with an installer, so you should only follow the steps.

Install Solr
--------------------------
You can find more information at http://lucene.apache.org/solr/tutorial.html#Overview

.1 Download Solr 1.4. Go to http://www.apache.org/dyn/closer.cgi/lucene/solr/,
   choose a mirror site, choose 1.4.0 directory, then dowload apache-solr-1.4.0.zip

.2 Unpack it to any convenient place, like c:\Program Files\Solr. We will call it
   [solr] in this document.

.3 Copy the schema.xml and solrconfig.xml from xc_solr/resources to Solr
   subdirectory [solr]/example/solr/conf

.4 run the server:
   go to [solr]/example
   type the following line:
   java  -Xms512M -Xmx1024M -server -jar start.jar
   If you have JRE instead of JDK, ignore the -server option.
   If you have less or more memory, or if you want to make some test with different
   settings, you can modify the minimum (-Xms) and maximum (-Xmx) memory allocation
   options.

.5 Go to the solr admin page to verify that the installation is working. It will be at
   http://localhost:8983/solr/admin

Note: later in this document in the Drupal section we will use this standard Solr
port (8983). If you will use the following settings, remember, that you will have
different port.

Optional settings:
The standard Solr comes with Jetty servlet container (servlet container is a special
web server for Java web applications). But there is a problem with
Jetty: it is not an easy task to run Jetty as service on Windows platform. So if you
want that Solr should start when you start your Windows box, we suggest you to
use Solr with Tomcat servlet container. Since you have downloaded Solr, you should
take the following steps:

.1 Download Tomcat 6 from http://tomcat.apache.org/download-60.cgi. You can select
   two versions: the zip and the Windows Service Installer. The later is an ordinary
   windows-like installer.
   .1 If you choose to install the Windows Service Installer, when installing,
       please select Tomcat/Service for installing Tomcat as service and Tomcat/Native
       checkboxes to enhance performance. If you accept the default values, the
       Tomcat will be installed to C:\Program Files\Apache Software Foundation\Tomcat 6.0\
   .2 If you choose zip version, you should extract it to a convenient place, say
       C:\Program Files\Apache Software Foundation\Tomcat 6.0\.
       .1 You should edit the [tomcat]/conf/tomcat-users.xml file, and add the
          following lines:
          <role rolename="manager"/>
          <user username="admin" password="secr3t" roles="manager"/>
       .2 Install windows service
          go to [tomcat]/bin directory
          type the following line to a command prompt:
          service install Solr
          (you can use any other service name instead of Solr)

.2 Check if Tomcat is installed correctly by going to http://localhost:8080/

.3 Check if Tomcat is installed as service by going Start > My machine >
   right mouse click > Handling > Services and servers > Services
   Sort the list by name (clicking on the 'Name' column) and search Apache Tomcat 6
   or Apache Tomcat Solr.
   Alternatively you can type: %SystemRoot%\system32\services.msc /s
   Check if the 'Launch type' column is set to Automatic. If not, click on the row,
   and set it to Automatic.

.4 Change the [tomcat]/conf/server.xml file to add the URIEncoding Connector
   element as following:
   <Server ...>
     <Service ...>
       <Connector ... URIEncoding="UTF-8"/>
        ...
       </Connector>
     </Service>
   </Server>
   Note, that there are multiple <Connector> elements, so you should take care
   of all instances!

.5 Stop the Tomcat service

.6 Copy the *solr*.war file from [solr]/dist to the Tomcat webapps directory
   [tomcat]/webapps

.7 Rename the *solr*.war file solr.war

.8 Configure Tomcat to recognise the solr home directory you created, by adding
   the Java Option -Dsolr.solr.home=[solr]\example\solr
   If you installed Tomcat with Windows installer, you can use the system tray
   icon to add the java option at Java tab.
   Or manually edit the environment script [tomcat]\bin\setenv.bat and add it
   to JAVA_OPTS

.9 Start the Tomcat service

.10 Go to the solr admin page to verify that the installation is working. It will be at
    http://localhost:8080/solr/admin

Note: later in this document in the Drupal section we will use this standard Solr
port (8983), and not this 8080 port number. If use this settings settings, remember,
that you have different port.

For Mac users.
By the time of writing we haven't tested any elements of Drupal Toolkit on Mac. You
can find a good introduction of how to set programs running automatically at startup
here: http://developer.apple.com/macosx/launchd.html

Install MySQL
--------------------------
Drupal supports the MySQL 5.0 community server. You can download it from
http://dev.mysql.com/downloads/mysql/5.0.html. You can choose the Windows Essentials
or Windows ZIP/Setup.EXE. Both have an installer which guide through the process.
We suggest you to use UTF-8 encoding (multi language option), and the default settings.

Install PHP
--------------------------
You should enable these extensions in PHP.ini:
mysql (the original MySQL extension),
mysqli (an improved connector for newer MySQL installations),

Other settings in php.ini:
session.save_handler = user
error_reporting = E_ALL & ~E_NOTICE
safe_mode = off
session.cache_limiter = nocache

You can read more about this settings in the PHP.ini comment sections.

It is possible, that some Drupal process will run longer than the default 30 seconds.
If after some long run you run into a blank screen, it is possible, that the script
exceeds the available execution time. The cause of the blank screen is usually
logged inside Apache error log: [apache]/logs/error.log.

You can increase the execution time in this valiable:
max_execution_time = 60;

Another problem could be the maximum amount of memory a script may consume. The default
is usually less than the optimal for Drupal. We suggest to use 16 or 32 MB.
memory_limit = 32M;

These numbers are global for any PHP applications. If you want to restrict these
to the Drupal intance, you can edit the .htaccess file inside root of Drupal
directory. Some of the php.ini file’s variables are not changeable elsewhere,
please consult the table available at http://php.net/manual/pl/ini.list.php.
For the meaning of the column "Changeable" see
http://php.net/manual/en/configuration.changes.modes.php.

Usually PHP configure the Apache httpd.conf, but if it doesn't do,
write the following lines at the end of the Apache http.conf (conf/httpd.conf):

ScriptAlias /php/ "c:/Program Files/PHP/"
Action application/x-httpd-php "c:/Program Files/PHP/php-cgi.exe"
PHPIniDir "c:/Program Files/PHP/"
LoadModule php5_module "c:/Program Files/PHP/php5apache2_2.dll"
LoadModule php5_module "c:/Program Files/PHP/php5apache2.dll"
LoadModule php5_module "c:/Program Files/PHP/php5apache.dll"

(of course you should replace c:/Program Files/PHP/ if you install PHP elsewhere).

And for the sake of the easiness, you can modify the following lines
<IfModule dir_module>
    DirectoryIndex index.html
</IfModule>

to the following:
<IfModule dir_module>
    DirectoryIndex index.html index.php
</IfModule>

This will automatically open not just index.html, but index.php (if exists)
in any directory if you call the directory URL from the browser.


Install Drupal
--------------------------
.1 Download drupal-xc-6.14.tar.gz from DocuShare
   You can fint it in http://docushare.lib.rochester.edu/docushare/dsweb/View/Collection-6130

.2 Deflate it and copy the resulting directory to the Apache web directory.
   On linux it is /var/www or /var/www/htdocs.
   On Windows it is c:\Program Files\Apache Software Foundation\Apache2.2\htdocs

   We suggest you to use Total Commander (on Win32) or Krusader (on Ubuntu) file
   manager, which helps you in packaging, copying, renameing and other file operations.
   If you don't have this tool, on linux you can use this command:

   tar -zxvpf drupal-xc-6.14.tar.gz

   There are some tips for different OS to unpack and place the file:
   http://drupal.org/getting-started/6/install/download

.3 Set permissions
   The you should copy sites/default/default.settings.php to sites/default/settings.php
   (inside Drupal directory), and make it writable during the installation process. It
   practically means, that when you copy this file, you should make it writable,
   and at the end of install, you should set back to readable.
   On linux use this pair of commands:

   chmod a+w sites/default/settings.php
   chmod a-w sites/default/settings.php

   On Windows you should click on file permissions form.

   Later, during the installation Drupal will try to create a new directory:
   sites/default/files. In some configuration it is possible, that it can not do,
   and ask you to create this directory manually. To prevent this, it is
   advisable to add proper permissions to the sites/default directory before
   starting the installation:

   chmod o+w sites/default

   More info: http://drupal.org/getting-started/6/install/set-permissions

.4 Create the MySQL database
   (See more details at http://drupal.org/getting-started/6/install/create-database)
   Launch mysql client. Type this into a command line: > mysql -uroot -p
   If you choose a different user then root, use that.
   MySQL will ask for a password.

   After you are inside, create the database:
   > CREATE DATABASE drupal

   Then create a user. Issue this command but replace the 'username' and 'password' with
   real values:
   > GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, INDEX, ALTER ON drupal.* TO 'username'@'localhost' IDENTIFIED BY 'password';

   If you have installed another Drupal version, use the drupal-xc as name.

   Then you should activate the priviledges:
   > FLUSH PRIVILEGES;
   Ok, it finished, you can quit from the client.
   > \q

.5 Run the installation process
   Go to http://localhost/drupal-xc
   You should give the name of the database name, the user, who has priviledge to modify
   the MySQL database, and its password (the same values, you gave at 'GRANT ...'
   comand.
   We provide basic data for the Drupal site. You can overwrite those. The Drupal user
   will be 'admin', and its password is 'xc'. You can overwrite these data, but please
   remember it.
   You should select whether the toolkit use a caching mechanism or not. If it use,
   then all OAI response will be stored, and the next time it will be served from
   the cache. We populate the cache with the 90 000 records MST responses, so it
   eliminates the HTTP request.

In the next parts of this manual we will refer the Drupal path in the
following form: admin/build/modules.
The means - depending to your overall Drupal and Clean URL settings -
http://yoursitename/drupal/admin/build/modules,
http://yoursitename/admin/build/modules,
http://yoursitename/drupal/?q=admin/build/modules, or
http://yoursitename/?q=admin/build/modules.

Using Drupal Toolkit
--------------------------
.1 First you should harvest some data.
   Go to OAI Harvester > Scheduled harvets > MST 90, and click 'Harvest'
   After the harvest finished, it will print out some statistics to the screen.
.2 Normalize the index
   Go to eXtensible Catalog > Solr > de-FRBR-ize index
   This takes a little while (1-2 mins) it made the normalization.
.3 Search something
   Write a query to the Search block on the left column.
   Click search.
   When the search result displayed, click on 'Search XC records'

Install Drupal Toolkit if you have an existing Drupal installed
--------------------------
If you haven't have sites/all/modules directory under Drupal, please,
create it first.

- download the xc package from http://drupal.org/project/xc
- extract to sites/all/modules
- install modules in the following order:
-- XC Core (xc)
-- XC Metadata (xc_metadata)
-- XC Utility (xc_util)
-- XC SQL Database Storage (xc_sql)
-- XC Schema (xc_schema)
-- OAI DC Schema (xc_oai_dc)
-- NCIP Integration (ncip)
-- XC NCIP Integration (xc_ncip_provider)
-- OAI Harvester (oaiharvester)
-- XC OAI Harvester Bridge (xc_oaiharvester_bridge)
-- XC Solr (xc_solr)
-- ILS (xc_ils)
-- XC Search (xc_search)
-- XC external services framework API (xc_external)
-- Syndetics Solution (syndetics)
-- EZProxy URL Rewrite (ezproxy_url_rewrite)
-- XC Authentication (xc_auth)
-- XC Browse (xc_browse)
-- XC Account
-- XC Metadata Import and Export (xc_import_export)

To install the modules:
1. go to Administer > Site building > Modules (admin/build/modules).
2. click on the all checkbox left of the Module name under eXtensible
   Catalog (XC) System box
3. click on 'Save configuration' button at the bottom of the page.


Setup Drupal Toolkit if you have an existing Drupal installed
--------------------------

- create storage
-- go to eXtensible Catalog (XC) > Metadata > Storage locations > Add location
   (admin/config/xc/metadata/location/add)
-- fill the form:
Name: default
Description: default location
-- click Continue
-- accept everything as is if you have the standard Solr installation (Jetty servlet
   containe runs on port 8983). If you setup Solr inside Tomcat, use the Tomcat
   server’s port number (default is 8080). In any cases you changed either the
   host name or the port numbers of the servlet container (we did not discussed
   this topics in this documentation), remember and apply your settings in this form.
-- click Add storage location

- setup NCIP
-- go to NCIP > NCIP providers > Add NCIP provider (admin/config/xc/ncip/provider/add)
-- fill the form:
Name: Rush Rhees Library
Description: University of Rochester, Rush Rhees Library, NCIP provider
Host: 128.151.244.137
Port: 8080
Path: /NCIPToolkit/
To Agency Id
Scheme: http://128.151.244.137:8080/NCIPToolkit/NCIPschemes/AgencyScheme.scm
Value: University of Rochester
-- click Add NCIP provider

- register an OAI-PMH repository
-- go to OAI Harvester > Repositories > Add Repository (admin/config/xc/harvester/repository/add)
-- fill the form:
Name: MST 90
Url:  http://128.151.244.133:8080/MST/MARCToXCTransformation-Service/oaiRepository
-- click Submit and validate

- setup ILS (this connects NCIP instances to field in a metadata record)
-- go to eXtensible Catalog (XC) > ILS settings > Add new ILS settings (admin/config/xc/ils/add)
-- fill the form:
OAI provider: MST 90 (this field will be deleted soon)
NCIP server: Rush Rhees Library
Bib identifier field: xc:recordID_NRU

- create a scheduled harvest
-- go to OAI Harvester > Scheduled harvests > Add scheduled harvest (admin/config/xc/harvester/schedule/step1)
-- fill the form:
Select repository: MST 90 (default, and only one)
Schedule: weekly
-- click Next
-- fill the form:
Choose Metadata Format: xc
Choose Parsing Mode: Regular expression based
-- Click Submit

Since the next steps require Solr running, be sure, that you started Solr.

- launch scheduled harvest manually
-- Go to OAI Harvester > Scheduled harvests > MST 90 - Weekly (admin/config/xc/harvester/schedule/[x]/start)
   (where [x] is a number refering to the ID of the harvester. If you follow this
   guide, the identifier will be 1, so the path will be admin/config/xc/harvester/schedule/1/start)
-- click Harvest
It will take for a while.

- re-structure index
-- Go to eXtensible Catalog (XC) > Solr > de-FRBR-ize index (admin/config/xc/solr/defrbrize_index)

The metadata are stored in the following tables:
 - xc_entity_properties
 - xc_entity_relationships
 - xc_sql_metadata
 - node
 - node_revisions
 - node_comment_statistics

ignore this - ben was here
