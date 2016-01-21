###Mica
------------------------

Mica is a powerful software application used to create web portals for
epidemiological study consortia.

Using Mica, consortia can build personalised web sites for publishing their
research activities and their membership.
Mica includes many domain-specific features such as study catalogues, data
dictionary browsers, on-line data access
request forms, community tools (forums, events, news) and others.

Mica Drupal 7 Client  is the  way to get you up and running with
[Mica](http://www.obiba.org/pages/products/mica2/).

Mica Migration module is helper to export Studies/networks/datasets
from Obiba's [Mica 9.x](http://demo.obiba.org/mica/)
[Drupal distribution](https://www.drupal.org/project/mica_distribution)
to [Mica2 1.x server](http://www.obiba.org/pages/products/mica2/), the new
version of the Mica studies catalog.
It generates zip files of packaged .json files.


###REQUIREMENTS
------------------------

Mica Migration  depends on :
1. Enabling mica_export Mica9.x modules
2. Download [mica_protobuf_lib](https://github.com/smirfolio/mica_protobuf_lib)
 in the sites/all/libraries,  if you have access to drush command you can get
 it by typing  : drush get-protobuf in your current mica installation


------------------------
This project is sponsored by OBiBa [http://www.obiba.org].
OBiBa is a collaborative international project whose mission is to build
high-quality open source software for biobanks.
