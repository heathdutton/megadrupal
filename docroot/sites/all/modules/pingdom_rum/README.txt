Pingdom is a provider of website monitoring services.

They have a service called Real User Monitoring.

On signing up for this, you are presented with an inline Javascript snippet
to paste into the HEAD section of the website you wish to monitor.
This module inserts that code for you.

INSTALLATION

Download this module into /sites/all/modules, and enable it for your site.

Now find the 24-character hexadecimal string that is the unique identifier
for your project. You can find this in two ways.
1. Go to https://my.pingdom.com/rum,
  where there is a link to your RUM project(s).
  The last part of the URL for your project is your project identifier.
  For example, if your project is managed from
                https://my.pingdom.com/rum/0123456789abcdef01234567,
  then your project Id is 0123456789abcdef01234567.
2. Look at the Javascript snippet that Pingdom ask you to insert.
  It begins
 <script type="application/javascript">var _prum={id:"0123456789abcdef01234567"}
  in this case, 0123456789abcdef01234567 would be your project identifier.

Having found your project identifier, go to /admin/config/services/pingdom.
There you can enter this project identifier.

View the source on a page on your website to verify that the snippet is now
in the HEAD section.

You should then be able to go to your Real User Monitor project on Pingdom.
Choose the "real time" monitoring option.
Visits to your webpage should come through in real time.
