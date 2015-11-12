url='http://localhost:8983/solr/wordnet/update'

java WordnetSyns2Solr ../prolog wordnet.xml

curl $url --data-binary '<delete><query>*:*</query></delete>' -H "Content-type: application/xml"
curl $url --data-binary @wordnet.xml -H "Content-type: application/xml"
curl $url --data-binary '<commit/>' -H "Content-type: application/xml"
curl $url --data-binary '<optimize/>' -H "Content-type: application/xml"
