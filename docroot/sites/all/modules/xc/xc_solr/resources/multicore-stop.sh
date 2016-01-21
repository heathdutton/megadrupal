# -Djetty.port=8984
java -DSTOP.PORT=9094 -DSTOP.KEY=multicorestop -Dsolr.solr.home=multicore -Xms1024M -Xmx2048M -jar start.jar --stop
