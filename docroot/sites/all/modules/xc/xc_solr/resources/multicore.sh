# -Djetty.port=8984
java -DSTOP.PORT=9094 -DSTOP.KEY=multicorestop -Dsolr.solr.home=multicore -Xms1024M -Xmx6000M -server -Djava.util.logging.config.file=logging.properties -XX:+UseConcMarkSweepGC -XX:+UseParNewGC -XX:ParallelGCThreads=2 -XX:SurvivorRatio=2 -XX:MaxPermSize=256M -Djava.awt.headless=true -jar start.jar
