/**
* @file JavaScript for classic marker.
*/

(function() {
    var ferank = document.createElement('script');
    ferank.type = 'text/javascript';
    ferank.async = true;
    ferank.src = ('https:' == document.location.protocol ? 'https://static' : 'http://static') + '.ferank.fr/pixel.js';
    var s = document.getElementsByTagName('script')[0];
    s.parentNode.insertBefore(ferank, s);
})();
