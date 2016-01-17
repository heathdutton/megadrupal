This module converts text and Tex in an HTML texarea to PDF.

Requirements include

1) html2ps
2) ps2pdf
3) mimeTex CGI script
4) markItUp

INSTALLATION

On most Linux installation you can install the requirements from the command line except for markItUp which must be downloaded from http://markitup.jaysalvat.com and unpackaged in the 'libraries' directory of Drupal. You may want to ask your systems administrator for help or create a support issue from the project home page.

On Ubuntu systems you can install html2ps and ps2pdf by using this command:

sudo apt-get install html2ps

The previous command should also install the ps2pdf binary.

To install mimeTex go to http://www.forkosh.com/mimetex.html and move the mimeTeX CGI script to your cgi-bin directory or

sudo apt-get install mimetex

When these are completed, uncompress the Drupal LaTeX Toolbar module in the modules directory and install normally.

Fill in paths to the appropriate directory from admin/config/media/texbar.

To create the toolbar create a textarea with the ID attribute set to your specified tag setting.

Note: html2ps has a file where you can style HTML. See the html2ps man page for additional information.

Project Home: http://drupal.org/project/texbar

Demonstration Page: http://demo.tetragy.com/texbar

This module is sponsored by Tetragy Limited: https://www.tetragy.com

