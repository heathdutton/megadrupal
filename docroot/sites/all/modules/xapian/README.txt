= Xapian integration =

== Overview ==

This project provides two modules that integrate with
http://xapian.org[Xapian]:

xapian::
Implements Drupal core search API for Xapian search library.

search_api_xapian::
Implements http://drupal.org/project/search_api[Search API project] API
for Xapian search library.

Both modules use the http://xapian.org/docs/bindings/php[Xapian PHP
binding] to access the original C++ library.
They are independent from each other.

=== About Xapian ===

Xapian is a search engine library with a highly adaptable toolkit which
allows developers to easily add advanced indexing and search facilities
to their own applications.
It implements the probabilistic model of information retrieval, and
provides facilities for performing ranked free-text searches, relevance
feedback, phrase searching, boolean searching, stemming, and simultaneous
update and searching.
It is highly scalable, and is capable of working with collections
containing hundreds of millions of documents.

=== Features ===

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/xapian

=== xapian module ===

* Let you index and search drupal content inside nodes.

=== search_api_xapian module ===

* Provides two Search API backend server types: one for handling a local
  xapian database and the other for handling a remote xapian database
  running by xapian-tcpsrv.

== Requirements ==

* External packages (both modules):
** Xapian
** Xapian's PHP5 bindings (on the web server)
* Drupal modules:
** For xapian module
*** search
** For search_api_xapian
*** search_api (and its dependencies)

== Installation of the Xapian requirements ==

The next documentation will summarize what you need to do to install
Xapian.

=== Debian/Ubuntu ===

aptitude install php5-xapian

Note: With Ubuntu Hardy, the library was not put in the built in include
path. Running this command resolved that

sudo ln -s /usr/share/php5/xapian.php /usr/share/php/xapian.php

==== Debian Squeeze (6.0) ====

There was a licence problem on Debian Squeeze, so php5-xapian is not
included as debian binary package. To solve that problem, you can build
your own php5-xapian package.
To see details on the problem and the original source of this steps see
http://trac.xapian.org/wiki/FAQ/PHP%20Bindings%20Package and
http://trac.xapian.org/ticket/191

[source,bash]
----
apt-get build-dep xapian-bindings
apt-get install php5-dev php5-cli devscripts
apt-get source xapian-bindings
cd xapian-bindings-1.2.*
rm debian/control
env PHP_VERSIONS=5 debian/rules maint
debuild -e PHP_VERSIONS=5 -us -uc
cd ..
dpkg -i php5-xapian_*.deb
----

If you are using php 5.4(wheezy), change the debuild command to:
[source,bash]
----
env DEB_BUILD_OPTIONS=nocheck debuild -e PHP_VERSIONS=5 -us -uc
----


=== Redhat/CentOS/Fedora ===

yum install xapian-core-libs xapian-core

=== By hand ===

At worst, you may have to build it from source, but that is not hard to
do.
The official documentation is on:

* http://www.xapian.org/docs/install.html
* http://www.xapian.org/download.php

== Installation ==

* Install xapian and its bindings (see Installation of the Xapian
requirements)
* Install as usual, see http://drupal.org/node/70151 for further
information.

== Configuration ==

=== xapian module ===

Customize the  settings in Administer >> Site configuration >>
Search settings >> Xapian search settings

Database::
Choose whether to use a local or remote database.
For the local database, the "local database options" fields must be
populated, specifically the path to the search database.
+
By default, the database will be created in the drupal files area, 
ensuring that the web server user has access to update the database.
+
For the remote database, the "remote database options" fields must be 
populated, and hostname and port being needed. See "Advanced Xapian usage"
for more information.

Performance::
If you have the "Index immediately", then the xapian module will process
changed/updated nodes when when those actions are performed, in the other
case it will be at cron time.

Display::
The result count field is used to determine how Xapian returns the
total number of results. Xapian does not return an exact number, it
estimates the results. This setting modifies that behaviour.

Logging::
It is also possible to enable query logging, which records both the search 
query and the time required to execute it. Once you have patched drupal
core, the response times for the drupal equivalent query are also logged,
making a convenient way to test the difference the Xapian module and the
standard drupal search.

Excluded node types::
Decide the node types you want to avoid indexing.

Algorithms::
Change the steaming language you want to use.

=== search_api_xapian module ===

The configuration is different for each backedn server type, but the
documentation for xapian module above covers all the options provided.

== Advanced Xapian usage ==

Xapian has the built in ability to host the database on a separate
machine than the web server. It you'd like to do this, the remote host
must have xapian installed and then they must run
`xapian-tcpsrv --port <port> <path/to/database>`, which will start a
server that can be accessed by the Xapian library. The Drupal module
defaults to port 6431, but any port can be used.

== Credits ==

5.x-1.x::
+
The original proof of concept developed for
http://www.taniwhasolutions.com[Taniwha Solutions].
+
Original development sponsored by http://www.trellon.com[Trellon].

6.x-1.x::
+
Initial development and sample view code by Lachlan Gunn
(http://drupal.org/user/277696).
+
Core patch, tighter drupal integration, adherance to drupal standards,
making ready for release by Simon Lindsay (http://drupal.org/user/143).
+
Additional cleanup by Jeremy Andrews (http://drupal.org/user/409).
+
Ongoing maintenance sponsored by http://tag1consulting.com[Tag1
Consulting] and http://drupal.org/user/132175[marvil07].

6.x-2.x and 7.x-1.x::
+
search_api_xapian, the implementation of
http://drupal.org/project/search_api[Search API project] API by
http://drupal.org/user/132175[marvil07]
+
Current maintainers.

=== Current Maintainers ===

Jeremy and marvil07.

// vim: set syntax=asciidoc:
