
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Initial setup
 * Customization
 * How to use

INTRODUCTION
------------

Current Maintainers: Baheerathan Vykundanathan <thamba@allerinternett.no>
Current Maintainers: adam7 <adambell7@gmail.com>

The Nodeflow module is meant for large publishing sites that usually involve a 
workflow process where articles created by several journalists are sent to an 
editor for approval and final publishing. The editor needs a birds-eye view of 
all articles waiting to be published so he can just drag and drop them around 
to publish articles and sort their positions on the front page of the site 
easily.


INSTALLATION
------------

The installation of this module is fairly straight-forward.

1. Copy this nodeflow/ directory to your modules directory.

2. Enable the module. Make sure the dependencies are installed/enabled.


INITIAL SETUP
-------------

After enabling the module, please follow these guidelines to get started.

1. On installation/enabling, you need to go into admin/structure/nodeflow to 
setup a nodeflow queue. Use this page to create as many queues as needed.
The nodeflow module also creates a page and adds it to your administration 
menu from where you can control your nodes.

2. The module also creates three default views, one that lists all nodes in 
"draft" status and another that lists all nodes in "published" status and the 
third is a default frontpage view setup.

3. On the nodeflow page you will first have to create a nodeflow queue, and then
populate the queue with articles by clicking on the "Nodeflow populate" 
button. Make sure you have at least one node that is in the published status and
of content type article.

4. There is a configuration page on admin/config/system/nodeflow where you can
configure certain behavioral aspects of nodeflow.


CUSTOMIZATION
-------------

* Nodeflow queues allow you to create multiple queues.  Each site needs
a minimum of one queue.

* Since nodeflow uses views, these views can be customized to display any 
fields that you wish.

* For example: The draft view can be modified to fetch nodes from another 
status defined in your workbench workflow.

* The frontpage view can be modified in any way you like except you need to 
keep the nodeflow relationship and the sort by nodeflow positions.
