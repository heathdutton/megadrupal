README.txt
==========

Description:
-----------

This module will help site administrator in tagging content. It will 
automatically find important tags from node content. It will create different 
drupal vocabularies and add tags to different Drupal vocabularies, which will 
help in rdfing the content. The rdf

Installation and Use:
---------------------
1) Download the known list from http://gsoc.chia.in/sites/default/files/data.tar.gz
2) Extract files in the data folder
3) Enable the module
4) Configure the module settings at admin/config/autordf
5) Enable auto tagging for your node-type  


New sites dont have predefined data for categorization of Named entities. So
I have collected a list to suggest Name Entity.
One can download the known list to help Autordf Better Suggest tags,
These names have exactly similar URLs in http://dbpedia.org/resource/[NamedEntity]

You can also train data from node/*/autordf to help module automatically identify
the word.


RoadMap
-------
Plan to Expose this as a service, a website can export its taxonomy tree and server
can suggest possible category based on existing knowledge available.

For Suggestions and Improvements
--------------------------------
Drop a request in issue queue http://drupal.org/node/add/project-issue/autordf  