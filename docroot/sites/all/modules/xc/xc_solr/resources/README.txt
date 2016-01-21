These files are Solr configuration files.
schema-for-[Solr version].xml - the schema XML for different Solr versions
solrconfig-for-[Solr version].xml - the configuration file for different Solr versions

Put these files into the Solr conf directory as schema.xml and solrconfig.xml.
If you make use of multicore Solr setup, be sure, that you copied these files into
the core(s) you want to use with Drupal Toolkit and also the following Solr
configuration files:

synonyms.txt
stopwords.txt
stopwords_en.txt
protwords.txt
elevate.xml

If you use 3.1 and keep ICUFoldingFilterFactory in the schema - which is good to removing
accents from composite type UTF-8 characters, and enable searching with or without
accents, create a "lib" directory under Solr home directoy (the new directory will be
apache-solr-xxx/example/solr/lib) or directories, if you use multicore
(apache-solr-xxx/example/multicore/your core #1/lib, apache-solr-xxx/example/multicore/your core #2/lib
etc. or apache-solr-xxx/example/multicore/lib if the apache-solr-xxx/example/multicore/solr.xml
contains a sharedLib attribute of the <solr> element like in <solr persistent="false" sharedLib="lib">)
with the following files (in parens we mentioned the directory in which the
file originally located relative to the downloadable Apache Solr package):

- apache-solr-analysis-extras-3.x.0.jar (dist)
- icu4j-xxx.jar (contrib/analysis-extras/lib)
- lucene-icu-3.x.0.jar (contrib/analysis-extras/lucene-libs)

Files to start/stop Solr:
solr.sh
solr.start
solr.stop
